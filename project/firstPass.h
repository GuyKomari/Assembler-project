#include <stdio.h>
#include <string.h>
#include "globalVariables.h"
#include "list.h"

/*

*/


/*
scanning line and sending it to the proper function in the "parser" module.
*/

void firstpass(char* filename);

/*Responsible: GIL*/
bool readLine(FILE*, char*);

/*.struct / .data / .string*/
bool isDataSymbol(char*);

void insertToSymbolTable(char*);

bool isSymbolExist(char*);