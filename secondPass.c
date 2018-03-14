#include "secondPass.h"

int ICounter = IC_START;/*IC*/


FILE *sourceFileHandle;/*input file handle*/
FILE *objFile, *entFile, *extFile; /*output files handles*/

extern lineCounter;/*line counter*/
extern IC , DC ;/*needed?*/
extern symbolListHead, symbolListTail, dataListHead, dataListTail;

lineCounter = 0;
/*
does the second pass, return true if succeeded
*/
bool secondPass(char* fileName)
{
	bool is_label, is_data_command, endFile, is_entry, extFlag, entFlag;

	char *objFileName[MAX_FILE_NAME] = {0}; /*object file name*/
	char *entFileName[MAX_FILE_NAME] = {0};	/*entry file name*/
	char *extFileName[MAX_FILE_NAME] = {0}; /*entry file name*/

	char line[MAX_LINE_LENGTH] = {0};
	is_label = is_data_command = endFile = is_entry = extFlag = entFlag = FALSE;

	sourceFileHandle = fopen(fileName, "r");
	if (sourceFileHandle == NULL)
	{
		printError(OPEN_FILE_ERROR, fileName);/*TODO: add a define to errors.h*/
		return;
	}
	/*input file opened successfully*/
	createFile(fileName, objFile, objFileName, OBJECT_FILE_END);/*create the object output file*/

	if(!isFileExists(objFileName))/*checks if created successfully*/
	{
		printError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	/*parse the input assembler file*/
	while (!(endFile = readLine(sourceFileHandle, line)))
	{
		lineCounter++;
		if(isEmptySentence(line) || isComment(line) || isExtern(line))/*case - empty line OR comment OR extern then continue*/
		{
			continue;
		}
		is_label = isLabel(line, NULL);/*contains a label definition*/
		if(is_label)
		{
			is_data_command = isDataCommand(line);/*contains a data declaration*/
		}
		is_entry = isEntry(line);

		if (is_entry)
		{
			if(!entFlag)/*first entry in the input file*/
			{
				entFlag = TRUE;
				createFile(fileName, entFile, entFileName, ENTRY_FILE_END);/*create the entry output file*/
			}
			if(!isFileExists(entFileName))
			{
				printError(OPEN_FILE_ERROR, entFileName);
				return;
			}
			printToEntryFile(entFileName ,line);/*print the label name and its address to the entry file*/
			continue;
		}
		if(is_label && is_data_command)/*TODO. case - label with data*/
		{
			encodingData(line);/*encoding a the data and add to the dataEncodingList*/
			continue;
		}
		else/*label with command*/
		{
			encodingCommand(objFileName, line);
		}
		printDataWeird(objFileName);/*print the data in weird base after finished to print the code*/
	}
}


void printToEntryFile(char* entFileName, char* line)
{
	int i;
	char *temp = trimStr(line);/*clear whitespaces*/
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	bool toWeird;
	FILE *entryFile;
	char weirdLabelAddress[BASE_LENGTH] = { 0 };
	entryFile = fopen(entryFile,"ab");

	if(!entryFile)
	{
		printError(OPEN_FILE_ERROR, entFileName);
		return;
	}
	temp = (char *)(temp + ENTRY_LENGTH);/*move the pointer after the ".entry" command*/
	i = 0;
	toWeird = TRUE;
	while(isspace(*temp))/*ignore whitespaces*/
		temp++;
	if(!isLabelDefined(symbolListHead, temp))
	{
		printWarning(LABEL_DECLARED_BUT_NOT_DEFINED, temp);/*TODO: add to errors.h*/
		return;
	}
	else
	{
		while(searchLabel)
		{
			if(strcmp((searchLabel->name),temp)==0)/*find the node that hands that label*/
			{
				if(toWeird = decimalToWierd((searchLabel->address), weirdLabelAddress))/*convert the label address to "32 weird base"*/
				{
					fprintf(entryFile, "%s	%s\n", temp, weirdLabelAddress);
				}
				else
				{
					printError(ERROR_CONVERT_LABEL_ADDRESS_TO_WEIRD, temp);
				}
				break;
			}
			searchLabel++;
		}
	}
	fclose(entryFile);
}

void createFile(char* fileName, FILE* dest, char* destName, char* end)
{
	char* name;
	char* endfile;
	char* i;
	char* dot;
	endfile = (char*)(fileName+strlen(fileName));
	dot = endfile;
	for(i = endfile; i>fileName; i--)
	{
		if(strncpy(endfile,".",1)==0)
		{
			dot = i;
			break;
		}
	}
	name = trimStr(fileName);
	name = strtok(name,dot);
	name = strcat(name, end);
	strcpy(destName, name);
	dest = fopen(name,"wb");
	if(!dest)
	{
		printf(ERROR_CREATE_FILE, destName);
		return;
	}
	fclose(dest);
}


void encodingCommand(char* objFileName, char* line)
{
	int opcodeGroup;
	char *temp, startLabel, opcodeName, firstOperand, secondOperand;
	char label[MAX_LINE_LENGTH]={0};/*the label*/

	if(!isFileExists(objFileName))
	{
		printError(OPEN_FILE_ERROR, objFileName);
		return;
	}

	temp = trimStr(line);
	startLabel = opcodeName = firstOperand = secondOperand = NULL;
	opcodeGroup = 0;

	if(isLabel(temp,label))/*case - commmand starts with label*/
	{
		temp = (char*)(temp + strlen(startLabel));
	}
	temp = trimLeftStr(temp);
	opcodeName = strtok(temp, " ");/*opcode name*/
	temp = (char*)(temp + strlen(opcodeName));
	while(isspace(temp));
		temp++;
	opcodeGroup = getOpcodeGroup(opcodeName);/*to which group the opcode belongs*/
	switch(opcodeGroup)
	{
		case FIRST_GROUP:/* two operands*/
		{
			firstOperand = strtok(NULL,",");/*the operands are separated by a comma*/
			firstOperand = trimStr(firstOperand);
			secondOperand = strtok(NULL,"");
			secondOperand = trimStr(secondOperand);
			makeBinaryCode(objFileName, opcodeName, firstOperand, secondOperand, THREE_OPERANDS);
		}
		case SECOND_GROUP:/*single operand*/
		{
			firstOperand = strtok(NULL,",");
			firstOperand = trimStr(firstOperand);
			makeBinaryCode(objFileName, opcodeName, firstOperand, NULL, TWO_OPERANDS);
		}
		case THIRD_GROUP:/*opcode only*/
		{
			makeBinaryCode(objFileName, opcodeName, firstOperand, NULL, SINGLE_OPERAND);
		}
		default:
		{
			printError(OPCODE_GROUP_ERROR, opcodeName);/*TODO: ... */
		 	break;
		}
	}
}


int getOpcodeGroup(char *opcodeName)
{
	int i = 0;
	/*TODO: move to globalVariabels.c

	char OpcodesFirstGroup[FIRST_GROUP_SIZE] = {"mov","cmp","add","sub","lea"};
	char OpcodesSecondGroup[SECOND_GROUP_SIZE] = {"not","clr","inc","dec","jmp","bne","red","prn","jsr"};
	char OpcodesThirdGroup[THIRD_GROUP_SIZ] = {"rts","stop"};
	*/
	extern OpcodesFirstGroup, OpcodesSecondGroup, OpcodesThirdGroup;

	for(i = 0; i < FIRST_GROUP_SIZE; i++)
	{
		if(strcmp(OpcodesFirstGroup + i, opcodeName))
			return FIRST_GROUP;
	}
	for(i=0; i<SECOND_GROUP_SIZE; i++)
	{
		if(strcmp(OpcodesSecondGroup + i, opcodeName))
			return SECOND_GROUP;
	}
	for(i=0; i<THIRD_GROUP_SIZE; i++)
	{
		if(strcmp(OpcodesThirdGroup + i, opcodeName))
			return THIRD_GROUP;
	}
	return 0;
}

void makeBinaryCode(char* objFileName, char* opcode, char* firstOperand, char *secondOperand, int numOfOperands)
{
	int i = 0 , j = 0;

	int binaryWord[WORD_SIZE] = {0};/*the whole line in binary code*/
	int binaryOpcode[OPCODE_SIZE] = {0};
	int binaryFirstOperand[OPERAND_SIZE] = {0};
	int binarySecondOperand[OPERAND_SIZE] = {0};

	if(isFileExists(objFileName))
	{
		printError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	switch(numOfOperands)
	{
		case SINGLE_OPERAND:
		{
			setOpcodeBinaryCode(opcode, binaryWord);
			break;
		}
		case TWO_OPERANDS:
		{
			setOpcodeBinaryCode(opcode, binaryOpcode);
			makeOperandBinary(firstOperand,binaryFirstOperand);
			break;
		}
		case THREE_OPERANDS:
		{
			setOpcodeBinaryCode(opcode, binaryOpcode);
			makeOperandBinary(firstOperand, binaryFirstOperand);
			makeOperandBinary(secondOperand, binarySecondOperand);
			break;
		}
		default: break;
	}
	for(i = 0 ; i < OPCODE_SIZE ; i++, j++)
		binaryWord[j] = binaryOpcode[i];
	for(i = 0 ; i < OPERAND_SIZE ; i++, j++)
		binaryWord[j] = binaryFirstOperand[i];
	for(i = 0 ; i < OPERAND_SIZE ; i++, j++)
		binaryWord[j] = binarySecondOperand[i];
	printBinaryWord(objFileName, binaryWord);/*print the word in 32 weird base*/	
	makeOperandsBinary(objFileName, binaryFirstOperand, binarySecondOperand, firstOperand, secondOperand, numOfOperands)/**/
}

void makeOperandBinary(char *operand, int binary[])
{
	int i = 0;

	if(isNumber(operand))/*addressing type number 1*/
	{
		binary[0] = 0;
		binary[1] = 0;
	}
	else if(isRegister(operand))/*addressing type number 2*/
	{
		binary[0] = 1;
		binary[1] = 1;
	}
	else if(isStructWithDotOperand(operand))/*addressing type number 3*/
	{
		binary[0] = 0;
		binary[1] = 1;
	}
	else/*addressing type number 4*/
	{
		binary[0] = 1;
		binary[1] = 0;
	}
}

void printBinaryWord(char *objFileName, int *binaryWord)
{
	char WeirdLineCounter[BASE_LENGTH] = {0};
	char weirdWord[BASE_LENGTH] = {0};

	binaryToWierd(binaryWord, weirdWord);/**/
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
	ICounter++;
	fclose(objFile);
}	


void setOpcodeBinaryCode(char *operand, int *binary)
{
	int i = 0;
	extern opcodes;
	for(i = 0; i < NUM_OF_OPCODES; i++)
	{
		if(strcmp(((opcodes[i])->opcodeName), operand) == 0)
		{
			copyBinaryOpcode(opcodes, i, binary);
		}
	}
}

void copyBinaryOpcode(opcodeStructure *opcodes, int index, int *binary)
{
	int i = 0;
	for(i = 0; i < OPCODE_SIZE; i++)
	{
		binary[i] = (opcodes[index].binaryCode)[i];
	}
}

bool isStructWithDotOperand(char* operand)
{
	if(strchr(operand, '.'))
		return TRUE;
	return FALSE;
}


void makeOperandsBinary(char *objFileName, int *binaryFirstOperand, int *binarySecondOperand, char *firstOperand,char *secondOperand, int numOfOperands)
{
	int i;
	bool label, reg, number;
	int firstAddressingType, secondAddressingType; 

	if(numOfOperands == SINGLE_OPERAND)
		return;

	/*
	char operndWeird[WORD_SIZE] = {0};
	char firstOperandWeird[MAX_32_WEIRD_LENGTH] = {0};
	char secondOperandWeird[MAX_32_WEIRD_LENGTH] = {0};
	char lineWeird[MAX_32_WEIRD_LENGTH] = {0};
	*/

	label = reg = number = FALSE;
	firstAddressingType = secondAddressingType = i = 0;

	firstAddressingType = getAddressingType(binaryWord, "first");
	secondAddressingType = getAddressingType(binaryWord, "second");

	switch(firstAddressingType)
	{
		case 0:/*first operand is a number*/
		{
			printNumberWeird(objFileName, firstOperand);
			if(numOfOperands == THREE_OPERANDS)
				printSecondOperand(objFileName, secondOperand, secondAddressingType);
			break;
		}
		case 1:/*first operand is data*/
		{
			printWeirdDataOperand(objFileName, firstOperand);
			if(numOfOperands == THREE_OPERANDS)
				printSecondOperand(objFileName, secondOperand, secondAddressingType);
			break;
		}
		case 2:/*first operand is struct with dot*/
		{	
			printStructWithDotWeird(objFileName, firstOperand);
			if(numOfOperands == THREE_OPERANDS)
				printSecondOperand(objFileName, secondOperand, secondAddressingType);
			break;
		}
		case 3:/*first oeprand is register*/
		{
			if((numOfOperands == THREE_OPERANDS) && isRegister(secondOperand))
			{
				printRegisterWeird(objFileName, firstOperand, secondOperand, TRUE, TRUE);
			}
			else
			{
				printRegisterWeird(objFileName, firstOperand, secondOperand, TRUE, FALSE);
				if(numOfOperands == THREE_OPERANDS)
					printSecondOperand(objFileName, firstOperand, secondOperand, secondAddressingType);
			}
			break;
		}
		default: break;
	}
}

void printRegisterWeird(char *objFileName, char *firstOperand, char *secondOperand, bool firstRegister, bool secondRegister)
{
	int i;
	int binaryWord[WORD_SIZE] = { 0 };
	char weirdWord[BASE_LENGTH] = { 0 };
	char WeirdLineCounter[BASE_LENGTH] = { 0 };

	extern registersBinary;
	for(i = 0; i < NUM_OF_REGISTERS; i++)
	{
		if((firstRegister) && (((registersBinary[i])->registerName), firstOperand) == 0)
		{
			copyBinaryRegister(binaryWord, registersBinary, i, "first");
		}
		if((secondRegister) && strcmp(((registersBinary[i])->registerName), secondOperand) == 0)
		{
			copyBinaryRegister(binaryWord, registersBinary, i, "second");
		}
	}
	binaryToWierd(binaryWord, weirdWord);
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
	ICounter++;

	fclose(objFile);
}

void copyBinaryRegister(int *binaryWord, RegisterStructure registers, int index, char *operand)
{
	int i = 0;
	if(strcmp(operand, "first") == 0)
	{
		for(i = 0; i < REGISTER_SIZE; i++)
		{
			binaryWord[i] = (registers[index]->address)[i];
		}
	}
	else/*seccond*/
	{
		for(i = REGISTER_SIZE - 1; i < REGISTER_SIZE*2 ; i++)
		{
			binaryWord[i] = (registers[index]->address)[i];
		}
	}	
}
void printStructWithDotWeird(char *objFileName, char *firstOperand)
{
	char *temp;
	temp = strtok(firstOperand, ".");/*the label name example: s.1 -> temp = s*/
	printWeirdDataOperand(objFileName, firstOperand);
	temp = strtok(NULL, ""); /*s.1 -> temp = 1*/
	printNumberWeird(objFileName, firstOperand);
}

void printNumberWeird(char* objFileName, char *firstOperand)
{
	int number = getNumber(firstOperand);/*TODO*/
	int binaryWord[WORD_SIZE] = { 0 };
	char WeirdLineCounter[BASE_LENGTH] = { 0 };
	char NumberWeird[BASE_LENGTH] = { 0 };

	decimalToBinary(number, binaryWord, EIGHT_BITS);
	binaryToWierd(binaryWord, NumberWeird);
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, NumberWeird);
	ICounter++;
	fclose(objFile);
}

void printSecondOperand(char* objFileName,char *firstOperand, char *secondOperand, int secondAddressingType)
{

	switch(secondAddressingType)
	{
		case 1:/*second operand is data*/
		{
			printWeirdDataOperand(objFileName, secondOperand);
			break;
		}
		case 2:/*second operand is struct with dot*/
		{
			printStructWithDotWeird(objFileName, secondOperand);
			break;
		}
		case 3:/*second operand is a register*/
		{
			printRegisterWeird(objFileName, firstOperand, secondOperand, FALSE, TRUE);
			break;
		}
		default: break;
	}

}

void printWeirdDataOperand(char* obfFileName, char *operand)
{
	int i;
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	bool toWeird;
	int binaryWord[WORD_SIZE] = {0};
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = {0};

	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(entryFile, "ab");
	if(!objFile)
	{
		printError(OPEN_FILE_ERROR, entFileName);
		return;
	}

	i = 0;
	toWeird = TRUE;

	while(searchLabel)
	{
		if(strcmp((searchLabel->name),operand)==0)/*find the node that hands that label*/
		{
			if(toWeird = decimalToBinary((searchLabel->address), binaryWord, EIGHT_BITS))/*convert the label address to "32 weird base"*/
			{
				if(isExtern(operand))/*ARE*/
				{
					binaryWord[8] = 0;
					binaryWord[9] = 1;
				}
				else/*ARE*/
				{
					binaryWord[8] = 1;
					binaryWord[9] = 0;
				}
				binaryToWierd(binaryWord, weirdWord);
				fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
				lineCounter++;
				break;
			}
			else
			{
				printError(ERROR_CONVERT_LABEL_ADDRESS_TO_WEIRD, operand);
			}
			break;
		}
		searchLabel++;
	}
	fclose(objFile);
}


int getAddressingType(int* binaryOperand, char* word)
{
	int res = 0;
	res = 2*binaryOperand[0] + binaryOperand[1];
	return res;
}



