#ifndef DATA_LIST_H
#define DATA_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "globalVariables.h"
#include "errors.h"



typedef enum {
	positiveNumber,
	negativeNumber,
	character,
	eof
} dataType;

typedef struct dataTableNode* dataPtr;

typedef struct dataTableNode {
	int dataCounter;
	dataType type;
	int asciiCode;
	dataPtr next;
}dataTableNode;

int addStringToData(dataPtr*, dataPtr*, char*, long);

bool addToDataList(dataPtr *head, dataPtr *tail, int dc, int dType, int ascii);

void addNodeToDataList(dataPtr*, dataPtr*, dataPtr*);

void freeDataList(dataPtr*);

void printDataList(dataPtr*);

#endif