#include "firstPass.h"

/*variables declaration*/
/*we use them also in the second pass so */
symbolPtr symbolListHead = NULL;
symbolPtr symbolListTail = NULL;
dataPtr dataListHead = NULL;
dataPtr dataListTail = NULL;
long IC = IC_START;/* = 100 */
long DC = DC_START;

/*
TODO:
implements the functions which parsing a line:
ParseData , isLabel , externLabels , parseCommand;
*/

/*does the first pass - returns TRUE if didnt find errors*/
bool firstpass(char* filename)
{
	bool is_label, is_data_command, endFile, is_entry, is_extern, symbolFlag, errorFlag;
	FILE* sourceFileHandle;

	char line[MAX_LINE_LENGTH + 1] = {0};
	char labelName[MAX_LINE_LENGTH + 1] = {0};
	char data[MAX_LINE_LENGTH + 1] = {0};
	is_label = is_data_command = endFile = is_entry = is_extern = symbolFlag = errorFlag = FALSE;
	/*open the input file*/
	sourceFileHandle = fopen(filename, "r");

	if (sourceFileHandle == NULL)
	{
		printError(OPEN_FILE_ERROR, filename);
		return FALSE;
	}
	while (!(endFile = readLine(sourceFileHandle, line)))/*parse every line in the file*/
	{
		is_label = isLabel(line,labelName);/*checks if there is a label definition -> LABEL: (more data).
 											if true, then stores the label in labelName*/
		if (is_label)/*case - there is a label definition*/
		{
			symbolFlag = TRUE;/* "turn on" the symbol flag */
		}
		is_data_command = isDataCommand(line,data);/* if contains a data command such as : .string/.data/.struct */
		if (is_data_command)/*case - there is a data definition*/
		{
			if (symbolFlag)/*case - the data definition are inside a label-> LABEL: .string "abcd" */
			{
				errorFlag&= isLabelDefined(symbolListHead,labelName);/*if label already defined then there is an error*/

				/*addToSymbolsList (head, tail, label name, address, isExternal, isCommand, isData, isEntry)*/
				errorFlag&= addToSymbolsList(symbolListHead, symbolListTail, labelName, DC, FALSE, FALSE, TRUE, FALSE);
				errorFlag&= ParseData(dataListHead, dataListTail, data);/*TODO: phrase 7 - parse the data and adds it to the data list - return true if didnt find errors in the data declaration*/
			}
		}
		else/*case - there is an entry or extern declaration or command declaration with or without a label*/
		{
			is_extern = isExtern(line);/*there is an extern declaration - .extern LABEL*/
			is_entry = isEntry(line);/*there is an enntry declaration - .entry - we dont do anything in the first pass*/
			if (is_extern || is_entry)/*case - entry or extern*/
			{
				if (is_extern)
				{
					errorFlag&= externLabels(line);/*TODO: phrase 9 - .extern LABEL1 LABEL2...*/
				}
			}
			else /* case - command declaration */
			{
				if (symbolFlag)/* case command inside a label*/
				{
				errorFlag&= isLabelDefined(symbolListHead,labelName);
				/*addToSymbolsList (head, tail, label name, address, isExternal, isCommand, isData, isEntry)*/
				errorFlag&= addToSymbolsList(symbolListHead, symbolListTail, labelName, IC, FALSE, TRUE, FALSE, FALSE);		
				}
				errorFlag&= parseCommand(line);/*counter lines for the code(IC) and finds errors
												 does not encoding the commands, we do it in the
												 second pass*/
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
	if(feof(fp))
		return FALSE;
	fgets(line, MAX_LINE_LENGTH + 1, fp);
	return TRUE;
}

/*
parse the data and insert to the data list
update the 
*/
bool ParseData(dataPtr *dataListHead, dataPtr *dataListTail, char *data)
{
	int i, dataLength, strLength, numRequiredBytes;
	char *token, *temp;
	char stringBuffer[MAX_LINE_LENGTH + 1] = {0};
	char labelName[MAX_LINE_LENGTH + 1] = { 0 }, symbolType[MAX_LINE_LENGTH + 1] = { 0 };
	char *stringSymbol = ".string", *dataSymbol = ".data", *structSymbol = ".struct";
	
	if (isLabel(data, labelName) == FALSE)
		return FALSE;
	if (getSymbol(data, symbolType) == FALSE)
		return FALSE;
	dataLength = strlen(data);
	if (strncmp(symbolType, stringSymbol, strlen(stringSymbol)) == 0)
	{
		token = strtok(data, "\"");
		while (token != NULL)
		{
			if (i == 1)
			{
				strncpy(stringBuffer, token, strlen(token));
			}
			else if (i > 2)
				return FALSE;
			i++;
			token = strtok(NULL, data);
		}
		if (i == 2)
		{
			DC = addStringToData(dataListHead, dataListTail, stringBuffer, DC);
			return TRUE;
		}
		else
			return FALSE;
	}
	else if (strncmp(symbolType, dataSymbol, strlen(dataSymbol)) == 0)
	{
		temp = data + strlen(dataSymbol);
		temp = trimLeftStr(temp);
		token = strtok(data, ",");
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
				printError("Invalid number in .data label");
			}
			token = strtok(NULL, data);
		}
		
	}
	else if (strncmp(symbolType, structSymbol, strlen(structSymbol)) == 0)
	{
		temp = data + strlen(dataSymbol);
		temp = trimLeftStr(temp);
		token = strtok(data, ",");
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
					DC += strLength + 1;
				}
				else if (strLength == 3 && token[0] == '\'' && token[2] == '\'')
				{
					addToDataList(dataListHead, dataListTail, DC, character, (int)(token[1]));
					DC++;
				}
				else
				{
					printError("Unrecognized Type in struct definition");
				}
			}
			token = strtok(NULL, data);
		}
	}
	else
		return FALSE;
	return 
}
/*phrase 9*/
bool externLabels(char *line)
{
	/*
	for every extern label:
	insert to symbolsList
	using the function addToSymbolsList
	*/
}

bool parseCommand(char *line)/*with or without label*/
{
	/*
	looking for errors
	and update IC
	*/
}