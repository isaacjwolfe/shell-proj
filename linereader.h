/*--------------------------------------------------------------------*/
/* linereader.h                                                       */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#ifndef LINEREADER_INCLUDED
#define LINEREADER_INCLUDED

#include <stdio.h>

/* If no lines remain in psFile, then return NULL. Otherwise read a line
   of psFile and return it as a string. The string does not contain a
   terminating newline character. The caller owns the string. */

char *LineReader_read(FILE *psFile);

#endif
