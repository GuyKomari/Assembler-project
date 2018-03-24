#include "secondPass.h"

/* Variables declaration */

int ICounter = IC_START;/*IC*/
static FILE *sourceFileHandle; /* input file handle */
static FILE *objFile, *entFile, *extFile; /* output files handles */
static char objFileName[MAX_FILE_NAME_LENGTH + 1] = { 0 }; /* object file name */
static char entFileName[MAX_FILE_NAME_LENGTH + 1] = { 0 }; /* entry file name */
static char extFileName[MAX_FILE_NAME_LENGTH + 1] = { 0 }; /* extern file name */
static char *assemblyFileName;
static bool extFlag = FALSE;

extern int IC, DC; /* IC and DC after the first pass */
extern symbolPtr symbolListHead, symbolListTail; /* extern from firstPass.c */
extern dataPtr dataListHead, dataListTail; /* extern from firstPass.c */

/*
Description:
The main function of the second pass.
Generates the output files - object file, entry file, and extern file.
Parses from the input file, and converts any line and operand to "weird 32 base".
*/
bool secondPass(char* fileName)
{
	int i = 0;
	bool is_label, is_data_command, endFile, is_entry, entFlag;
	char line[MAX_LINE_LENGTH + 1] = { 0 }; /* line in the assembler file */

	lineCounter = 0; /* resets the global line counnter */
	is_label = is_data_command = endFile = is_entry = entFlag = FALSE;
	assemblyFileName = fileName;
	sourceFileHandle = fopen(fileName, "r");
	if (sourceFileHandle == NULL)
	{
		printFileError(OPEN_FILE_ERROR, fileName);
		freeSymbolsList(symbolListHead);
		freeDataList(dataListHead);
		return FALSE;
	}
	/* input file opened successfully */
	createFile(fileName, objFile, objFileName, OBJECT_FILE_END);/* create the object output file */

	if (!isFileExists(objFileName))/*checks if created successfully*/
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		freeSymbolsList(symbolListHead);
		freeDataList(dataListHead);
		return FALSE;
	}
	printIcAndDCWeird();/* prints the IC and the DC to the object file into the first line */
	/* parse the input assembler file */
	while ((endFile = readLine(sourceFileHandle, line)))
	{
		if (isEmptySentence(line) || isComment(line) || isExtern(line))/*case - empty line OR comment OR extern then continue*/
		{
			continue;
		}
		is_entry = isEntry(line);
		is_label = isLabel(line, NULL); /*contains a label definition*/
		if (!is_entry && is_label)
		{
			is_data_command = isDataCommand(line);/*contains a data declaration*/
		}
		if (is_entry)
		{
			if (!entFlag)/*first entry in the input file*/
			{
				entFlag = TRUE;
				createFile(fileName, entFile, entFileName, ENTRY_FILE_END);/*create the entry output file*/
			}
			if (!isFileExists(entFileName))
			{
				printFileError(OPEN_FILE_ERROR, entFileName);
				freeSymbolsList(symbolListHead);
				freeDataList(dataListHead);
				return FALSE;
			}
			printToEntryFile(line);/*print the label name and its address to the entry file*/
			continue;
		}
		if (is_label && is_data_command)
		{
			continue;
		}
		else/*label with command*/
		{
			encodingCommand(line);/*encodding the line and each operand*/
		}
		for (i = 0; i < MAX_LINE_LENGTH + 1; i++)/* reset the line */
		{
			line[i] = 0;
		}
	}
	printDataWeird();/*print the data in weird base after finished to print the instructions*/

	freeSymbolsList(&symbolListHead);/* free memory of the symbols list */
	freeDataList(&dataListHead);/* free memory of the symbols list */
	return TRUE;/* second pass succeeded */
}

/*
Description:
Prints the DC and the IC after were calculated during the first pass
*/
void printIcAndDCWeird()
{
	char weirdIC[MAX_32_WEIRD_LENGTH] = { 0 };
	char weirdDC[MAX_32_WEIRD_LENGTH] = { 0 };
	objFile = fopen(objFileName, "wb");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	decimalToWierd((IC - IC_START), weirdIC);
	decimalToWierd(DC, weirdDC);
	fprintf(objFile, " %s	 %s  \n\n", weirdIC, weirdDC);
	fclose(objFile);
}

/*
Description:
Prints the data of the assembly file in weird 32 base, after the instructions were printted.

objFileName - file name to print
*/
void printDataWeird()
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
/*
Description:
If the label was defined during the assembly -
then prints the entry label and its address to the entry file in weird 32 base,
otherwise prints a warning.

line - a single line from the assembly file
*/
void printToEntryFile(char* line)
{
	char *temp = trimStr(line);/*clear whitespaces*/
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	char weirdLabelAddress[MAX_32_WEIRD_LENGTH] = { 0 };
	entFile = fopen(entFileName, "ab");

	if (!entFile)
	{
		printFileError(OPEN_FILE_ERROR, entFileName);
		return;
	}
	temp = (char *)(temp + ENTRY_LENGTH);/*move the pointer after the ".entry" command*/
	while (isspace(*temp))/*ignore whitespaces*/
		temp++;
	if (LabelDeclaredButNotDefined(temp) == TRUE)
	{
		printWarning(LABEL_DECLARED_BUT_NOT_DEFINED, temp);
		fclose(entFile);
		return;
	}
	else
	{
		while (searchLabel)
		{
			if (strcmp((searchLabel->name), temp) == 0)/*find the node that hands that label*/
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
/*
Description:
Checks if an entry label was defined during the assembly file.

temp - label name
*/
bool LabelDeclaredButNotDefined(char *temp)
{
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	while (searchLabel)
	{
		if (strcmp((searchLabel->name), temp) == 0)/*find the node that hands that label*/
		{
			return FALSE;
		}
		searchLabel = searchLabel->next;
	}
	return TRUE;
}
/*
Description:
Generates an output file.

fileName - the input file name
dest - output file hendle
destName - output file name
end - output file extension
*/
void createFile(char* fileName, FILE* dest, char* destName, char* end)
{
	int strLength, fileExtensionLength, destExtensionLength, i;
	strLength = strlen(fileName);
	destExtensionLength = strlen(end);
	if (strLength >= MAX_FILE_NAME_LENGTH)
		return;
	for (i = strLength - 1; i > 0; i--)
	{
		if (fileName[i] == '.')
		{
			fileExtensionLength = strLength - i;
			if (strLength - fileExtensionLength + destExtensionLength >= MAX_FILE_NAME_LENGTH)
				return;
			strncpy(destName, fileName, i);
			strncpy(destName + i, end, destExtensionLength);
			dest = fopen(destName, "wb");
			if (dest == NULL)
			{
				printFileError(ERROR_CREATE_FILE, destName);
				return;
			}
			fclose(dest);
		}
	}
}

/*
Description:
Encoding a command

objFileName - object file name
line - a single line from the assembly file
*/
void encodingCommand(char* line)
{
	int i, opcodeGroup, trimmedLineLength;
	char *temp, *lineBuffer;
	char *sourceOperand;
	char *destOperand;
	char opcodeName[5] = { 0 };/*opcode word size*/
	char label[MAX_LINE_LENGTH] = { 0 };/*the label*/
	temp = sourceOperand = destOperand = NULL;
	if (!isFileExists(objFileName))
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	temp = trimStr(line);
	trimmedLineLength = strlen(temp);
	lineBuffer = (char*)malloc(trimmedLineLength + 2);
	if (!lineBuffer)
	{
		printError(ALLOCATE_MEMORY_ERROR);
		return;
	}
	strncpy(lineBuffer, temp, trimmedLineLength + 1);
	lineBuffer[trimmedLineLength + 1] = '\0';
	opcodeGroup = 0;

	if (isLabel(temp, label))/*case - commmand starts with label*/
	{
		temp = (char*)(temp + strlen(label) + 1);
	}
	while (isspace(*temp))/*ignore whitespaces*/
	{
		temp++;
	}
	strcat(temp, "\n");
	for (i = 0; (*temp) != '\0' && (!isspace(*temp)); i++)
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

	switch (opcodeGroup)
	{
	case FIRST_GROUP:/* two operands*/
	{
		sourceOperand = strtok(temp, ",");/*the operands are separated by a comma*/
		destOperand = strtok(NULL, "");
		destOperand = trimStr(destOperand);
		break;
	}
	case SECOND_GROUP:/*single operand*/
	{
		destOperand = strtok(temp, "");
		destOperand = trimStr(destOperand);
		break;
	}
	case THIRD_GROUP:/*opcode only*/
	{
		break;
	}
	default:
	{
		break;
	}
	}
	makeBinaryCode(opcodeGroup, opcodeName, sourceOperand, destOperand);
	free(lineBuffer);
}
/*
Description:
Returns the opcode group number.

opcodeName - the opcode name in binary
*/
int getOpcodeGroup(char *opcodeName)
{
	int i = 0;
	for (i = 0; i < FIRST_GROUP_SIZE; i++)
	{
		if (strcmp(OpcodesFirstGroup[i], opcodeName) == 0)
			return FIRST_GROUP;
	}
	for (i = 0; i < SECOND_GROUP_SIZE; i++)
	{
		if (strcmp(OpcodesSecondGroup[i], opcodeName) == 0)
			return SECOND_GROUP;
	}
	return THIRD_GROUP;
}
/*
Description:
The executor of line encoding.

opcodeGroup - opcode group number
opcode - opcoda name
sourceOperand -  source Operand
destOperand - destination Operand
*/
void makeBinaryCode(int opcodeGroup, char* opcode, char* sourceOperand, char *destOperand)
{
	int i = 0, j = 0;

	int binaryWord[WORD_SIZE] = { 0 };/*the whole line in binary code*/
	int binaryOpcode[OPCODE_SIZE] = { 0 };
	int binarySourceOperand[OPERAND_SIZE] = { 0 };
	int binaryDestOperand[OPERAND_SIZE] = { 0 };

	if (!isFileExists(objFileName))
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	switch (opcodeGroup)
	{
	case THIRD_GROUP:
	{
		setOpcodeBinaryCode(opcode, binaryOpcode);
		break;
	}
	case SECOND_GROUP:
	{
		setOpcodeBinaryCode(opcode, binaryOpcode);
		makeSingleOperandBinary(destOperand, binaryDestOperand);
		break;
	}
	case FIRST_GROUP:
	{
		setOpcodeBinaryCode(opcode, binaryOpcode);
		makeSingleOperandBinary(sourceOperand, binarySourceOperand);
		makeSingleOperandBinary(destOperand, binaryDestOperand);
		break;
	}
	default: break;
	}
	for (i = 0; i < OPCODE_SIZE; i++, j++)
		binaryWord[j] = binaryOpcode[i];
	for (i = 0; i < OPERAND_SIZE; i++, j++)
		binaryWord[j] = binarySourceOperand[i];
	for (i = 0; i < OPERAND_SIZE; i++, j++)
		binaryWord[j] = binaryDestOperand[i];
	printBinaryWord(binaryWord);/*print the word in 32 weird base*/
	makeOperandsWeird(opcodeGroup, binarySourceOperand, binaryDestOperand, sourceOperand, destOperand);/**/
}
/*
Description:
Converts a single operand to binary for the line encodding .

operand - operand string
binary - binary array
*/
void makeSingleOperandBinary(char *operand, int *binary)
{

	if (isNumOperand(operand))/*addressing type number 1*/
	{
		binary[0] = 0;
		binary[1] = 0;
	}
	else if (isRegister(operand))/*addressing type number 2*/
	{
		binary[0] = 1;
		binary[1] = 1;
	}
	else if (isStructWithDotOperand(operand))/*addressing type number 3*/
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
/*
Description:
Converts a word to weird 32 base and prints its encodding to the object output file.

binaryWord - binary word array
*/
void printBinaryWord(int *binaryWord)
{
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };

	binaryToWierd(binaryWord, weirdWord);/**/
	decimalToWierd(ICounter, WeirdLineCounter);
	objFile = fopen(objFileName, "ab");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
	ICounter++;
	fclose(objFile);
}

/*
Description:
Converts an operand to binary.

operand - opcode name
binary - binary array
*/
void setOpcodeBinaryCode(char *operand, int *binary)
{
	int i = 0;
	for (i = 0; i < NUM_OF_OPCODES; i++)
	{
		if (strcmp(((opcodes[i]).opcodeName), operand) == 0)
		{
			copyBinaryOpcode(i, binary);
			return;
		}
	}
}
/*
Description:
Copy the binary opcode.

index - index of the opcode in the opcodes array
binary - binary array
*/
void copyBinaryOpcode(int index, int *binary)
{
	int i = 0;
	for (i = 0; i < OPCODE_SIZE; i++)
	{
		binary[i] = (opcodes[index].binaryCode)[i];
	}
}

/*
Description:
Prints the source operand and the destination in weird 32 base.

opcodeGroup - opcode group name
binarySourceOperand - source operand group
binaryDestOperand - destination operand group
sourceOperand - source operand string
destOperand - destination operand string
*/
void makeOperandsWeird(int opcodeGroup, int *binarySourceperand, int *binaryDestOperand, char *sourceOperand, char *destOperand)
{
	int sourceAddressingType, destAddressingType;

	if (opcodeGroup == THIRD_GROUP)
		return;

	sourceAddressingType = destAddressingType = 0;

	destAddressingType = getAddressingType(binaryDestOperand);
	if (opcodeGroup == SECOND_GROUP)
	{
		printDestOperand(sourceOperand, destOperand, destAddressingType);
		return;
	}

	/*case - FIRST_GROUP*/
	sourceAddressingType = getAddressingType(binarySourceperand);
	switch (sourceAddressingType)
	{
	case 0:/*first operand is a number*/
	{
		printNumberWeird(objFileName, sourceOperand);
		printDestOperand(sourceOperand, destOperand, destAddressingType);
		break;
	}
	case 1:/*first operand is data*/
	{
		printWeirdDataOperand(sourceOperand);
		printDestOperand(sourceOperand, destOperand, destAddressingType);
		break;
	}
	case 2:/*first operand is struct with dot*/
	{
		printStructWithDotWeird(sourceOperand);
		printDestOperand(sourceOperand, destOperand, destAddressingType);
		break;
	}
	case 3:/*first oeprand is register*/
	{
		if (isRegister(destOperand))
		{
			printRegisterWeird(sourceOperand, destOperand, TRUE, TRUE);
		}
		else
		{
			printRegisterWeird(sourceOperand, destOperand, TRUE, FALSE);
			printDestOperand(sourceOperand, destOperand, destAddressingType);
		}
		break;
	}
	default: break;
	}
}
/*
Description:
Prints a register in weird 32 base.

sourceOperand - source operand string
destOperand - dest operand string
sourceRegister - is source operand register
destRegister - is dest operand register
*/
void printRegisterWeird(char *sourceOperand, char *destOperand, bool sourceRegister, bool destRegister)
{
	int i;
	int binaryWord[WORD_SIZE] = { 0 };
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };

	for (i = 0; i < NUM_OF_REGISTERS; i++)
	{
		if ((sourceRegister) && strcmp(((registersBinary[i]).registerName), sourceOperand) == 0)
		{
			copyBinaryRegister(binaryWord, i, "first");
		}
		if ((destRegister) && strcmp(((registersBinary[i]).registerName), destOperand) == 0)
		{
			copyBinaryRegister(binaryWord, i, "second");
		}
	}
	binaryToWierd(binaryWord, weirdWord);
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, weirdWord);
	ICounter++;

	fclose(objFile);
}
/*
Description:
Copy the binary register code.

binaryWord - binary array
index - index of the register in the registersBinary structure
operandType - "first" or "second"
*/
void copyBinaryRegister(int *binaryWord, int index, char* operandType)
{
	int i;
	int j = 0;
	if ((strcmp(operandType, "first") == 0))
	{
		for (i = 0; i < OPCODE_SIZE; i++, j++)
		{
			binaryWord[i] = (registersBinary[index].address)[j];
		}
	}
	else
	{
		for (i = 4; i < OPCODE_SIZE * 2; i++, j++)
		{
			binaryWord[i] = (registersBinary[index].address)[j];
		}
	}
}
/*
Description:
Prints a "struct with dot" in weird 32 base.

objFileName - object File Name
firstOperand - operand string to print in weird base
*/
void printStructWithDotWeird(char *firstOperand)
{
	int length;
	char *token, *temp;
	length = strlen(firstOperand);
	temp = (char*)malloc(length + 1);
	if (temp == NULL)
	{
		printError(ALLOCATE_MEMORY_ERROR);
		return;
	}
	strncpy(temp, firstOperand, length + 1);
	token = strtok(temp, ".");/*the label name example: s.1 -> temp = s*/
	printWeirdDataOperand(token);
	token = strtok(NULL, ""); /*s.1 -> temp = 1*/
	printNumberWeird(objFileName, token);
	free(temp);
}
/*
Description:
Prints a number in weird 32 base.

objFileName - object File Name
operand - operand string to print in weird base
*/
void printNumberWeird(char* objFileName, char *firstOperand)
{
	int number = getNumber(firstOperand);
	int binaryWord[WORD_SIZE] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };
	char NumberWeird[MAX_32_WEIRD_LENGTH] = { 0 };

	decimalToBinary(number, binaryWord, EIGHT_BITS);
	binaryToWierd(binaryWord, NumberWeird);
	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}
	fprintf(objFile, "%s	%s\n", WeirdLineCounter, NumberWeird);
	ICounter++;
	fclose(objFile);
}
/*
Description:
Prints the second opernd in weird 32 base.

sourceOperand - first operand string
destOperand - desination operand string
destAddressingType - desination operand addressing type
*/
void printDestOperand(char *sourceOperand, char *destOperand, int destAddressingType)
{
	switch (destAddressingType)
	{
	case 0:
	{
		printNumberWeird(objFileName, destOperand);
		break;
	}
	case 1:/*second operand is data*/
	{
		printWeirdDataOperand(destOperand);
		break;
	}
	case 2:/*second operand is struct with dot*/
	{
		printStructWithDotWeird(destOperand);
		break;
	}
	case 3:/*second operand is a register*/
	{
		printRegisterWeird(sourceOperand, destOperand, FALSE, TRUE);
		break;
	}
	default: break;
	}

}
/*
Description:
Prints a  data opernd in weird 32 base.

objFileName - object File Name
operand - operand string
*/
void printWeirdDataOperand(char *operand)
{
	int addressNum;
	symbolPtr searchLabel = symbolListHead;/*search is an hendle to the head of the symbols list*/
	int binaryWord[WORD_SIZE] = { 0 };
	char weirdWord[MAX_32_WEIRD_LENGTH] = { 0 };
	char WeirdLineCounter[MAX_32_WEIRD_LENGTH] = { 0 };

	decimalToWierd(ICounter, WeirdLineCounter);

	objFile = fopen(objFileName, "ab");
	if (!objFile)
	{
		printFileError(OPEN_FILE_ERROR, objFileName);
		return;
	}

	while (searchLabel)
	{
		if (strcmp((searchLabel->name), operand) == 0) /*find the node that hands that label*/
		{
			decimalToBinary(addressNum = (searchLabel->address), binaryWord, EIGHT_BITS);/*convert the label address to "32 weird base"*/
			if ((searchLabel->isExternal) == TRUE)/*ARE*/
			{
				binaryWord[8] = 0;
				binaryWord[9] = 1;
				printToExternFile(operand);
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
/*
Description:
Prints To extern File

operand - operand string
*/
void printToExternFile(char *operand)
{
	char weirdAddress[MAX_32_WEIRD_LENGTH] = { 0 };
	if (!extFlag)
	{
		createFile(assemblyFileName, extFile, extFileName, EXTERN_FILE_END);/* create the object output file */
		extFlag = TRUE;
	}
	extFile = fopen(extFileName, "ab");
	if (!extFile)
	{
		printFileError(OPEN_FILE_ERROR, extFileName);
	}
	decimalToWierd(ICounter, weirdAddress);
	fprintf(extFile, "%s	%s\n", operand, weirdAddress);
	fclose(extFile);
}


/*
Description:
Returns an operand addressing type

binaryOperand - binary operand
*/
int getAddressingType(int* binaryOperand)
{
	int res = 0;
	res = 2 * binaryOperand[0] + binaryOperand[1];
	return res;
}
