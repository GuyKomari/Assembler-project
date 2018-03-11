#ifndef DATA_LIST_H
#define DATA_LIST_H
#include <stdio.h>
#include "globalVariables.h"


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


int addToDataList(dataPtr *head, dataPtr *tail, int dc, int dType, int ascii);

void addNodeToDataList(dataPtr, dataPtr*, dataPtr*);

void freeList(dataPtr*);

#endif