#include <stdio.h>

typedef struct symbolsTableNode * symbolPtr;

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
bool addToSymbolsList(symbolPtr * ,symbolPtr *, char*, int, bool, bool, bool, bool);

void addNodeToSymbolList(symbolPtr, symbolPtr*, symbolPtr*);

void freeList(symbolPtr *head);

/*phase 17 in the first pass*/
void updateDataSymbols(int);

