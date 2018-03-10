#include <stdio.h>
#include <string.h>
#include "GlobalsFunctions.h"
#include "errors.h"


bool isRegister(char *token)
{
	int i = 0;
	extern char* Registers;
	for(i = 0; i > NUM_OF_REGISTERS ; i++)
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
	char * temp, start;
	int len;
	temp = trimStr(src);
	start = temp;
	len = 0;
	if (!(isalpha(*temp)))/*label doesnt start with a letter*/
	{
		printError(LABEL_NOT_START_WITH_ALPHA);
		return FALSE;
	}
	while (!isspace(*temp) && strncmp((char*)(temp), ":", 1) != 0)
	{
		if (!isalpha(*temp) && !isdigit(*temp))
		{
			printError(LABEL_CONTAINS_NON_ALPHA_OR_DIGIT);
			return FALSE;
		}
		temp++;
		len++;
	}
	if (strncmp((char*)(temp++), ":", 1) != 0)
	{
		printf(MISSSING_COLON_ERROR);
		return FALSE;
	}
	if (!isspace(*temp))
	{
		if (ispunct(*temp) || isdigit(*temp) || isalpha(*temp))
		{
			printError("additional character after declaration of a label");
			return FALSE;
		}
		printError("missing a space");
		return FALSE;
	}
	strncpy(dest, start, len);
	return TRUE;
}
/*
headOfSymbolsList - head of the symbols list;
token - is a valid label
lineCounter - the code line
*/
bool isLabelDefined(symbolPtr headOfSymbolsList, char* token, int lineCounter)
{
	while(headOfSymbolsList)
	{
		if(!(strcmp(token,(headOfSymbolsList->name))))
		{
			fprintf(stderr, "Label %s, line %d,  already defined",token,lineCounter);
			return TRUE;	
		}
		headOfSymbolsList = headOfSymbolsList->next;
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
		(*temp)++;
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
		(*temp)++;
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
    char* back = str + strlen(str);
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
	char *symbols[3] = {".string", ".data", ".struct"};
	bool afterLabel = FALSE, detectedSymbol = FALSE;
	char* trimmedData;
	int i, j, length, correctSymbol = -1;
	trimmedData = trimStr(data);
	length = strlen(trimmedData);
	for (i = 0; i < length; i++)
	{
		if (afterLabel)
		{
			if (!isspace(trimmedData[i]))
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
			if (!isalpha(trimmedData[i]))
				return FALSE;
			else if (trimmedData[i] == ":")
			{
				if (i != 0)
					afterLabel = TRUE;
				else
					return FALSE;
			}
			else
				return FALSE;
		}
	}

	if (correctSymbol != -1)
	{
		strncpy(dest, symbols[correctSymbol], strlen(symbols[correctSymbol]));
		return TRUE;
	}
	return FALSE;
}

bool addNumberToDataList(dataPtr *head, dataPtr *tail, int dc, int num)
{
	if (num >= 0)
	{
		addToDataList(dataListHead, dataListTail, dc, positiveNumber, num);
	}
	else
	{
		addToDataList(dataListHead, dataListTail, dc, negativeNumber, num);
	}
}


int addStringToData(dataPtr *dataListHead, dataPtr *dataListTail, char *str, long dc)
{
	return 1;
}




int getCommandSize(char* command)
{
	int i, j;
	AddressingMode srcOperandAddressing, destOperandAddressing;
	opcodeStructure *opcode;
	char* token;
	token = strtok(command, " \t,");

	for (i = 0; i < NUM_OF_OPCODES; i++)
	{
		if (strcmp(token, opcodes->opcodeName) == 0)
		{
			opcode = &(opcodes[i]);
			sizeOfCommand++;
			switch (opcode->group)
			{
				/* 2 Operands Opcode */
			case FIRST_GROUP:
			{
				token = strtok(NULL, " \t,");
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

				if (token[0] == '#') /*srcOperandAddressing = 0 or 2*/
				{
					if (strlen(token) <= 1 || (atoi(token[1]) == 0 && strcmp(token + 1, "0") != 0))
					{
						printError("A number is expected in the first operand");
						return FALSE;
					}
					sizeOfCommand++;
				}

				else
				{
					for (j = 0; j < NUM_OF_REGISTERS)
					{
						if (strcmp(token, Registers[j]) == 0)
						{
							srcOperandAddressing = DIRECT_REGISTER;


							break;
						}
					}
				}

			}
			break;

			/* 1 Operand Opcode*/
			case SECOND_GROUP:
			{
				token = strtok(NULL, " ,");
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

			}
			break;

			/* No Operands Opcode */
			case THIRD_GROUP:
			{

			}
			break;

			default:
				return FALSE;
			}
}



/*
TODO: add errors checking
Description: convert 10 word length represents in binary code to "wierd 32 base"
*/
bool binaryToWierd (int* binary, char* res)
{
	bool hasError;
	int num1, num2 ,mult, i;
	char* temp = (char*)malloc(sizeof(char));
	num1 = num2 = i = 0;
	mult = MULT;
	for(i = 0; i < (WORD_SIZE/2) ; i++)
	{
		num1 = num1 + mult*binary[i];
		mult=mult/2;
	}
	printf("num1 = %d\n", num1);
	decimalToWierd(num1,res);
	printf("\n%s\n",res );
	mult = MULT;
	for(i = WORD_SIZE/2; i < WORD_SIZE ; i++)
	{
		num2 = num2 + mult*binary[i];
		mult=mult/2;
	}
	decimalToWierd(num2,temp);
	res = strcat(res,temp);
	free(temp);
	return hasError;
}

/*
TODO: add errors checking
Description: convert a number in decimal base to "wierd 32 base"
*/
bool decimalToWierd(int num, char* res)
{
	bool error;
	char *p1, *p2;
	int div ;
	if(!num) /*case - num = 0*/
	{
		strcpy(res, WIERD_32_BASE[0]);
	}
	if(num<0) /*case - num < 0*/
	{
		num *= -1;
		num = MAX10BITS - num;
	}
	while(num)
	{
		div = BASE_LENGTH *(((double)num/BASE_LENGTH) - (num/BASE_LENGTH));
		res = strcat(res ,WIERD_32_BASE[div]);
		num = num/BASE_LENGTH;
	}
    for (p1 = res, p2 = res + strlen(res) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
	return TRUE;
}

/*TODO: add errors checking*/
bool decimalToBinary(int n, int binaryNum[], int arrSize)
{
	bool hasError;
	int i;
	if(n<0)/*case - num < 0*/
	{
		n = MAX10BITS - n*(-1);
	}
    for(i= arrSize-1 ; i>=0 ; i--) 
    {
        binaryNum[i] = n % 2;
        n = n / 2;
    }
    return hasError
}