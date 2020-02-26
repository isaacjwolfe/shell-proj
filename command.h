/*--------------------------------------------------------------------*/
/* command.h                                                          */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#ifndef COMMAND_INCLUDED
#define COMMAND_INCLUDED

#include "dynarray.h"

/*--------------------------------------------------------------------*/

/* A command is an object that contains an action, the arguments for 
   the action, and input and output destinations for such action. */

typedef struct Command* Command_T;

/*--------------------------------------------------------------------*/

/* Returns a new command object with the name/action pcName, with the 
   arguments oArguments, and with standard input and output designated
   by pcStdIn and pcStdOut */

Command_T Command_new(char* pcName, char* pcStdIn, char* pcStdOut, 
	DynArray_T oArguments);


/*--------------------------------------------------------------------*/

/* Print a command, oCommand. */

void Command_write(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Free oCommand. */

void Command_free(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Return stdin string of oCommand. */

char* Command_getStdIn(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Return stdout string of oCommand. */

char* Command_getStdOut(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Return name of oCommand. */

char* Command_getName(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Return oArguments of oCommand. */

DynArray_T Command_getArguments(Command_T oCommand);

/*--------------------------------------------------------------------*/

/* Return char** array of oCommand's Command->oArguments. */

char** Command_getArgsArray(Command_T oCommand);

#endif