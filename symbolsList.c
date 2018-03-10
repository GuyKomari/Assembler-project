#include "symbolsList.h"

bool addToSymbolsList(symbolPtr *head ,symbolPtr *tail ,char* symbol, int addr, bool isEx , bool isC , bool isD, bool isEn)
{
	symbolPtr temp;
	(symbolsTableNode*)temp = (symbolsTableNode*)(malloc(sizeof(symbolsTableNode)));
	if(!temp)
	{
		fprintf(stderr , "cannot allocate memory in line %d for label %s \n",addr ,symbol);
		return FALSE;
	}
	temp->name = symbol;
	temp->address = addr;
	temp->isExternal = isEx;
	temp->isCommand = isC;
	temp->isData = isD;
	temp->isEntry = isEn;
	temp->next = NULL;
	addNodeToSymbolList(temp , head , tail);
	return TRUE;
}


void addNodeToSymbolList(symbolPtr temp, symbolPtr *head, symbolPtr *tail)
{
	if(*head == NULL)/*case- empty list*/
	{
		*head = temp;
		*tail = temp;
		temp->next = NULL;
		return;
	}
	else
	{
		(*tail)->next=temp;
		*tail=temp;
		temp->next = NULL;
		return;
	}
}

void updateDataSymbols(symbolPtr *head, int ic)
{
	symbolPtr temp = *head;
	while(temp)
	{
		if(temp->isData)
			temp->address+=ic;
	}
}

void freeList(symbolPtr *head)
{
	symbolPtr temp;
	while(*head)
	{
		temp=*head;
		*head=(*head)->next;
		free(temp);
	}
}