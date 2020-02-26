/*--------------------------------------------------------------------*/
/* ish.c                                                              */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#define _GNU_SOURCE

#include "dynarray.h"
#include "token.h"
#include "linereader.h"
#include "lexdfa.h"
#include "command.h"
#include "synAnalyze.h"
#include "ish.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

/*--------------------------------------------------------------------*/

/* The name of the executable binary file. */
const char* pcPgmName;

/*--------------------------------------------------------------------*/

/* Redirects oCommand's commands to the proper files, both 
   standard input and standard output. */

static void redirect(Command_T oCommand)
{
	char* currStdin; 
	char* currStdout; 
	int fd;
	int ret;

	currStdin = Command_getStdIn(oCommand);
	if (currStdin != NULL)
	{
		fd = open(currStdin, O_RDONLY);
		if (fd == -1) {perror(pcPgmName); exit(EXIT_FAILURE);}

		ret = close(0);
		if (ret == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

		ret = dup(fd);
		if (ret == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

		ret = close(fd);
		if (ret == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }
	}

	currStdout = Command_getStdOut(oCommand);
	fflush(stdout);
	if (currStdout != NULL)
	{
		fd = creat(currStdout, 0600);
		if (fd == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

		ret = close(1);
		if (ret == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

		ret = dup(fd);
		if (ret == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

		ret = close(fd);
		if (ret == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }
	}
}

/*--------------------------------------------------------------------*/

static void executeCommand(Command_T oCommand)
{
	size_t ulArgumentCount;
	pid_t iPid;
	int iRet;
	DynArray_T oArguments;
	char** ppcArguments;
	char* commandName;
	char* HOME;

	/* Define dynarray of arguments. */
	oArguments = Command_getArguments(oCommand);
	ppcArguments = Command_getArgsArray(oCommand);

	/* Store number of elements in dynarray. */
	if (oArguments == NULL) ulArgumentCount = 0;
	else ulArgumentCount = DynArray_getLength(oArguments);

	/* Store command name. */
	commandName = Command_getName(oCommand);

	/* Execute exit command. */
	if (strcmp(commandName, "exit") == 0)
	{
		/* Free oCommand. */
		exit(0);
	}

	/* Execute setenv command. */
	else if (strcmp(commandName, "setenv") == 0)
	{
		/* Ensure not to few aruments.. */
		if (ulArgumentCount == 0)
		{
			fprintf(stderr, "%s: missing variable\n", pcPgmName);
		}

		/* ...or too many. */
		else if (ulArgumentCount > 2)
		{
			fprintf(stderr, "%s: too many arguments\n", pcPgmName);
		}

		/* Execute with empty string as value if only 1 arg. */
		else if (ulArgumentCount == 1)
		{
			setenv(DynArray_get(oArguments, 0), "", 1);
		}

		/* Otherwise, execute with additional argument. */
		else
		{
			setenv(DynArray_get(oArguments, 0), 
				   DynArray_get(oArguments, 1), 1);
		}
	}

	/* Execute unsetenv command. */
	else if (strcmp(commandName, "unsetenv") == 0)
	{
		/* Ensure not missing variables... */
		if (ulArgumentCount < 1)
		{
			fprintf(stderr, "%s: missing variable\n", pcPgmName);
		}

		/* ...or too many variables. */
		else if (ulArgumentCount > 1)
		{
			fprintf(stderr, "%s: too many arguments\n", pcPgmName);
		}

		/* Otherwise, execute unsetenv. */
		else unsetenv(DynArray_get(oArguments, 0));
	}

	/* Execute cd commands. */
	else if (strcmp(commandName, "cd") == 0)
	{

		/* Make sure there aren't too many args... */
		if (ulArgumentCount > 1)
		{
			fprintf(stderr, 
					"%s: too many arguments\n",
					pcPgmName);
		}

		/* ...and that if there are zero args that HOME is set. */
		else if (ulArgumentCount == 0)
		{
			HOME = getenv("HOME");
			if (HOME != NULL)
			{
				chdir(HOME);
			}
			else 
			{
				fprintf(stderr, 
					"%s: HOME environemnt variable not set\n",
					pcPgmName);
			}

		}

		/* Otherwise, call change directory. */
		else
		{
			chdir(DynArray_get(oArguments, 0));
		}
	}

	/* Otherwise, the command is external. */
	else
	{

		iRet = fflush(stdin);
		if (iRet == EOF) {perror(pcPgmName); exit(EXIT_FAILURE); }
		iRet = fflush(stdout);
		if (iRet == EOF) {perror(pcPgmName); exit(EXIT_FAILURE); }

		iPid = fork();
		if (iPid == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }

		if (iPid == 0)
		{
			/* Redirect if needed to stdin and stdout. */
			redirect(oCommand);

			/* Execute external command. */
			execvp(commandName, ppcArguments);

			fprintf(stderr, 
				    "%s: No such file or directory\n", 
				    pcPgmName);
			exit(EXIT_FAILURE);
		}
		iPid = wait(NULL);
		if (iPid == -1) {perror(pcPgmName); exit(EXIT_FAILURE); }
	}
	Command_free(oCommand);
	free(ppcArguments);
}

/*--------------------------------------------------------------------*/

/* Program main that returns an int. 
   int argc is the number of arguments, *argv[] an array of the 
   arguments. */

int main(int argc, char *argv[])
{
	char *pcLine;
	DynArray_T oTokens;
	Command_T oCommand;
	int iRet;

	pcPgmName = argv[0];

    printf("%c ", '%');

    /* Continually analyze stdin. */
	while((pcLine = LineReader_read(stdin)) != NULL)
	{
		printf("%s\n", pcLine);
		iRet = fflush(stdout);
		if (iRet == EOF)
			{perror(pcPgmName); exit(EXIT_FAILURE);}

		/* Lex analyze. */
		oTokens = LexDFA_lexLine(pcLine);

		oCommand = NULL;

		/* Make command is oTokens != NULL. */
		if (oTokens != NULL)
		{
			oCommand = SynAnalyze_analyze(oTokens);
			LexDFA_freeTokens(oTokens);
			DynArray_free(oTokens);
		}

		/* Execute command if it is not null. */
		if (oCommand != NULL)
		{
			executeCommand(oCommand);
		}

		if (pcLine != NULL) free(pcLine);
		printf("%c ", '%');
	}
	printf("\n");
	return 0;
}