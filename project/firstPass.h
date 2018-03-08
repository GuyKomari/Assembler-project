#include <stdio.h>
#include <string.h>
#include "GlobalFunctions.h"
#include "globalVariables.h"
#include "symbolList.h"
#include "dataList.h"
#include "errors.h"

/*
scanning line and sending it to the proper function in the "parser" module.
*/

void firstpass(char* filename);

/*Responsible: GIL*/
bool readLine(FILE*, char*);

bool ParseData(dataPtr, dataPtr ,char *);

bool externLabels(char *line);

bool parseCommand(char *line);


