#include "firstPass.h"

/*global variables declaration*/

symbolPtr symbolListHead = NULL;
symbolPtr symbolListTail = NULL;
dataPtr dataListHead = NULL;
dataPtr dataListTail = NULL;
long IC = IC_START;/* = 100 */
long DC = DC_START;
int lineCounter;
/*
TODO:
implements the functions which parsing a line:
ParseData , isLabel , externLabels , parseCommand;
*/

/*does the first pass - returns TRUE if no errors were found*/
bool firstpass(char* filename)
{
	bool is_label, is_data_command, endFile, is_extern, symbolFlag, errorFlag;
	FILE* sourceFileHandle;

	char line[MAX_LINE_LENGTH + 1] = {0};
	char labelName[MAX_LINE_LENGTH + 1] = {0};
	char data[MAX_LINE_LENGTH + 1] = {0};
	is_label = is_data_command = endFile = is_extern = symbolFlag = FALSE;
	errorFlag = TRUE;

	sourceFileHandle = fopen(filename, "r");/*open the input file*/
	if (sourceFileHandle == NULL)
	{
		printError(OPEN_FILE_ERROR, filename);
		return FALSE;
	}
	/*else*/
	while (!(endFile = readLine(sourceFileHandle, line)))/*parse every line in the file*/
	{
		lineCounter++;
		if(isEmptySentence(line) || isComment(line) || isEntry(line))
		{
			continue;
		}
		is_label = isLabel(line,labelName);/*checks if there is a label definition -> LABEL: (more data).
 											if true, then stores the label in labelName*/
		if (is_label)/*case - there is a label definition*/
		{
			symbolFlag = TRUE;/* "turn on" the symbol flag */
			is_data_command = isDataCommand(line,data);/* if contains a data command such as : .string/.data/.struct */

		}
		if (is_data_command && symbolFlag)/*case - there is a data definition*/
		/*case - the data definition are inside a label-> LABEL: .string "abcd" */
		{
			errorFlag&= isLabelDefined(symbolListHead,labelName);/*if label already defined then there is an error*/
			/*addToSymbolsList (head, tail, label name, address, isExternal, isCommand, isData, isEntry)*/
			errorFlag&= addToSymbolsList(symbolListHead, symbolListTail, labelName, DC, FALSE, FALSE, TRUE, FALSE);
			errorFlag&= ParseData(dataListHead, dataListTail, data);/*TODO: phrase 7 - parse the data and adds it to the data list - return true if didnt find errors in the data declaration*/
		}
		else/*case - there is an entry or extern declaration or command declaration with or without a label*/
		{
			is_extern = isExtern(line);/*there is an extern declaration - .extern LABEL*/
			if (is_extern)/*case - entry or extern*/
			{
				if (is_extern)
				{
					errorFlag&= externLabel(line);/*TODO: phrase 9 - .extern LABEL...*/
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
	if (!errorFlag)
	{
		return FALSE;
	}
	updateDataSymbols(IC);
	return TRUE;
}


bool readLine(FILE* fp, char* line)
{
	if(feof(fp))
		return FALSE;
	fgets(line, MAX_LINE_LENGTH, fp);
	return TRUE;
}

/*
parse the data and insert to the data list
update the 
*/
bool ParseData(dataPtr dataListHead, dataPtr dataListTail, char *data)
{
	int i, length;
	char *token;
	char stringBuffer[MAX_LINE_LENGTH + 1] = {0};
	char *stringSymbol = ".string", *dataSymbol = ".data", *structSymbol = ".struct";
	char labelName[MAX_LINE_LENGTH + 1] = {0}, symbolType[MAX_LINE_LENGTH + 1] = {0};
	if (getLabel(data, labelName) == FALSE)
		return FALSE;
	if (getSymbol(data, symbolType) == FALSE)
		return FALSE;
	length = strlen(data);
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

	}
	else if (strncmp(symbolType, structSymbol, strlen(structSymbol)) == 0)
	{
		
	}
	else
		return FALSE;




	/*while(data) --example1 - STR: .string "abcdef"
				 example2 - LENGTH: .data 6,-9,15
	{
		/*
		looking for errors ->
		DC++ -> 
		insert to data list 
		(addToDataList(dataPtr *head ,dataPtr *tail ,int dc, int dType, int ascii))
		dc - DC , dType - enum {positiveNumber, negativeNumber, character, eof}
		
	}*/
}
/*phrase 9*/
bool externLabel(char *line)
{
	char *temp;
	bool addedToList;
	temp = trimStr(line);
	temp = (char *)(temp + EXTERN_LENGTH);
	temp = trimLeftStr(temp);
	addedToList = addToSymbolsList(symbolListHead ,symbolListHeadTail ,temp, 0, TRUE , FALSE , FALSE, FALSE);
	return addedToList;
}

bool parseCommand(char *line)/*with or without label*/
{
	/*
	looking for errors
	and update IC
	*/
}