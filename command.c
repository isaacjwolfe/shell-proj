/*--------------------------------------------------------------------*/
/* command.c                                                          */
/* Author: Isaac Wolfe                                                */
/*--------------------------------------------------------------------*/

#include "command.h"
#include "ish.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------------*/

/* A Command is an object that contains pcName, the arguments 
   for the action (oArguments), and pcStdIn and pcStdOut. */

struct Command
{
	/* The name of the command/action. */
	char* pcName;

	/* StdIn file name */
	char* pcStdIn;

	/* StdOut file name */
	char* pcStdOut;

	/* Dynarray_T object of the potential arguments. */
	DynArray_T oArguments;
};

/*--------------------------------------------------------------------*/

/* Make a new command object with the name/action pcName, with the 
   arguments oArguments, and with standard input and output designated
   by pcStdIn and pcStdIn */

Command_T Command_new(char* pcName, char* pcStdIn, char* pcStdOut, 
	DynArray_T oArguments)
{
	Command_T oCommand;
	size_t ulNameLength;
	size_t ulStdInLength;
	size_t ulStdOutLength;
	size_t ulIndex;
	size_t ulArgumentsLength;
	size_t ulCurrArgumnetLength;
	char* currArgument;


	assert(pcName != NULL);

	/* Allocate memory for the oCommand object itself. */
	oCommand = (struct Command*)malloc(sizeof(struct Command));

	/* Retuen error if insufficient memory. */
	if (oCommand == NULL)
		{perror(pcPgmName); exit(EXIT_FAILURE);}

	/* Allocate memory for pcName contained within oCommand.
	   We have already established that pcName is not NULL.
	   Exit error if insufficient memory. */
	ulNameLength = strlen(pcName) + 1;
	oCommand->pcName = (char*)malloc(sizeof(char) * ulNameLength);
	if (oCommand->pcName == NULL)
		{perror(pcPgmName); exit(EXIT_FAILURE);}
	strcpy(oCommand->pcName, pcName);

	/* Copy over pcStdIn. */
	/* Make oCommand's pcStdIn NULL of NULL is passed in... */
	if (pcStdIn == NULL)
	{
		oCommand->pcStdIn = NULL;
	}
	/* ...otherwise, allocate memory and copy pcStdIn.
	   Exit error if insufficient memory. */
	else 
	{
		ulStdInLength = strlen(pcStdIn) + 1;
		oCommand->pcStdIn = 
		(char*)malloc(sizeof(char) * ulStdInLength);
		if (oCommand->pcStdIn == NULL)
			{perror(pcPgmName); exit(EXIT_FAILURE);}
		strcpy(oCommand->pcStdIn, pcStdIn);
	}

	/* Copy over pcStdOut. */
	/* Make oCommand's pcStdOut NULL of NULL is passed in... */
	if (pcStdOut == NULL)
	{
		oCommand->pcStdOut = NULL;
	}
	/* ...otherwise, allocate memory and copy pcStdIn.
	   Exit error if insufficient memory. */
	else 
	{
		ulStdOutLength = strlen(pcStdOut) + 1;
		oCommand->pcStdOut = 
		(char*)malloc(sizeof(char) * ulStdOutLength);
		if (oCommand->pcStdOut == NULL)
			{perror(pcPgmName); exit(EXIT_FAILURE);}
		strcpy(oCommand->pcStdOut, pcStdOut);
	}

	/* Copy over oArguments. */
	/* Make oCommand's oArguments NULL of NULL is passed in... */
	if (oArguments == NULL)
	{
		oCommand->oArguments = NULL;
	}
	/* ...otherwise, allocate memory and copy over each element
	   in the dynamic array. */
	else 
	{
		/* Memory allocation for oCommands's oArguments. */
		ulArgumentsLength = DynArray_getLength(oArguments);
		oCommand->oArguments = DynArray_new(ulArgumentsLength);

		/* Memory allocation for each element in oArguments. */
		for (ulIndex = 0; 
			 ulIndex < DynArray_getLength(oArguments);
			 ulIndex++)
		{	
			ulCurrArgumnetLength = 
			strlen(DynArray_get(oArguments, ulIndex)) + 1;
			currArgument = 
			(char*)malloc(sizeof(char) * ulCurrArgumnetLength);
			strcpy(currArgument, DynArray_get(oArguments, ulIndex));
			(void)DynArray_set(oCommand->oArguments, ulIndex, currArgument);	
		}	
	}

	/* if (oArguments != NULL) DynArray_free(oArguments); */
	return oCommand;
}

/*--------------------------------------------------------------------*/

/* Print a command, oCommand. */

void Command_write(Command_T oCommand)
{
	size_t ulIndex;
	assert(oCommand != NULL);

	/* Print name of the commmand. */
	printf("Command name: %s\n", oCommand->pcName);

	/* Print oArguemnts if not NULL. */
	if (oCommand->oArguments != NULL)
	{
		for (ulIndex = 0; 
			ulIndex < DynArray_getLength(oCommand->oArguments);
			ulIndex++)
		{
			printf("Command arg: %s\n", 
				(char*)DynArray_get(oCommand->oArguments, ulIndex));
		}
	}

	/* Print StdIn of command if not NULL. */
	if (oCommand->pcStdIn != NULL)
		printf("Command stdin: %s\n", oCommand->pcStdIn);

	/* Print StdOut of command if not NULL. */
	if (oCommand->pcStdOut != NULL)
		printf("Command stdout: %s\n", oCommand->pcStdOut);

}

/*--------------------------------------------------------------------*/

/* Free oCommand. */

void Command_free(Command_T oCommand)
{
	size_t ulIndex;

	/* Free pcName. */
	free(oCommand->pcName);

	/* Free oArguments. */
	if (oCommand->oArguments != NULL)
	{
		for (ulIndex = 0; 
			ulIndex < DynArray_getLength(oCommand->oArguments);
			ulIndex++)
		{
			free(DynArray_get(oCommand->oArguments, ulIndex));
		}
		DynArray_free(oCommand->oArguments);
	}

	/* Free pcStdIn if not NULL. */
	if (oCommand->pcStdIn != NULL) 
	{
		free(oCommand->pcStdIn);
	}

	/* Free pcStdOut if not NULL. */
	if (oCommand->pcStdOut != NULL) 
	{
		free(oCommand->pcStdOut);
	}

	/* Free oCommand itself. */
	free(oCommand);
}

/*--------------------------------------------------------------------*/

/* Return pcStdIn of oCommand. */

char* Command_getStdIn(Command_T oCommand)
{
	assert(oCommand != NULL);
	return oCommand->pcStdIn;
}

/*--------------------------------------------------------------------*/

/* Return pcStdOut of oCommand. */

char* Command_getStdOut(Command_T oCommand)
{
	assert(oCommand != NULL);

	return oCommand->pcStdOut;
}

/*--------------------------------------------------------------------*/

/* Return name of oCommand. */

char* Command_getName(Command_T oCommand)
{
	assert(oCommand != NULL);
	return oCommand->pcName;
}

/*--------------------------------------------------------------------*/

/* Return oArguments of oCommand. */

DynArray_T Command_getArguments(Command_T oCommand)
{
	assert(oCommand != NULL);
	return oCommand->oArguments;
}

/*--------------------------------------------------------------------*/

/* Return char** array of oCommand's Command->oArguments. */

char** Command_getArgsArray(Command_T oCommand)
{
	char** ppcArguments;
	size_t index;
	size_t ulLength;

	assert(oCommand != NULL);

	/* If oArguments is empyt, make the uLength zero. */
	if (oCommand->oArguments == NULL) ulLength = 0;
	else ulLength = DynArray_getLength(oCommand->oArguments);

	/* Use oArgument's length to malloc memory. Create two additional 
	   char* of memory to store the command name and the NULL 
	   character. */
	ppcArguments = 
	(char**)malloc(sizeof(char*) * 
		(ulLength + 2));

	/* Return failure if no memory. */
	if (ppcArguments == NULL)
		{perror(pcPgmName); exit(EXIT_FAILURE);}

	/* Make firt index name, fill middle with args (if any) and
	   make last NULL. */
	ppcArguments[0] = oCommand->pcName;
	for (index = 1; 
		index < ulLength + 1;
		index++)
	{
		ppcArguments[index] = DynArray_get(oCommand->oArguments, index - 1);
	}
	ppcArguments[index] = NULL;

	return ppcArguments;
}

