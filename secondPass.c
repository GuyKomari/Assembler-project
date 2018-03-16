#include "secondPass.h"

int ICounter = IC_START;/*IC*/

FILE *sourceFileHandle; /* input file handle */
FILE *objFile, *entFile, *extFile; /* output files handles */
char objFileName[MAX_FILE_NAME + 1] = { 0 }; /* object file name */
char entFileName[MAX_FILE_NAME + 1] = { 0 }; /* entry file name */
char extFileName[MAX_FILE_NAME + 1] = { 0 }; /* extern file name */
char *assemblerFileName;
bool extFlag = FALSE;

extern IC , DC ; /* IC and DC after the first pass */
extern symbolPtr symbolListHead, symbolListTail; /* extern from firstPass.c */
extern dataPtr dataListHead, dataListTail; /* extern from firstPass.c */ 

/*
secondPass:
Does the second pass, return true if succeeded
*/
bool secondPass(char* fileName)
{
	int i = 0;
	bool is_label, is_data_command, endFile, is_entry, entFlag;
	char line[MAX_LINE_LENGTH + 1] = {0}; /* line in the assembler file */

	lineCounter = 0; /* resets the global line counnter */
	is_label = is_data_command = endFile = is_entry = entFlag = FALSE;
	assemblerFileName = fileName;
	sourceFileHandle = fopen(fileName, "r");
	if (sourceFileHandle == NULL)
	{
		printFileError(OPEN_FILE_ERROR, fileName);
		return FALSE;
	}
	/* input file opened successfully */
	createFile(fileName, objFile, objFileName, OBJECT_FILE_END);/* create the object output file */

	if(!isFileExists(objFileName))/*checks if created successfully*/
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return FALSE;
	}
	printIcAndDCWeird();/* prints the IC and the DC to the object file int the first line */
	printf("%s\n", "start of the second pass");/*TODO: REMOVE*/

	/* parse the input assembler file */
	while ((endFile = readLine(sourceFileHandle, line)))
	{
		/*TODO : check for extern label if defined, else print warning*/
		if(isEmptySentence(line) || isComment(line) || isExtern(line))/*case - empty line OR comment OR extern then continue*/
		{
			continue;
		}	
		is_entry = isEntry(line);
		is_label = isLabel(line, NULL);/*contains a label definition*/
		if(!is_entry && is_label)
		{
			is_data_command = isDataCommand(line);/*contains a data declaration*/
		}
		if (is_entry)
		{
			if(!entFlag)/*first entry in the input file*/
			{
				entFlag = TRUE;
				createFile(fileName, entFile, entFileName, ENTRY_FILE_END);/*create the entry output file*/
			}
			if(!isFileExists(entFileName))
			{
				printFileError(OPEN_FILE_ERROR, entFileName);
				return FALSE;
			}
			printToEntryFile(entFileName ,line);/*print the label name and its address to the entry file*/
			continue;
		}
		if(is_label && is_data_command)/*TODO. case - label with data*/
		{
			continue;
		}
		else/*label with command*/
		{
			encodingCommand(objFileName, line);
		}
		for (i = 0; i < MAX_LINE_LENGTH + 1; i++)
		{
			line[i] = 0;
		}
	}
	printDataWeird(objFileName);/*print the data in weird base after finished to print the code*/
	freeSymbolsList(&symbolListHead);
	freeDataList(&dataListHead);
	return TRUE;
}

void printIcAndDCWeird()
{
	char weirdIC[MAX_32_WEIRD_LENGTH] = { 0 };
	char weirdDC[MAX_32_WEIRD_LENGTH] = { 0 };
	objFile = fopen(objFileName, "wb");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return FALSE;
	}
	decimalToWierd((IC - IC_START), weirdIC);
	decimalToWierd(DC, weirdDC);
	fprintf(objFile," %s	 %s  \n\n", weirdIC, weirdDC);
	fclose(objFile);
}

void printDataWeird(char *objFileName)
{
	int i = 0;
	dataPtr temp = dataListHead;
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };

	objFile = fopen(objFileName, "ab");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}

	while (temp)
	{
		for (i = 0; i<MAX_32_WEIRD_LENGTH; i++)
		{
			WeirdLineCounter[i] = 0;
			weirdWord[i] = 0;
		}
		decimalToWierd((temp->asciiCode), weirdWord);
		decimalToWierd(ICounter, WeirdLineCounter);
		fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
		temp = temp->next;
		ICounter++;
	}
	fclose(objFile);
}




void printToEntryFile(char* entFileName, char* line)
{
	char *temp = trimStr(line);/*clear whitespaces*/
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	char weirdLabelAddress[MAX_32_WEIRD_LENGTH] = { 0 };
	entFile = fopen(entFileName,"ab");

	if(!entFile)
	{
		printFileError(OPEN_FILE_ERROR, entFileName);
		return;
	}
	temp = (char *)(temp + ENTRY_LENGTH);/*move the pointer after the ".entry" command*/
	while(isspace(*temp))/*ignore whitespaces*/
		temp++;
	if(!isLabelDefined(&symbolListHead, temp))
	{
		printFileError(LABEL_DECLARED_BUT_NOT_DEFINED, temp);/* TODO: chang to warning */
		return;
	}
	else
	{
		while(searchLabel)
		{
			if(strcmp((searchLabel->name),temp)==0)/*find the node that hands that label*/
			{
				decimalToWierd((searchLabel->address), weirdLabelAddress);/*convert the label address to "32 weird base"*/
				fprintf(entFile, "%s	%s\n", temp, weirdLabelAddress);
				break;
			}
			searchLabel = searchLabel->next;
		}
	}
	fclose(entFile);
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
		printFileError(ERROR_CREATE_FILE, destName);
		return;
	}
	fclose(dest);
}


void encodingCommand(char* objFileName, char* line)
{
	int i;
	int opcodeGroup;
	char * temp ;
	char * firstOperand;
	char * secondOperand;
	char opcodeName[5] = { 0 };/*opcode word size*/
	char label[MAX_LINE_LENGTH]={0};/*the label*/
	temp = firstOperand = secondOperand = NULL;
	if(!isFileExists(objFileName))
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}

	temp = trimStr(line);
	opcodeGroup = 0;

	if(isLabel(temp,label))/*case - commmand starts with label*/
	{
		temp = (char*)(temp + strlen(label) + 1);
	}
	while (isspace(*temp))/*ignore whitespaces*/
	{
		temp++;
	}
	strcat(temp, "\n");
	for (i = 0; *temp!=NULL && (!isspace(*temp)) ; i++)
	{
		opcodeName[i] = temp[0];
		temp++;
	}
	opcodeGroup = getOpcodeGroup(opcodeName);/*to which group the opcode belongs*/
	if (opcodeGroup != THIRD_GROUP)
	{
		while (isspace(*temp))/*ignore whitespaces*/
		{
			temp++;
		}
	}

	switch(opcodeGroup)
	{
		case FIRST_GROUP:/* two operands*/
		{
			firstOperand = strtok(temp,",");/*the operands are separated by a comma*/
			secondOperand = strtok(NULL,"");
			makeBinaryCode(objFileName, opcodeGroup, opcodeName, firstOperand, secondOperand);
			break;
		}
		case SECOND_GROUP:/*single operand*/
		{
			secondOperand = strtok(temp,"");
			makeBinaryCode(objFileName, opcodeGroup, opcodeName, firstOperand, secondOperand);
			break;
		}
		case THIRD_GROUP:/*opcode only*/
		{
			makeBinaryCode(objFileName, opcodeGroup, opcodeName, firstOperand, secondOperand);
			break;
		}
		default:
		{
		 	break;
		}
	}
}


int getOpcodeGroup(char *opcodeName)
{
	int i = 0;
	for(i = 0; i < FIRST_GROUP_SIZE; i++)
	{
		if(strcmp(OpcodesFirstGroup[i], opcodeName) == 0)
			return FIRST_GROUP;
	}
	for(i=0; i < SECOND_GROUP_SIZE; i++)
	{
		if(strcmp(OpcodesSecondGroup[i], opcodeName) == 0)
			return SECOND_GROUP;
	}
	return THIRD_GROUP;
}

void makeBinaryCode(char* objFileName, int opcodeGroup, char* opcode, char* firstOperand, char *secondOperand)
{
	int i = 0 , j = 0;

	int binaryWord[WORD_SIZE] = {0};/*the whole line in binary code*/
	int binaryOpcode[OPCODE_SIZE] = {0};
	int binaryFirstOperand[OPERAND_SIZE] = {0};
	int binarySecondOperand[OPERAND_SIZE] = {0};

	if(!isFileExists(objFileName))
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	switch(opcodeGroup)
	{
		case THIRD_GROUP:
		{
			setOpcodeBinaryCode(opcode, binaryWord);
			break;
		}
		case SECOND_GROUP:
		{
			setOpcodeBinaryCode(opcode, binaryOpcode);
			makeSingleOperandBinary(secondOperand, binarySecondOperand);
			break;
		}
		case FIRST_GROUP:
		{
			setOpcodeBinaryCode(opcode, binaryOpcode);
			makeSingleOperandBinary(firstOperand, binaryFirstOperand);
			makeSingleOperandBinary(secondOperand, binarySecondOperand);
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
	makeOperandsBinary(objFileName, opcodeGroup, binaryFirstOperand, binarySecondOperand, firstOperand, secondOperand);/**/
}

void makeSingleOperandBinary(char *operand, int *binary)
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
		binary[0] = 1;
		binary[1] = 0;
	}
	else/*addressing type number 4*/
	{
		binary[0] = 0;
		binary[1] = 1;
	}
}

void printBinaryWord(char *objFileName, int *binaryWord)
{
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = {0};
	char weirdWord[MAX_32_WEIRD_LENGTH] = {0};

	binaryToWierd(binaryWord, weirdWord);/**/
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
	ICounter++;
	fclose(objFile);
}	


void setOpcodeBinaryCode(char *operand, int *binary)
{
	int i = 0;
	for(i = 0; i < NUM_OF_OPCODES; i++)
	{
		if(strcmp(((opcodes[i]).opcodeName), operand) == 0)
		{
			copyBinaryOpcode(i, binary);
			return;
		}
	}
}

void copyBinaryOpcode(int index, int *binary)
{
	int i = 0;
	for(i = 0; i < OPCODE_SIZE; i++)
	{
		binary[i] = (opcodes[index].binaryCode)[i];
	}
}


void makeOperandsBinary(char *objFileName, int opcodeGroup, int *binaryFirstOperand, int *binarySecondOperand, char *firstOperand,char *secondOperand)
{
	int i;
	bool label, reg, number;
	int firstAddressingType, secondAddressingType; 

	if(opcodeGroup == THIRD_GROUP)
		return;

	label = reg = number = FALSE;
	firstAddressingType = secondAddressingType = i = 0;

	secondAddressingType = getAddressingType(binarySecondOperand);
	if (opcodeGroup == SECOND_GROUP)
	{
		printSecondOperand(objFileName, firstOperand, secondOperand, secondAddressingType);
		return;
	}

	/*case - FIRST_GROUP*/
	firstAddressingType = getAddressingType(binaryFirstOperand);
	switch(firstAddressingType)
	{
		case 0:/*first operand is a number*/
		{
			printNumberWeird(objFileName, firstOperand);
			printSecondOperand(objFileName, firstOperand, secondOperand, secondAddressingType);
			break;
		}
		case 1:/*first operand is data*/
		{
			printWeirdDataOperand(objFileName, firstOperand);
			printSecondOperand(objFileName, firstOperand, secondOperand, secondAddressingType);
			break;
		}
		case 2:/*first operand is struct with dot*/
		{	
			printStructWithDotWeird(objFileName, firstOperand);
			printSecondOperand(objFileName, firstOperand, secondOperand, secondAddressingType);
			break;
		}
		case 3:/*first oeprand is register*/
		{
			if(isRegister(secondOperand))
			{
				printRegisterWeird(objFileName, firstOperand, secondOperand, TRUE, TRUE);
			}
			else
			{
				printRegisterWeird(objFileName, firstOperand, secondOperand, TRUE, FALSE);
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
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };

	for(i = 0; i < NUM_OF_REGISTERS; i++)
	{
		if((firstRegister) && strcmp(((registersBinary[i]).registerName), firstOperand) == 0)
		{
			copyBinaryRegister(binaryWord, i);
		}
		if((secondRegister) && strcmp(((registersBinary[i]).registerName), secondOperand) == 0)
		{
			copyBinaryRegister(binaryWord, i);
		}
	}
	binaryToWierd(binaryWord, weirdWord);
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
	ICounter++;

	fclose(objFile);
}

void copyBinaryRegister(int *binaryWord,int index)
{
	int i = 0;
	for(i = 0; i < OPCODE_SIZE; i++)
	{
		binaryWord[i] = (registersBinary[index].address)[i];
	}	
}
void printStructWithDotWeird(char *objFileName, char *firstOperand)
{
	char *temp;
	temp = strtok(firstOperand, ".");/*the label name example: s.1 -> temp = s*/
	printWeirdDataOperand(objFileName, firstOperand);
	temp = strtok(NULL, ""); /*s.1 -> temp = 1*/
	printNumberWeird(objFileName, temp);
}

void printNumberWeird(char* objFileName, char *firstOperand)
{
	int number = getNumber(firstOperand);/*TODO*/
	int binaryWord[WORD_SIZE] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };
	char NumberWeird[MAX_32_WEIRD_LENGTH] = { 0 };

	decimalToBinary(number, binaryWord, EIGHT_BITS);
	binaryToWierd(binaryWord, NumberWeird);
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
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

void printWeirdDataOperand(char* objFileName, char *operand)
{
	int i;
	int addressNum;
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	bool toWeird;
	int binaryWord[WORD_SIZE] = {0};
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = {0};

	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if(!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}

	i = 0;
	toWeird = TRUE;

	while(searchLabel)
	{
		if(strcmp((searchLabel->name),operand)==0)/*find the node that hands that label*/
		{
			decimalToBinary(addressNum = (searchLabel->address), binaryWord, EIGHT_BITS);/*convert the label address to "32 weird base"*/
			if((searchLabel->isExternal) == TRUE)/*ARE*/
			{
				binaryWord[8] = 0;
				binaryWord[9] = 1;
				printToExternFile(operand, addressNum);
			}
			else/*ARE*/
			{
				binaryWord[8] = 1;
				binaryWord[9] = 0;
			}
			binaryToWierd(binaryWord, weirdWord);
			fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
			ICounter++;
			/*print to the extern file */
			break;
		}
		searchLabel = searchLabel->next;
	}
	fclose(objFile);
}

void printToExternFile(char *operand, int addressNum)
{
	char weirdAddress[MAX_32_WEIRD_LENGTH] = { 0 };
	if (!extFlag)
	{
		createFile(assemblerFileName, extFile, extFileName, EXTERN_FILE_END);/* create the object output file */
	}
	extFile = fopen(extFileName, "ab");
	if (!extFile)
	{
		printFileError(OPEN_FILE_ERROR, extFileName);
	}
	decimalToWierd(addressNum, weirdAddress);
	fprintf(extFile, "%s	%s", operand, addressNum);
	fclose(extFile);
}



int getAddressingType(int* binaryOperand)
{
	int res = 0;
	res = 2*binaryOperand[0] + binaryOperand[1];
	return res;
}



