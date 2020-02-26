/*--------------------------------------------------------------------*/
/* lexdfa.c                                                           */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include "token.h"
#include "lexdfa.h"
#include "ish.h"
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*--------------------------------------------------------------------*/


/* Write all tokens in oTokens to stdout.  First write the number
   tokens; then write the word tokens. */

void LexDFA_writeTokens(DynArray_T oTokens)
{
   size_t u;
   size_t uLength;
   struct Token *psToken;

   assert(oTokens != NULL);

   uLength = DynArray_getLength(oTokens);

   for (u = 0; u < uLength; u++)
   {
   	psToken = DynArray_get(oTokens, u);
   	Token_writeToken(psToken);
   }
}

/*--------------------------------------------------------------------*/

/* Free all of the tokens in oTokens. */

void LexDFA_freeTokens(DynArray_T oTokens)
{
   size_t u;
   size_t uLength;
   struct Token *psToken;

   assert(oTokens != NULL);

   uLength = DynArray_getLength(oTokens);

   for (u = 0; u < uLength; u++)
   {
      psToken = DynArray_get(oTokens, u);
      Token_freeToken(psToken);
   }
}

/*--------------------------------------------------------------------*/

/* Lexically analyze string pcLine.  If pcLine contains a lexical
   error, then return NULL.  Otherwise return a DynArray object
   containing the tokens in pcLine.  The caller owns the DynArray
   object and the tokens that it contains. */

DynArray_T LexDFA_lexLine(const char *pcLine)
{
   /* lexLine() uses a DFA approach.  It "reads" its characters from
      pcLine. The DFA has these three states: */
   enum LexState {STATE_START, STATE_IN_TOKEN, STATE_IN_LITERAL, 
   	STATE_SPECIAL, STATE_END_LITERAL};

   /* The current state of the DFA. */
   enum LexState eState = STATE_START;

   /* An index into pcLine, a pointer to a buffer of text 
      in which the characters comprising each token are 
      accumulated, and an index into the buffer of text. */ 
   size_t ulLineIndex = 0;
   char *pcText;
   int ulTextIndex = 0;

   char c;
   struct Token *psToken;
   DynArray_T oTokens;
   int iSuccessful;

   assert(pcLine != NULL);

   /* Create an empty token DynArray object. */
   oTokens = DynArray_new(0);
   if (oTokens == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}

   /* Allocate memory for a buffer that is large enough to store the
      largest token that might appear within pcLine. */
   pcText = (char*)malloc(strlen(pcLine) + 1);
   if (pcText == NULL)
      {perror(pcPgmName); exit(EXIT_FAILURE);}

   for (;;)
   {
      /* "Read" the next character from pcLine. */
      c = pcLine[ulLineIndex];
      ulLineIndex++;

      switch (eState)
      {
         /* Handle the START state. */
         case STATE_START:
            if (c == '\0')
            {
               free(pcText);
               return oTokens;
            }
            else if (isspace(c))
            {
            	eState = STATE_START;
            }
            else if ((c == '>') || (c == '<'))
            {
            	pcText[ulTextIndex++] = c;
            	eState = STATE_SPECIAL;
            }
            else if (c == '\"')
            {
            	eState = STATE_IN_LITERAL;
            }
            else
            {
            	pcText[ulTextIndex++] = c;
            	eState = STATE_IN_TOKEN;
            }
            break;

         /* Handle the IN_NUMBER state. */
         case STATE_IN_TOKEN:
            if (c == '\0')
            {
               /* Create an ORDINARY token. */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_ORDINARY, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
               free(pcText);
               return oTokens;
            }
            else if (isspace(c))
            {
               /* Create an ORDINARY token. */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_ORDINARY, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
               eState = STATE_START;
            }
            else if (c == '"')
            {
            	eState = STATE_IN_LITERAL;
            }
            else if ((c == '<') || c == '>')
            {
               /* Create an ORDINARY token. */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_ORDINARY, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;

               pcText[ulTextIndex++] = c;
               eState = STATE_SPECIAL;
            }
            else
            {
            	pcText[ulTextIndex++] = c;
            	eState = STATE_IN_TOKEN;
            }
            break;

         /* Handle the IN_LITERAL state. */
         case STATE_IN_LITERAL:
            if (c == '\0')
            {
               /* Print umatched quote error. */
               fprintf(stderr, "%s: unmatched quote\n", pcPgmName);
               ulTextIndex = 0;
               free(pcText);
               LexDFA_freeTokens(oTokens);
               DynArray_free(oTokens);
               return NULL;
            }
            else if (c == '\"')
            {
            	eState = STATE_END_LITERAL;
            }
            else
            {
               pcText[ulTextIndex++] = c;
               eState = STATE_IN_LITERAL;
            }
            break;

         /* Handle END_LITERAL state. */
         case STATE_END_LITERAL:
            if (c == '\0')
            {
            	/* Create an ORDINARY token. */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_ORDINARY, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
               free(pcText);
               return oTokens;
            }
            else if (isspace(c))
            {
            	/* Create an Ordinary Token */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_ORDINARY, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
  
               eState = STATE_START;
            }
            else if (c == '\"')
            {
               eState = STATE_IN_LITERAL;
            }
            else if ((c == '<') || (c == '>'))
            {
            	/* Create an ORDINARY token. */
            	pcText[ulTextIndex] = '\0';
            	psToken = Token_newToken(TOKEN_ORDINARY, pcText);
            	iSuccessful = DynArray_add(oTokens, psToken);
            	if (! iSuccessful)
            		{perror(pcPgmName); exit(EXIT_FAILURE);}
            	ulTextIndex = 0;
            	eState = STATE_SPECIAL;
            }
            else
            {
            	pcText[ulTextIndex++] = c;
            	eState = STATE_IN_TOKEN;
            }
            break;

        /* Handle SPECIAL state. */
        case STATE_SPECIAL:
            if (c == '\0')
            {
               /* Create a SPECIAL token. */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_SPECIAL, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
               free(pcText);
               return oTokens;
            } 
            else if (isspace(c)) 
            {
               /* Create an SPECIAL Token */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_SPECIAL, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
  
               eState = STATE_START;
           }
           else if (c == '\"')
           {
           	   /* Create a SPECIAL Token */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_SPECIAL, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;
  
               eState = STATE_IN_LITERAL;
           }
           else if ((c == '<') || (c == '>'))
           {
           	   /* Create an SPECIAL Token */
               pcText[ulTextIndex] = '\0';
               psToken = Token_newToken(TOKEN_SPECIAL, pcText);
               iSuccessful = DynArray_add(oTokens, psToken);
               if (! iSuccessful)
                  {perror(pcPgmName); exit(EXIT_FAILURE);}
               ulTextIndex = 0;

           	pcText[ulTextIndex++] = c;
           	eState = STATE_SPECIAL;
           }
           else 
           {
           		pcText[ulTextIndex] = '\0';
           		psToken = Token_newToken(TOKEN_SPECIAL, pcText);
           		iSuccessful = DynArray_add(oTokens, psToken);
            	if (! iSuccessful)
            		{perror(pcPgmName); exit(EXIT_FAILURE);}
            	ulTextIndex = 0;
            	pcText[ulTextIndex++] = c;
            	eState = STATE_IN_TOKEN;
            }
            break;

         default:
            assert(0);
      }
   }
}