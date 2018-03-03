#include <stdio.h>


/*
typedef enum {
	isCommand,
	isExternal
} commandType;
*/

typedef struct symbolsTable
{
	char* name;
	uint address;
	bool isExternal;
	bool isCommand;
	bool isData;
	bool isEntry;
} symbolsTable;

/*
TODO:
1. add prev ?
*/

typedef struct symbolsTableNode {
	symbolsTable data;
	symbolsTableNode* next;
} symbolsTableNode;


typedef struct finalTable {
	char* labelName;
	uint decimalAddress;
	char* base32Address;
	char* operation;

} finalTable;


/*addToSymbolsList (line, address, isExternal, isCommand, isData)*/
bool addToSymbolsList(char*, uint, bool, bool, bool);

/*phase 17 in the first pass*/
void updateDataSymbols(int);