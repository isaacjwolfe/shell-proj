/*--------------------------------------------------------------------*/
/* synAnalyze.h                                                       */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#ifndef SYNANALYZE_INCLUDED
#define SYNANALYZE_INCLUDED

#include "command.h"
#include "dynarray.h"

/*--------------------------------------------------------------------*/

/* Accept a dynamic array of oTokens and return a Command object,
   unless the dynamic array conatians errors. In the case of errors,
   return NULL. */

Command_T SynAnalyze_analyze(DynArray_T oTokens);

/*--------------------------------------------------------------------*/

#endif
