#ifndef SYMBOLS_LIST_H
#define	SYMBOLS_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "globalVariables.h"


typedef struct symbolsTableNode* symbolPtr;

typedef struct symbolsTableNode {
	char* name;
	uint address;
	bool isExternal;
	bool isCommand;
	bool isData;
	bool isEntry;
	symbolPtr next;
} symbolsTableNode;


/*addToSymbolsList (label name, address, isExternal, isCommand, isData, isEntry)*/
bool addToSymbolsList(symbolPtr* ,symbolPtr*, char*, int, bool, bool, bool, bool);

void addNodeToSymbolList(symbolPtr, symbolPtr*, symbolPtr*);

void freeSymbolsList(symbolPtr *head);

/*phase 17 in the first pass*/
void updateDataSymbols(symbolPtr*, int);

void printSymbolsList(symbolPtr *);

#endif
