/*--------------------------------------------------------------------*/
/* lexdfa.h                                                           */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#ifndef LEXDFA_INCLUDED
#define LEXDFA_INCLUDED

/*--------------------------------------------------------------------*/

/* Write all tokens in oTokens to stdout.  First write the number
   tokens; then write the word tokens. */

void LexDFA_writeTokens(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Free all of the tokens in oTokens. */

void LexDFA_freeTokens(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

/* Lexically analyze string pcLine.  If pcLine contains a lexical
   error, then return NULL.  Otherwise return a DynArray object
   containing the tokens in pcLine.  The caller owns the DynArray
   object and the tokens that it contains. */

DynArray_T LexDFA_lexLine(const char *pcLine);

#endif
