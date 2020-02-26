/*--------------------------------------------------------------------*/
/* ishlex.c                                                           */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "token.h"
#include "linereader.h"
#include "lexdfa.h"
#include "ish.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/*--------------------------------------------------------------------*/

/* The name of the executable binary file. */
const char *pcPgmName;

/*--------------------------------------------------------------------*/

/* Program main that returns an int. 
   int argc is the number of arguments, *argv[] an array of the 
   arguments. */

int main(int argc, char *argv[])
{
	char *pcLine;
	DynArray_T oTokens;
	int iRet;

	pcPgmName = argv[0];

    printf("%c ", '%');

	while((pcLine = LineReader_read(stdin)) != NULL)
	{
		printf("%s\n", pcLine);
		iRet = fflush(stdout);
		if (iRet == EOF)
			{perror(pcPgmName); exit(EXIT_FAILURE);}

		oTokens = LexDFA_lexLine(pcLine);

		if (oTokens != NULL)
		{
			LexDFA_writeTokens(oTokens);
			LexDFA_freeTokens(oTokens);
			DynArray_free(oTokens);
		}

		free(pcLine);
		printf("%c ", '%');
	}
	printf("\n");
	return 0;
}
