#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GlobalsFunctions.h"
#include "symbolsList.h"
#include "errors.h"


/*
scanning line and sending it to the proper function in the "parser" module.
*/

bool firstpass(char*);

bool readLine(FILE*, char*);

bool ParseData(dataPtr*, dataPtr* ,char *);

bool externLabels(char *line);

bool parseCommand(char *line);


#endif