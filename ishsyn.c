/*--------------------------------------------------------------------*/
/* ishsyn.c                                                           */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "token.h"
#include "linereader.h"
#include "lexdfa.h"
#include "ish.h"
#include "command.h"
#include "synAnalyze.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/*--------------------------------------------------------------------*/

/* The name of the executable binary file. */
const char* pcPgmName;

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

    /* Continually print prompt and analyze line. */
	while((pcLine = LineReader_read(stdin)) != NULL)
	{
		printf("%s\n", pcLine);
		iRet = fflush(stdout);
		if (iRet == EOF)
			{perror(pcPgmName); exit(EXIT_FAILURE);}

		/* Lex analyze. */
		oTokens = LexDFA_lexLine(pcLine);

		oCommand = NULL;

		printf("%s", "");

		/* Syn analyze if oTokens is not NULL. */
		if (oTokens != NULL)
		{
			oCommand = SynAnalyze_analyze(oTokens);
			LexDFA_freeTokens(oTokens);
			DynArray_free(oTokens);
		}

		/* Write and free oCommand (if not NULL). */
		if (oCommand != NULL)
		{
			Command_write(oCommand);
			Command_free(oCommand);
		}

		if (pcLine != NULL) free(pcLine);
		printf("%c ", '%');
	}
	printf("\n");
	return 0;
}
