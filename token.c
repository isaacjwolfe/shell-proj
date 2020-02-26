/*--------------------------------------------------------------------*/
/* token.c                                                            */
/* Author: Isaac Wolfe	                                              */
/*--------------------------------------------------------------------*/

#include "token.h"
#include "ish.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/

/* A Token is either a number or a word, expressed as a string. */

struct Token
{
   /* The type of the token. */
   enum TokenType eType;

   /* The string which is the token's value. */
   char *pcValue;
};

/*--------------------------------------------------------------------*/

/* Write a single token, psToken, to stdout. */

void Token_writeToken(Token_T psToken)
{
	assert(psToken != NULL);

	   	printf("Token: %s ", psToken->pcValue);
   	if (psToken->eType == TOKEN_SPECIAL)
   		printf("(special)\n");
   	else printf("(ordinary)\n");
}

/*--------------------------------------------------------------------*/

/* Free token psToken. */

void Token_freeToken(Token_T psToken)
{
	assert(psToken != NULL);

	free(psToken->pcValue);
	free(psToken);
}

/*--------------------------------------------------------------------*/

/* Create and return a token whose type is eTokenType and whose
   value consists of string pcValue.  The caller owns the token. */

Token_T Token_newToken(enum TokenType eTokenType,
   char *pcValue)
{
   Token_T psToken;

   /* assert pcValue is not NULL. */
   assert(pcValue != NULL);

   /* Allocate memory and create token (copied over). */
   psToken = (struct Token*)malloc(sizeof(struct Token));
   if (psToken == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}
   psToken->eType = eTokenType;
   psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
   if (psToken->pcValue == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}
   strcpy(psToken->pcValue, pcValue);

   return psToken;
}

/*--------------------------------------------------------------------*/

/* Return the eType of oToken. */

enum TokenType Token_getType(Token_T oToken)
{
   assert(oToken != NULL);

   return oToken->eType;
}

/*--------------------------------------------------------------------*/

/* Return the string value of oToken. */

char* Token_getVal(Token_T oToken)
{
   assert(oToken != NULL);

   return oToken->pcValue;
}


