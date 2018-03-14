#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include "globalVariables.h"
#include "GlobalsFunctions.h"


#define MAX_FILE_NAME 50

#define ENTRY_FILE_END ".ent"

#define EXTERN_FILE_END ".ext"

#define OBJECT_FILE_END ".ob"

#define SINGLE_OPERAND 1

#define TWO_OPERANDS 2

#define THREE_OPERANDS 3

#define FIRST_GROUP 1

#define SECOND_GROUP 2

#define THIRD_GROUP 3

#define FIRST_GROUP_SIZE 5

#define SECOND_GROUP_SIZE 9

#define THIRD_GROUP_SIZE 2

#define EIGHT_BITS 8

#define TEN_BITS 10

bool secondPass(char*);

void createFile(char* , FILE* ,char*, char*);

void printToEntryFile(char* , char*);

void encodingCommand(char* , char*);



#endif
