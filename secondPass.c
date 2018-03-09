#include "secondPass.h"

codePtr codeListHead = NULL;
codePtr codeListtail = NULL;


bool secondPass(char* fileName)
{
	bool is_label, is_data_command, endFile, is_entry, is_extern, extFlag, entFlag;
	FILE* sourceFileHandle;
	FILE* objFile, entFile, extFile;
	int ICounter;

	extern IC , DC ;
	extern symbolListHead, symbolListTail, dataListHead, dataListTail

	char line[MAX_LINE_LENGTH + 1] = {0};
	is_label = is_data_command = endFile = is_entry = is_extern = extFlag = entFlag = FALSE;
	ICounter = 0;

	sourceFileHandle = fopen(filename, "r");

	if (sourceFileHandle == NULL)
	{
		printError("Couldn't open file", filename);
		return;
	}

	createFile(fileName, objFile, ".ob");

	while (!(endFile = readLine(sourceFileHandle, line)))
	{
		ICounter++;
		if(isEmptySentence(line) || isComment(line))
		{
			continue;
		}
		is_label = isLabel(line);
		is_data_command = isDataCommand(line);
		is_extern = isExtern(line);
		is_entry = isEntry(line);
		if(is_extern)
		{
			if(!extFlag)
			{
				extFlag = TRUE;
				if(!(createFile(filename, extFile, ".ext")))
				{
					return FALSE;
				}
			}
			printToExternFile(line);
			continue;
		}
		if (is_entry)
		{
			printToEntryFile(line);
			continue;
		}
		if(is_label)
		{
			if(is_data_command)
			{
				continue;
			}
			else/*command*/
			{
				encoddingCommand(line);
			}
		}

	}

}

void printToExternFile(FILE* extFile ,char* line)
{

}

void printToEntryFile(FILE* entFile, char* line)
{
	int i;
	char *temp = trimStr(line);
	symbolPtr search = symbolListHead;
	bool toWeird;
	char res[5]={0};/**/
	temp = (char *)(temp + ENTRY_LENGTH);
	i = 0;
	toWeird = TRUE;
	while(isspace(*temp))
		temp++;
	if(!isLabelDefined(headOfSymbolsList,temp))
	{
		printWarning("entry a label that not defined");
		return;
	}
	else
	{
		while(search)
		{
			if(strcmp(search->name,temp)==0)
			{
				if(toWeird = decimalToWierd(search->address, res))
				{
					fprintf(entFile, "%s	%s\n", temp, res);
				}
				break;
			}
			search++;
		}
	}
}

void createFile(char* fileName, FILE* dest, char* end)
{
	char* name = strtok(fileName,".");

}

void encoddingCommand(char* line)
{

}
