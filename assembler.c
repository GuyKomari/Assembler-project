#include "assembler.h"
#include "GlobalsFunctions.h"
#include "errors.h"

/*
TODO:
1. add the file name as an argument when calling printError (involves re-implementation of printError).
2.  
*/

void runAssembler(char* fileName)
{
	FILE* fp;
	if (isFileExists(fileName) == FALSE)
	{
		printError("File is not exist", fileName);
		return;
	}
	firstpass(fileName);
}