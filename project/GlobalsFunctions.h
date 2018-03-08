/*is token a valid register*/
bool isRegister(char *token);

/*is token a valid opcode*/
bool isOpcode(char *token);

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
int isLabelDefined(char* token);

/*
Description: is token a struct
Get a line to check if has a data defenition - .string | .data | .struct ,
if true - return true and insert the data to the second string
Responsible: GIL
*/
bool isDataCommand(char*,char*);

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
int isEmptySentence(const char* token);



/*
Description: is token comment sentence
Responsible: GUY
*/
int isComment(const char* token);

/*
is token a declaration - .data or .string or .extern or .entry or .struct
if true return the type declaration otherwise -1 
*/
int isDeclaration(const char* token);


/*
Description: is token a keyword
Responsible: GIL
*/
int isKeyword(const char* token);

/*
TODO:
1. change the string functions (functions that handles strings) to (char* target, char* dest) type.
Instead of returning string.
*/


/*
TODO: to see if it's necessary
Responsible: GIL
*/
int isFirstGroup(opcodeStructure opcode);

int isSecondGroup(opcodeStructure opcode);

int isThirdGroup(opcode_structure opcode);

/*char* getLabelNameFromLine(char*);*/

bool isFileExists(char*);


/*
Description: get a line of data and return the label name if exists, if not, return NULL.
Responsible: GIL
*/
bool getLabel(char* data, char* dest);


/*
Description: get a line of data and return the data Symbol type if exists (.struct, .string
or .data), if not, return NULL.
Responsible: GIL
*/
bool getSymbol(char* data, char* dest);


int addStringToData(dataPtr dataListHead, dataPtr dataListTail, char *str, long dc);


/*
TODO: change the string functions (functions that handles strings) to (char* target, char* dest) type
Description: trim whitespace from str
Responsible: GUY
*/
char *trimStr(char *str);

/*trim whitespace on right end of str*/
char *trimRightStr(char *str);

/*trim whitespaces on left end of str*/
char *trimLeftStr(char *str);


/*
Description: convert decimal code to binary code
Responsible: GUY
*/
bool decimalToBinary(int n, int binaryNum[], int arrSize)

/*
Description: convert binary code to "wierd 32 base"
Responsible: GUY
*/
bool binaryToWierd (int binary[], char* res)

/*
Description: convert a number in decimal base to "wierd 32 base"
Responsible: GUY
*/
bool decimalToBinary(int, int binaryNum[], int arrSize)


