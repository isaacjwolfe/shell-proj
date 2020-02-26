/*--------------------------------------------------------------------*/
/* token.h                                                            */
/* Author: Isaac Wolfe	                                              */
/*--------------------------------------------------------------------*/

#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

#include <stddef.h>

/*--------------------------------------------------------------------*/

/* A Token object can be either a special or ordinary. */

enum TokenType {TOKEN_SPECIAL, TOKEN_ORDINARY};

/*--------------------------------------------------------------------*/

/* A token object is a sequence of non-white-space characters that is 
   separated from other tokens by white-space characters. The special 
   characters '<' and '>' form separate special tokens. Strings 
   enclosed in double quotes (") form part or all of a single token. */

typedef struct Token *Token_T;

/*--------------------------------------------------------------------*/

/* Write a single token, psToken, to stdout. */

void Token_writeToken(Token_T psToken);

/*--------------------------------------------------------------------*/

/* Free token psToken. */

void Token_freeToken(Token_T psToken);

/*--------------------------------------------------------------------*/

/* Create and return a token whose type is eTokenType and whose
   value consists of string pcValue.  The caller owns the token. */

Token_T Token_newToken(enum TokenType eTokenType,
   char *pcValue);

/*--------------------------------------------------------------------*/

/* Return the eType of oToken. */

enum TokenType Token_getType(Token_T oToken);

/*--------------------------------------------------------------------*/


/* Return the string value of oToken. */

char* Token_getVal(Token_T oToken);

/*--------------------------------------------------------------------*/



#endif

