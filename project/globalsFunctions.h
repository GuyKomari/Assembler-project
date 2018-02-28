

/*is token a register*/
int isRegister(const char *token);

/*is token a valid opcode*/
int isOpcode(const char *token);

/*is token a valid label*/
int isLabel(const char* token);

/*is label already defined*/
int isLabelDefined(const char* token);

/*is token a struct*/
int isStruct(const char* token);

/*is token empty sentence*/
int isEmptySentence(const char* token);

/*is token a new sentence*/
int isNewSentence(const char* token)

/*is token comment sentence*/
int isComment(const char* token);

/*
is token a declaration - .data or .string or .extern or .entry or .struct
if true return the type declaration otherwise -1 
*/
int isDeclaration(const char* token);

/*
is token a comand sentence - mov,add....
if true return the type of command otherwise -1 
*/
int isComandSentence(const char* token);

/*is token a keyword*/
int isKeyword(const char* token);

/*trim whitespace from str*/
char *trimStr(char *str);

/*trim whitespace on right end of str*/
char *trimRightStr(char *str);

/*trim whitespaces on left end of str*/
char *trimLeftStr(char *str);

/*returns the sentence in sentence i*/
char *getSentence(File * file, int i);

/*convert decimal code to binary code*/
char *decimalToBinary(char *code)

/*convert binary code to "wierd 32 base"*/
char *binaryToWierd(char *code)

int isFirstGroup(opcodeStructure opcode);

int isSecondGroup(opcodeStructure opcode);

int isThirdGroup(opcode_structure opcode);

