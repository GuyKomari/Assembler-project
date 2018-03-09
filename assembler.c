#include "assembler.h"


void runAssembler(char* fileName)
{
	bool flag;
	FILE* fp;
	flag = TRUE;
	if (isFileExists(fileName) == false)
	{
		printError(OPEN_INPUT_FILE_ERROR, filename);
		return;
	}
	flag = firstpass(fileName);
	if(flag)/*no errors found in the first pass*/
	{
		flag = secondPass(fileName);
		if(!flag)/*error found in the second pass*/
		{
			printError(ERRORS_IN_SECOND_PASS,filename);
			return;
		}
	}
	else/*errors found in the first pass*/
	{
		printError(ERRORS_IN_FIRST_PASS,filename);
		return;
	}
}