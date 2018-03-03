/*is token a register*/
int isRegister(char *token);

/*is token a valid opcode*/
int isOpcode(char *token);


/*
Description: is token a valid label
Responsible: GIL
*/
bool isLabel(char*);

/*
Description: is label already defined
Responsible: GUY
*/
int isLabelDefined(char* token);

/*
Description: is token a struct
Responsible: GIL
*/
bool isDataCommand(char*);

/*
Description:
Responsible: GUY
*/
bool isEntry(char*);

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
Description: trim whitespace from str
Responsible: GUY
*/
char *trimStr(char *str);

/*trim whitespace on right end of str*/
char *trimRightStr(char *str);

/*trim whitespaces on left end of str*/
char *trimLeftStr(char *str);

/*returns the sentence in sentence i*/
char *getSentence(File * file, int i);

/*
Description: convert decimal code to binary code
Responsible: GIL
*/
bool decimalToBinary(char *code, char* dest);

/*
Description: convert binary code to "wierd 32 base"
Responsible: GUY
*/
char *binaryToWierd(char *code)

/*
TODO: to see if it's necessary
Responsible: GIL
*/
int isFirstGroup(opcodeStructure opcode);

int isSecondGroup(opcodeStructure opcode);

int isThirdGroup(opcode_structure opcode);



/*char* getLabelNameFromLine(char*);*/



bool isFileExists(char*);

