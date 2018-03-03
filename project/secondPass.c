#include "globalFunctions.h"


bool secondPass(char* fileName)
{
	bool is_label, is_data_command, endFile, is_entry, is_extern;
	FILE* sourceFileHandle;
	int IC; 
	char line[MAX_LINE_LENGTH + 1] = {0};
	bool endFile = FALSE;
	IC = 0;


	sourceFileHandle = fopen(filename, "r");

	if (sourceFileHandle == NULL)
	{
		printError("Couldn't open file", filename);
		return;
	}

	while (!(endFile = readLine(sourceFileHandle, line)))
	{
		is_label = isLabel(line);
		is_data_command = isDataCommand(line);
		is_extern = isExtern(line);
		is_entry = isEntry(line);
		if (is_entry && !(isExtern || isDataCommand))
		{
			
		}
	}

}