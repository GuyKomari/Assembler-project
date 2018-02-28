#include <stdio.h>
#include <string.h>
/*

*/


/*
scanning line and sending it to the proper function in the "parser" module.
*/

int readLine(File * fp);

int isFirstVarLabel(char* line);

/*.struct / .data / .string*/
int isDataSymbol(char* token);

void insertToSymbolTable(char* token);

int isSymbolExist(char* token);