/*--------------------------------------------------------------------*/
/* synAnalyze.c                                                       */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#include "synAnalyze.h"
#include "token.h"
#include "ish.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*--------------------------------------------------------------------*/

enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

/* Accept a dynamic array of oTokens and return a Command object,
   unless the dynamic array conatians errors. In the case of errors,
   return NULL. */

Command_T SynAnalyze_analyze(DynArray_T oTokens)
{
	size_t index;
	Command_T oCommand;
	DynArray_T oArguments = NULL;
	int stdInPresent = FALSE;
	int stdOutPresent = FALSE; 
	int argsCount = 0;
	Token_T oToken = NULL;
	char* stdIn = NULL;
	char* stdOut = NULL;
	char* commandName; 

	assert(oTokens != NULL);
	if (DynArray_getLength(oTokens) == 0) return NULL;

	/* Store the first token as commandName.
	   Make sure it's ORDINARY. */
	oToken = DynArray_get(oTokens, 0);

	if (Token_getType(oToken) != TOKEN_ORDINARY)
	{
		fprintf(stderr, "%s: missing command name\n", pcPgmName);
		return NULL;
	}

	commandName = Token_getVal(oToken);

	/* Traverse oTokens to find command, StdIn, & Stdout. */
	for (index = 1; index < DynArray_getLength(oTokens); index++)
	{
		oToken = DynArray_get(oTokens, index);

		if (Token_getType(oToken) == TOKEN_SPECIAL)
		{
			/* If the special token is StdIn... */
			if (strcmp(Token_getVal(oToken), "<") == 0)
			{
				/* ...make sure there isn't more than one... */
				if (stdInPresent == TRUE)
				{
					fprintf(stderr, 
						"%s: multiple redirection of standard input\n",
						pcPgmName);
					return NULL;
				}

				/* ... mark stdInPresent as TRUE.... */
				stdInPresent = TRUE;

				/* ... make sure there is a file destination... */
				/* (ensure not at the end of the dynarray) */
				if (index == (DynArray_getLength(oTokens) - 1))
				{
					fprintf(stderr, 
						"%s: standard input redirection",
						pcPgmName);
					fprintf(stderr, " without file name\n");
					if (oArguments != NULL) DynArray_free(oArguments);
					return NULL;
				}

				/* ...if there is another token, save it as StdIn. */
				oToken = DynArray_get(oTokens, ++index);

				/* If the next token is SPECIAL, return error. */
				if (Token_getType(oToken) == TOKEN_SPECIAL)
				{
					fprintf(stderr, 
						"%s: standard input redirection",
						pcPgmName);
					fprintf(stderr, " without file name\n");
					if (oArguments != NULL) DynArray_free(oArguments);
					return NULL;
				}

				/* Otherwise, save the ordinary token as StdIn. */
				stdIn = Token_getVal(oToken);
			}

		/* Otherwise, if the token is StdOut... */
		else 
		{
				/* ...make sure there isn't more than one... */
			if (stdOutPresent == TRUE)
			{
				fprintf(stderr, 
					"%s: multiple redirection of standard output\n",
					pcPgmName);
				if (oArguments != NULL) DynArray_free(oArguments);
				return NULL; 
			}

			stdOutPresent = TRUE;

				/* ...make sure there is a file destination... */
			if (index == (DynArray_getLength(oTokens) - 1))
			{
				fprintf(stderr, 
					"%s: standard output redirection",
					pcPgmName);
				fprintf(stderr, " without file name\n");
				if (oArguments != NULL) DynArray_free(oArguments);
				return NULL;
			}

				/* ...if there is another token, save it as StdOut. */
			oToken = DynArray_get(oTokens, ++index);

				/* If the next tokem is SPECIAL, return error. */
			if (Token_getType(oToken) == TOKEN_SPECIAL)
			{
				fprintf(stderr, 
					"%s: standard input redirection",
					pcPgmName);
				fprintf(stderr, " without file name\n");
				if (oArguments != NULL) DynArray_free(oArguments);
				return NULL;
			}

			stdOut = Token_getVal(oToken);
		}
	}

		/* Otherwise, if the token is ORDINARY. */
		else
		{
			/* Make new dynarray is there are no args yet. */
			if (argsCount == 0)
			{
				oArguments = DynArray_new(0);

				/* Return error if unsufficient memory. */
				if (oArguments == NULL)
					{perror(pcPgmName); exit(EXIT_FAILURE);}

				argsCount++;
			}

			/* Add the argument to the dynarray. */
			DynArray_add(oArguments, Token_getVal(oToken));
		}
	}

	/* Call Command to make the new Command with StdIn, StdOut,
	   commandName, and oArguments. */
	oCommand = Command_new(commandName, stdIn, stdOut, oArguments);

	/* Return error if unsufficient memory. */
	if (oCommand == NULL)
		{perror(pcPgmName); exit(EXIT_FAILURE);}

	if (argsCount > 0) DynArray_free(oArguments);

	return oCommand;
}

