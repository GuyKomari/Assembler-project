#include "firstPass.h"

/*variables declaration*/
symbolPtr symbolListHead = NULL;
symbolPtr symbolListTail = NULL;
dataPtr dataListHead = NULL;
dataPtr dataListTail = NULL;
long IC = IC_START; /* = 100 */
long DC = DC_START;


/*
Description: the main function of the first pass.
parses from the input file the commands and their sizes and updates IC.
parses from the input file the labels, entries, and externals, and updates DC.
*/
bool firstpass(char* filename)
{
	int i;
	bool is_label, is_data_command, endFile, is_entry, is_extern, symbolFlag, noErrorsFlag, defined_label;
	FILE* sourceFileHandle;

	char line[MAX_LINE_LENGTH + 1] = { 0 };
	char labelName[MAX_LINE_LENGTH + 1] = { 0 };
	char data[MAX_LINE_LENGTH + 1] = { 0 };
	is_label = is_data_command = endFile = is_entry = is_extern = symbolFlag = defined_label = FALSE;
	noErrorsFlag = TRUE;
	/*open the input file*/
	sourceFileHandle = fopen(filename, "r");

	if (sourceFileHandle == NULL)
	{
		printError(OPEN_FILE_ERROR);
		return FALSE;
	}
	while ((endFile = readLine(sourceFileHandle, line)))/*parse every line in the file*/
	{
		symbolFlag = FALSE;
		if (isComment(line) || (is_entry = isEntry(line)) || isEmptySentence(line))
		{
			continue;
		}
		is_extern = isExtern(line);
		is_label = isLabel(line, labelName);/*checks if there is a label definition -> LABEL: (more data). if true, then stores the label in labelName*/
		if (is_label)/*case - there is a label definition*/
		{
			symbolFlag = TRUE;/* "turn on" the symbol flag */
		}
		is_data_command = getSymbol(line, data);/* if contains a data command such as : .string/.data/.struct */
		if (is_data_command)/*case - there is a data definition*/
		{
			if (symbolFlag)/*case - the data definition are inside a label-> LABEL: .string "abcd" */
			{
				defined_label = isLabelDefined(&symbolListHead, labelName);
				noErrorsFlag &= !defined_label; /*if label already defined then there is an error*/
				if (!defined_label)
				{
					/*addToSymbolsList (head, tail, label name, address, isExternal, isCommand, isData, isEntry)*/
					noErrorsFlag &= addToSymbolsList(&symbolListHead, &symbolListTail, labelName, DC, FALSE, FALSE, TRUE, FALSE);
				}
				noErrorsFlag &= ParseData(&dataListHead, &dataListTail, line);/*TODO: phrase 7 - parse the data and adds it to the data list - return true if didnt find errors in the data declaration*/
			}
		}
		else/*case - there is an entry or extern declaration or command declaration with or without a label*/
		{
			if (is_extern || is_entry)/*case - entry or extern*/
			{
				if (is_extern)
				{
					noErrorsFlag &= externLabels(line);/*TODO: phrase 9 - .extern LABEL1...*/
				}
			}
			else /* case - command declaration */
			{
				if (symbolFlag)/* case command inside a label*/
				{
					defined_label = isLabelDefined(&symbolListHead, labelName);
					noErrorsFlag &= !defined_label; /*if label already defined then there is an error*/
				}
				noErrorsFlag &= parseCommand(line);/*counter lines for the code(IC) and finds errors
												   does not encoding the commands, we do it in the
												   second pass*/
			}
		}
		for (i = 0; i < MAX_LINE_LENGTH + 1; i++)
		{
			line[i] = 0;
			labelName[i] = 0;
			data[i] = 0;
		}
	}
	if (!noErrorsFlag)
	{
		return FALSE;
	}
	printf("%s\n", "Firstpass");
	updateDataSymbols(&symbolListHead, IC);
	printSymbolsList(&symbolListHead);
	printDataList(&dataListHead);
	fclose(sourceFileHandle);
	return TRUE;
}


/*
Description: parses the data and inserts it to the data list, and updates DC.
*/
bool ParseData(dataPtr *dataListHead, dataPtr *dataListTail, char *data)
{
	int i, dataLength, strLength, numRequiredBytes;
	char *token, *temp, *dataTempBuffer;
	char stringBuffer[MAX_LINE_LENGTH + 1] = { 0 };
	char labelName[MAX_LINE_LENGTH + 1] = { 0 }, symbolType[MAX_LINE_LENGTH + 1] = { 0 };
	char *stringSymbol = ".string", *dataSymbol = ".data", *structSymbol = ".struct";
	const int stringSymbolLength = 7, dataSymbolLength = 5, structSymbolLength = 7;
	i = dataLength = strLength = numRequiredBytes = 0;
	dataLength = strlen(data);
	dataTempBuffer = (char*)malloc(dataLength + 1);
	if (!dataTempBuffer)
	{
		printError(ALLOCATE_MEMORY_ERROR);
		return FALSE;
	}
	strncpy(dataTempBuffer, data, dataLength + 1);
	if (isLabel(dataTempBuffer, labelName) == FALSE)
	{
		free(dataTempBuffer);
		return FALSE;
	}
	if (getSymbol(dataTempBuffer, symbolType) == FALSE)
	{
		free(dataTempBuffer);
		return FALSE;
	}
	temp = dataTempBuffer;
	temp += strlen(labelName) + 1;
	if (!isspace(*temp))
	{
		free(dataTempBuffer);
		printError(MISSING_SPACE_AFTER_COLON);
		return FALSE;
	}
	temp = trimLeftStr(temp);
	if (strncmp(symbolType, stringSymbol, stringSymbolLength) == 0)
	{
		temp += stringSymbolLength;
		if (!isspace(*temp))
		{
			free(dataTempBuffer);
			printError(MISSING_SPACE_AFTER_STRING_DECLARATION);
			return FALSE;
		}
		temp = trimLeftStr(temp);
		token = strtok(dataTempBuffer, "\"");
		while (token != NULL)
		{
			if (i == 1)
			{
				strncpy(stringBuffer, token, strlen(token) - 1);
			}
			else if (i > 2)
			{
				free(dataTempBuffer);
				return FALSE;
			}
			i++;
			token = strtok(NULL, dataTempBuffer);
		}
		if (i == 2)
		{
			DC = addStringToData(dataListHead, dataListTail, stringBuffer, DC);
			free(dataTempBuffer);
			return TRUE;
		}
		else
		{
			free(dataTempBuffer);
			return FALSE;
		}
	}
	else if (strncmp(symbolType, dataSymbol, dataSymbolLength) == 0)
	{
		temp += dataSymbolLength;
		if (!isspace(*temp))
		{
			free(dataTempBuffer);
			printError(MISSING_SPACE_AFTER_DATA_DECLARATION);
			return FALSE;
		}
		temp = trimLeftStr(temp);
		token = strtok(temp, ",");
		while (token != NULL)
		{
			i = atoi(trimStr(token));
			if (i != 0 || (strcmp(token, "0") == 0))
			{
				addNumberToDataList(dataListHead, dataListTail, DC, i);
				DC++;
			}
			else
			{
				printError(INVALID_NUMBER_IN_DATA_LABEL);
			}
			token = strtok(NULL, ",");
		}

	}
	else if (strncmp(symbolType, structSymbol, structSymbolLength) == 0)
	{
		temp += structSymbolLength;
		if (!isspace(*temp))
		{
			free(dataTempBuffer);
			printError(MISSING_SPACE_AFTER_STRUCT_DECLARATION);
			return FALSE;
		}
		temp = trimLeftStr(temp);
		token = strtok(temp, ",");
		while (token != NULL)
		{
			token = trimLeftStr(token);
			i = atoi(token);
			if (i != 0 || (strcmp(token, "0") == 0))
			{
				addNumberToDataList(dataListHead, dataListTail, DC, i);
				DC++;
			}
			else
			{
				strLength = strlen(token);
				if (strLength > 2 && token[0] == '"' && token[strLength - 1] == '"')
				{
					strncpy(stringBuffer, token + 1, strLength - 2);
					addStringToData(dataListHead, dataListTail, stringBuffer, DC);
					DC += strLength - 1;
				}
				else if (strLength == 3 && token[0] == '\'' && token[2] == '\'')
				{
					addToDataList(dataListHead, dataListTail, DC, character, (int)(token[1]));
					DC++;
				}
				else
				{
					printError(STRUCT_DEFINITION_ERROR);
				}
			}
			token = strtok(NULL, ",");
		}
	}
	else
	{
		free(dataTempBuffer);
		return FALSE;
	}
	free(dataTempBuffer);
	return TRUE;
}


/*
Description: gets a line that contains an extern label (.extern) and adds it to the symbols list.
*/
bool externLabels(char *line)
{
	if (isExtern(line))
	{
		char* temp;
		int extLength = EXTERN_LENGTH;		/*".extern" Length is 7*/
		temp = trimStr(line);
		temp += extLength;
		temp = trimStr(temp);
		return addToSymbolsList(&symbolListHead, &symbolListTail, temp, 0, TRUE, FALSE, FALSE, FALSE);
	}
	else
		return FALSE;
}


/*
gets a line with a command (for example "mov r1, r2"), and if it has a label, adds it to the symbols list.
afterwards, calculates the size of the command in memory, and increments IC with the command size.
*/
bool parseCommand(char *line)/*with or without label*/
{
	int sizeOfCommand = 0;
	char *temp;
	char labelName[MAX_LINE_LENGTH + 1] = { 0 };
	temp = line;
	if (isLabel(line, labelName) == TRUE)
	{
		temp += strlen(labelName) + 1;
		addToSymbolsList(&symbolListHead, &symbolListTail, labelName, IC, FALSE, TRUE, FALSE, FALSE);
	}

	temp = trimStr(temp);
	sizeOfCommand = getCommandSize(temp);
	IC += sizeOfCommand;
	return (sizeOfCommand != 0) ? TRUE : FALSE;
}
