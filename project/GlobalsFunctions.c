#include <stdio.h>
#include "globalVariables.h"
#include "GlobalFunctions.h"


bool isLabel(char* token)
{

}

bool isDataCommand(char* token)
{
	
}

bool isFileExists(char* fileName)
{
	FILE* fHandle;
	fHandle = fopen(fileName, "r");
	if (fHandle == NULL)
		return false;
	else
		fclose(fHandle);
	return true;
}