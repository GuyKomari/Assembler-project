#include "main.h"

int main(int argc, char** argv)
{
	int i = 0, fileNameLength;
	char* fileName;
	if (argc < 2)
	{
		printError(MISSING_INPUT_FILE);
	}
	for (i = 1; i < argc; i++)
	{
		fileNameLength = strlen(argv[i]);
		fileName = (char*)malloc(fileNameLength + 1);
		if (fileName == NULL)
		{
			printError(FILE_NAME_MEM_ALLOCATION_FAILURE);
			continue;
		}
		strncpy(fileName, argv[i], fileNameLength);
		fileName[fileNameLength] = '\0';
		runAssembler(fileName);
		free(fileName);
	}
	return 0;
}
