#include "GlobalsFunctions.h"



bool isRegister(char *token)
{
	int i = 0;
	for(i = 0; i < NUM_OF_REGISTERS ; i++)
	{
		if(!(strcmp(token, Registers[i])))
			return TRUE;
	}
	return FALSE;
}

bool isOpcode(char *token)
{
	int i = 0;
	for(i = 0 ; i < NUM_OF_OPCODES ; i++)
	{
		if(!(strcmp(token, opcodes[i].opcodeName)))
			return TRUE;
	}
	return FALSE;

}

bool isLabel(char* src, char* dest)
{
	bool isExternDef, isEntryDef;
	char *temp, *start;
	int len;
	temp = trimStr(src);
	len = 0;
	isExternDef = isEntryDef = FALSE;
	if (isExtern(temp))
	{
		isExternDef = TRUE;
		temp = (char *)(temp + EXTERN_LENGTH);
	}
	else if (isEntry(temp))
	{
		isEntryDef = TRUE;
		temp = (char *)(temp + ENTRY_LENGTH);
	}
	temp = trimLeftStr(temp);
	start = temp;
	if (!(isalpha(*temp)))/*label doesnt start with a letter*/
	{
		printError("The label does not begins with an alpha letter");
		return FALSE;
	}
	while (*temp && !isspace(*temp) && strncmp((char*)(temp), ":", 1) != 0)
	{
		if (*temp && !isalpha(*temp) && !isdigit(*temp))
		{
			printError("The label contains non-alpha or non-digit characters");
			return FALSE;
		}
		temp++;
		len++;
	}
	if (!isExternDef && strncmp((char*)(temp++), ":", 1) != 0)
	{
		printf("A colon is missing");
		return FALSE;
	}
	if (!isExternDef && !isspace(*temp))
	{
		if (ispunct(*temp) || isdigit(*temp) || isalpha(*temp))
		{
			printError("additional character after declaration of a label");
			return FALSE;
		}
		printError("missing a space");
		return FALSE;
	}
	if (dest != NULL)
		strncpy(dest, start, len);
	return TRUE;
}
/*
headOfSymbolsList - head of the symbols list;
token - is a valid label
lineCounter - the code line
*/
bool isLabelDefined(symbolPtr *headOfSymbolsList, char* token)
{
	symbolPtr temp = *headOfSymbolsList;
	while(temp)
	{
		if(!(strcmp(token,(temp)->name)))
		{
			printError("Label is already defined");
			return TRUE;
		}
		temp = (temp)->next;
	}
	return FALSE;
}

/*
TODO:
add errors printing ?
check if the label is not a keyword ?
*/
bool isEntry(char* line)
{
	char *temp = trimStr(line);
	if(strncmp(temp , ".entry", ENTRY_LENGTH))
		return FALSE;
	temp = (char *)(temp + ENTRY_LENGTH);

	if(!isspace(*temp))
		return FALSE;

	while(isspace(*temp))
		temp++;

	if(!(isalpha(*temp)))
		return FALSE;

	while(*temp)
	{
		if(isspace(*temp))
			return FALSE;
		temp++;
	}
	return TRUE;
}

/*
TODO: add the define to global variables class.
add errors printing ?
check if the label is not a keyword ?
*/
#define EXTERN_LENGTH 7
bool isExtern(char* line)
{
	int i;
	char *temp = trimStr(line);
	if(strncmp(temp , ".extern", EXTERN_LENGTH))
		return FALSE;
	temp = (char *)(temp + EXTERN_LENGTH);

	if(!isspace(*temp))
		return FALSE;

	while(isspace(*temp))
		temp++;

	if(!(isalpha(*temp)))
		return FALSE;

	for (i = 0; i < NUM_OF_KEYWORDS; i++)
	{
		if (strcmp(temp, Keywords[i]) == 0)
		{
			printError("A variable name has the same name as a keyword");
			return FALSE;
		}
	}

	while(*temp)
	{
		if(isspace(*temp))
			return FALSE;
		temp++;
	}
	return TRUE;
}


bool isValidLabel(char* token)
{
	int i = 0, length;
	char* trimmed;
	trimmed = trimStr(token);
	length = strlen(trimmed);
	if (length == 0 || length > 30)
		return FALSE;
	if (!isalpha(trimmed[i++]))
		return FALSE;
	for (; i < length; i++)
	{
		if (!(isalpha(trimmed[i]) || isdigit(trimmed[i])))
			return FALSE;
	}
	return TRUE;
}

bool isNumOperand(char* token)
{
	int  i = 0;
	char* trimmed;
	trimmed = trimStr(token);
	if (trimmed[i++] != '#')
		return FALSE;
	if (atoi(trimmed + i) == 0)
	{
		if (trimmed[i] == '+' || trimmed[i] == '-')
			i++;
		if (strcmp(trimmed + 1, "0") != 0)
			return FALSE;
	}
	return TRUE;
}


bool isEmptySentence(char* token)
{
	while (*token != '\0') 
	{
		if (!isspace((unsigned char)*token))
      		return FALSE;
    	token++;
	}
  return TRUE;
}

bool isComment(char* token)
{
	char* temp = trimLeftStr(token);
	if(temp[0]==';')
		return TRUE;
	return FALSE;
}


char *trimLeftStr(char *str)
{
	while(isspace(*str))
		str++;
    return str;
}

char *trimRightStr(char *str)
{
	int length = strlen(str);
	if (length == 0)
		return str;
    char* back = str + length;
	
    while(isspace(*--back));
    *(back+1) = '\0';
    return str;
}

char* trimStr(char* str)
{
	char* res = trimRightStr(trimLeftStr(str));
	return res;
}


bool isFileExists(char* fileName)
{
	FILE* fHandle;
	fHandle = fopen(fileName, "r");
	if (fHandle == NULL)
		return FALSE;
	else
		fclose(fHandle);
	return TRUE;
}


bool getSymbol(char* data, char* dest)
{
	char *symbols[3] = { ".string", ".data", ".struct" };
	bool afterLabel = FALSE, detectedSymbol = FALSE;
	int i, j, length, correctSymbol = -1;
	length = strlen(data);
	for (i = 0; i < length; i++)
	{
		if (afterLabel)
		{
			if (!isspace(data[i]))
			{
				for (j = 0; j < 3; j++)
				{
					if (strncmp(data + i, symbols[j], strlen(symbols[j])) == 0)
					{
						if (detectedSymbol)
							return FALSE;
						else
						{
							detectedSymbol = TRUE;
							correctSymbol = j;
							i += strlen(symbols[j]) - 1;
							continue;
						}
					}
				}
			}
		}
		else
		{
			if (data[i] == ':')
			{
				if (i != 0)
					afterLabel = TRUE;
				else
					return FALSE;
			}
			else if (!isalpha(data[i]))
				return FALSE;

			else
				continue;
		}
	}

	if (correctSymbol != -1)
	{
		if (dest != NULL)
			strncpy(dest, symbols[correctSymbol], strlen(symbols[correctSymbol]));
		return TRUE;
	}
	return FALSE;
}

/*why returns bool and not void? #GIL*/
bool addNumberToDataList(dataPtr *head, dataPtr *tail, int dc, int num)
{
	if (num >= 0)
	{
		addToDataList(head, tail, dc, positiveNumber, num);
	}
	else
	{
		addToDataList(head, tail, dc, negativeNumber, num);
	}
	return TRUE;
}


int addStringToData(dataPtr *dataListHead, dataPtr *dataListTail, char *str, long dc)
{
	int i, length;
	length = strlen(str);
	for (i = 0; i < length; i++)
	{
		addToDataList(dataListHead, dataListTail, dc, character, (int)(str[i]));
		dc++;
	}
	addToDataList(dataListHead, dataListTail, dc, character, '\0');
	dc++;
	return dc;
}


AddressingMode getOperandAddressing(char* token)
{
	if (isNumOperand(token))
		return IMMEDIATE;
	else if (isRegister(token))
		return DIRECT_REGISTER;
	else if (isStructWithDotOperand(token))
		return STRUCT_ACCESS;
	else if (isValidLabel(token))
		return DIRECT_MEMORY;
	else
		return -1;
}




int getCommandSize(char* command)
{
	int i, sizeOfCommand = 0;
	AddressingMode srcOperandAddressing, destOperandAddressing;
	opcodeStructure *opcode;
	char* token, *delim = " \t,";

	token = strtok(command, delim);
	for (i = 0; i < NUM_OF_OPCODES; i++)
	{
		if (token == NULL)
			continue;
		if (strcmp(token, opcodes[i].opcodeName) == 0)
		{
			opcode = &(opcodes[i]);

			switch (opcode->group)
			{
				/* 2 Operands Opcode */
			case FIRST_GROUP:
			{
				token = strtok(NULL, delim);
				if (token == NULL)
				{
					printError("Too few operands in the command");
					return FALSE;
				}

				if (isKeyword(token))
				{
					printError("An operand name cannot be the same as a keyword name");
					return FALSE;
				}

				if ((srcOperandAddressing = getOperandAddressing(token)) == -1)
				{
					printError("Source operand is invalid");
					return 0;
				}

				token = strtok(NULL, delim);
				if (token == NULL)
				{
					printError("Too few operands in the command");
					return FALSE;
				}

				if (isKeyword(token))
				{
					printError("An operand name cannot be the same as a keyword name");
					return FALSE;
				}

				if ((destOperandAddressing = getOperandAddressing(token)) == -1)
				{
					printError("Destination operand is invalid");
					return 0;
				}

				if (strcmp(opcode->opcodeName, "lea") == 0 && (srcOperandAddressing == IMMEDIATE || srcOperandAddressing == DIRECT_REGISTER))
				{
					printError("\"lea\" opcode cannot have an immediate or immediate register addressing mode");
					return 0;
				}

				if (destOperandAddressing == IMMEDIATE)
				{
					if (strcmp(opcode->opcodeName, "cmp") != 0)
					{
						printError("Only \"cmp\" opcode can use immediate addressing mode in the destination operand");
						return 0;
					}
				}

				token = strtok(NULL, delim);
				if (token != NULL)
				{
					printError("Too much words in the command");
					return 0;
				}

				sizeOfCommand++; /*opcode instruction size*/

				/* For example: "sub r1,r4" */
				if (srcOperandAddressing == DIRECT_REGISTER && destOperandAddressing == DIRECT_REGISTER)
					sizeOfCommand++;

				/* For example: "mov S1.1,S2.2" - requires the address of S1 + the number at the first field in S1 + the address of S2 + the number at the second field in S2 (4 instructions in total)*/
				else if (srcOperandAddressing == STRUCT_ACCESS && destOperandAddressing == STRUCT_ACCESS)
					sizeOfCommand += 4;

				/* For example: "cmp S1.1,#2" - requires the address of S1 + the number at the first field in S1 + the number 2 (3 instructions in total) */
				else if (srcOperandAddressing == STRUCT_ACCESS || destOperandAddressing == STRUCT_ACCESS)
					sizeOfCommand += 3;

				/* All the other cases, for example: "lea STR,r1" - requires the address of STR + the code of the register r1 (2 instructions total) */
				else
					sizeOfCommand += 2;

				return sizeOfCommand;
			}
			break;

			/* 1 Operand Opcode*/
			case SECOND_GROUP:
			{
				token = strtok(NULL, delim);
				if (token == NULL)
				{
					printError("Too few operands in the command");
					return FALSE;
				}
				if (isKeyword(token))
				{
					printError("An operand name cannot be the same as a keyword name");
					return FALSE;
				}

				if ((destOperandAddressing = getOperandAddressing(token)) == -1)
				{
					printError("Destination operand is invalid");
					return 0;
				}

				if (destOperandAddressing == IMMEDIATE)
				{
					if (strcmp(opcode->opcodeName, "prn") != 0)
					{
						printError("Only \"prn\" opcode can use immediate addressing mode in the destination operand");
						return 0;
					}
				}

				token = strtok(NULL, delim);
				if (token != NULL)
				{
					printError("Too much words in the command");
					return 0;
				}

				sizeOfCommand++;
				if (destOperandAddressing == STRUCT_ACCESS)
					sizeOfCommand += 2;
				else
					sizeOfCommand++;

				return sizeOfCommand;
			}
			break;

			/* No Operands Opcode */
			case THIRD_GROUP:
			{
				token = strtok(NULL, delim);
				if (token != NULL)
				{
					printError("Too much words in the command");
					return 0;
				}
				sizeOfCommand = 1;
				return sizeOfCommand;
			}
			break;

			default:
				return 0;
			}
			return 0;
		}
	}
	return 0;
}




/*
Description: convert 10 word length represents in binary code to "wierd 32 base"
*/
void decimalToWierd(int num, char* res)
{
	char *p1, *p2;
	int div;
	int temp = num;

	if (num<0)/*case - num < 0*/
	{
		num = MAX10BITS - (num*(-1));
		printf("%d\n", num);
	}

	if (num < 0)
	{
		strcpy(res, WIERD_32_BASE[0]);
	}
	while (num)
	{
		div = BASE_LENGTH * (((double)num / BASE_LENGTH) - (num / BASE_LENGTH));
		res = strcat(res, WIERD_32_BASE[div]);
		num = num / BASE_LENGTH;
	}
	if (temp > 0 && temp < BASE_LENGTH)
	{
		res = strcat(res, WIERD_32_BASE[0]);

	}
	for (p1 = res, p2 = res + strlen(res) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
}

void binaryToWierd(int *binary, char* res)
{
	int num = 0;
	num = binaryToDecimal(binary);
	decimalToWierd(num, res);
}

/*arr size = 8(number in command) 10(data)*/
void decimalToBinary(int num, int *binaryNum, int arrSize)
{
	int i;
	if (num<0)/*case - num < 0*/
	{
		num = MAX10BITS - num * (-1);
	}
	for (i = arrSize - 1; i >= 0; i--)
	{
		binaryNum[i] = num % 2;
		num = num / 2;
	}
}

int binaryToDecimal(int *binaryNum)
{
	int i;
	int num = 0;
	int mult = 1;
	for (i = WORD_SIZE - 1; i >= 0; i--)
	{
		num = num + binaryNum[i] * mult;
		mult = mult * 2;
	}
	return num;
}


bool isDataCommand(char* token)
{
	char * temp;
	temp = trimStr(token);
	while (!isspace(*temp))
	{
		temp++;
	}
	while (isspace(*temp))
	{
		temp++;
	}
	printf("%s\n", temp);
	if (!strncmp(temp, ".data", 5) || !strncmp(temp, ".string", 7) || !strncmp(temp, ".struct", 7))
		return TRUE;
	return FALSE;
}

int getNumber(char* token)
{
	char * temp;
	char * num;
	int end = 0;
	temp = trimStr(token);
	if (strncmp(temp, "#", 1) == 0)
	{
		temp++;
	}
	num = temp;
	return atoi(num);
}

bool isNumber(char* token)
{
	char * temp;
	temp = trimStr(token);
	if (strncmp(temp, "#", 1) == 0)
	{
		return TRUE;
	}
	else if (strncmp(temp, "+", 1) == 0 || strncmp(temp, "-", 1) == 0)
	{
		return TRUE;
	}
	else if (isdigit(temp[0]))
	{
		return TRUE;
	}
	return FALSE;
}

bool isKeyword(char* token)
{
	int i = 0;
	char *tmp = trimStr(token);
	for (i = 0; i < NUM_OF_KEYWORDS; i++)
	{
		if (strcmp(Keywords[i], tmp) == 0)
			return TRUE;
	}
	return FALSE;
}

bool isStructWithDotOperand(char* operand)
{
	if(strchr(operand, '.'))
		return TRUE;
	return FALSE;
}

bool readLine(FILE* fp, char* line)
{
	lineCounter++;
	if (line == NULL)
		return FALSE;
	if(feof(fp))
		return FALSE;
	return (fgets(line, MAX_LINE_LENGTH, fp) != NULL) ? TRUE : FALSE;
}