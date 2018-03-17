/*
Authors:
Gil Mansharov
Guy Komari
*/

#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "globalVariables.h"
#include "GlobalsFunctions.h"


/*
functions used for the second pass module
*/

bool secondPass(char*);

void createFile(char*, FILE*, char*, char*);

void printToEntryFile(char*);

int getOpcodeGroup(char*);

int getAddressingType(int*);

void encodingCommand(char*);

void makeBinaryCode(int, char*, char*, char *);

void makeSingleOperandBinary(char *, int *);

void printToExternFile(char *);

void printIcAndDCWeird();

void printBinaryWord(int *);

void setOpcodeBinaryCode(char *, int *);

void copyBinaryOpcode(int, int *);

void makeOperandsWeird(int, int *, int *, char *, char *);

void printRegisterWeird(char *, char *, bool, bool);

void copyBinaryRegister(int *, int, char *);

void printStructWithDotWeird(char *);

void printNumberWeird(char*, char *);

void printDestOperand(char *, char *, int);

void printWeirdDataOperand(char *);

void printDataWeird();

bool LabelDeclaredButNotDefined(char *);

#endif