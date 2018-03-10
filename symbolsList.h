#include <stdio.h>

typedef struct symbolsTableNode * symbolPtr;

typedef struct symbolsTableNode {
	char* name;
	int address;
	bool isExternal;
	bool isCommand;
	bool isData;
	bool isEntry;
	symbolPtr next;
} symbolsTableNode;


/*addToSymbolsList (label name, address, isExternal, isCommand, isData, isEntry)*/
bool addToSymbolsList(symbolPtr * ,symbolPtr *, char*, int, bool, bool, bool, bool);

void addNodeToSymbolList(ptr, ptr *, ptr *);

void freeList(ptr *head)

/*phase 17 in the first pass*/
void updateDataSymbols(int);

