#include <stdio.h>
#ifndef global
#include "globalVariables.h"
#endif


typedef enum {
	positiveNumber,
	negativeNumber,
	character,
	eof
} dataType;

typedef struct dataTableNode* dataPtr;

typedef struct {
	int dataCounter;
	dataType type;
	int asciiCode;
	dataPtr next;
}dataTableNode;


int addToDataList(dataPtr *head, dataPtr *tail, int dc, int dType, int ascii);

void addNodeToDataList(dataPtr, dataPtr*, dataPtr*);

void freeList(dataPtr*);