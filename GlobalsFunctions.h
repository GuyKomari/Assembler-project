#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dataList.h"
#include "symbolsList.h"
#include "globalVariables.h"
#include "errors.h"


/*is token a valid register*/
bool isRegister(char*);

/*is token a valid opcode*/
bool isOpcode(char*);

/*TODO: implement these 3 functions that gets a single word and tells if it's a label or numOperand
Responsible: GIL
*/

bool isValidLabel(char*);

bool isNumOperand(char*);

bool isStructWithDotOperand(char*);

AddressingMode getOperandAddressing(char*);

/*
Description: is token a valid label 
Get a line to check if has a label defenition,
if true - return true and insert the label to the destination string
Responsible: GIL
*/
bool isLabel(char*,char*);

/*
Description: gets a valid label and checks if this label already defined
Responsible: GUY
*/
bool isLabelDefined(symbolPtr*, char*);


/*
Description:
Responsible: GUY
*/
bool isEntry(char*);



bool isExtern(char*);


/*
Description: is token empty sentence
Responsible: GUY
*/
bool isEmptySentence(char*);



/*
Description: is token comment sentence
Responsible: GUY
*/
bool isComment(char*);

/*
is token a declaration - .data or .string or .extern or .entry or .struct
if true return the type declaration otherwise -1 
*/
bool isDateDeclaration(char*);


/*
Description: is token a keyword
Responsible: GIL
*/
bool isKeyword(char*);



/*char* getLabelNameFromLine(char*);*/

bool isFileExists(char*);


/*
Adds a number to data List
*/
bool addNumberToDataList(dataPtr*, dataPtr*, int, int);


/*
Description: get a line of data and return the data Symbol type if exists (.struct, .string
or .data), if not, return NULL.
Responsible: GIL
*/
bool getSymbol(char*, char*);


/*
TODO: change the string functions (functions that handles strings) to (char* target, char* dest) type
Description: trim whitespace from str
Responsible: GUY
*/
char *trimStr(char*);

/*trim whitespace on right end of str*/
char *trimRightStr(char*);

/*trim whitespaces on left end of str*/
char *trimLeftStr(char*);


/*
Description: gets a parsed command (i.e "mov r1, r3") without any spaces or unnecessary characters.
returns the number that IC should be incremented with.
returns 0 when failed.
Responsible: GIL
*/
int getCommandSize(char*);

/*
Description: convert decimal code to binary code
Responsible: GUY
*/
void decimalToBinary(int , int *, int );

/*
Description: convert binary code to "wierd 32 base"
Responsible: GUY
*/
void binaryToWierd(int* binary, char* res);

/*
Description: convert a number in decimal base to "wierd 32 base"
Responsible: GUY
*/

void decimalToWierd(int num, char* res);

int binaryToDecimal(int *);



bool isDataCommand(char* );

bool isNumber(char* );

bool isKeyword(char*);


int getNumber(char*);

bool readLine(FILE*, char*);



#endif
