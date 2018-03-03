#include "globalVariables.h"


/*
extern IC = 0;

extern DC = 0;
*/

bool symbolFlag = FALSE;
bool errorFlag = FALSE;



/*
TODO:
1. Implement "isEntry" and "isExternal" functions
2. add a list node to handle the table lists
*/
bool firstpass(char* filename)
{
	bool is_label, is_data_command, endFile, is_entry, is_extern;
	FILE* sourceFileHandle;
	long IC, DC;
	char line[MAX_LINE_LENGTH + 1] = {0};
	/* char labelName[MAX_LINE_LENGTH + 1] = {0};*/
	IC = DC = 0;

	sourceFileHandle = fopen(filename, "r");

	if (sourceFileHandle == NULL)
	{
		printError("Couldn't open file", filename);
		return;
	}

	while (!(endFile = readLine(sourceFileHandle, line)))
	{
		is_label = isLabel(line);
		if (is_label)
		{
			/*getLabelNameFromLine(line, labelName);*/
			symbolFlag = true;
		}
		else
		{
			is_data_command = isDataCommand(line);/*.string , .data , .struct */
			if (is_data_command)
			{
				if (symbolFlag)
				{
					errorFlag = addToSymbolsList(line, DC, FALSE, FALSE, TRUE);
					DC += addToDataList(line);
				}
			}
			else
			{
				is_extern = isExtern(line);
				is_entry = isEntry(line);
				if (is_extern || is_entry)
				{
					if (is_extern)
					{
						errorFlag = addToSymbolsList(line, 0, TRUE, FALSE, FALSE,FALSE);
					}
				}
				else
				{
					if (symbolFlag)
					{
						errorFlag = addToSymbolsList(line, IC, FALSE, TRUE, FALSE);
					}
				}
			}
		}
	}
	if (errorFlag)
	{
		return FALSE;
	}
	updateDataSymbols(IC);
}


bool readLine(FILE* fp, char* line)
{

}