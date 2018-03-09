#include "list.h"

bool addToSymbolsList(symbolPtr *head ,symbolPtr *tail ,char* symbol, int num, bool isEx , bool isC , bool isD, bool isEn)
{
	symbolPtr temp;
	bool isValidLabel = isLabelDefined(head, symbol);
	if(!isValidLabel)
	{
		fprintf(stderr,"the label declaration in line %d is invalid", num);
		return FALSE;
	}
	temp = (symbolPtr)(malloc(sizeof(symbolsTableNode)));
	if(!temp)
	{
		fprintf(stderr , "cannot allocate memory in line %d for label %s \n",num ,symbol);
		return FALSE;
	}
	temp->name = symbol;
	temp->address = num;
	temp->isExternal = isEx;
	temp->isCommand = isC;
	temp->.isData = isD;
	temp->.isEntry = isEn;
	temp->next = NULL;
	addNodeToSymbolList(temp , head , tail);
}


void addNodeToSymbolList(symbolPtr temp, symbolPtr *head, symbolPtr *tail)
{
	if(*head==NULL)/*case- empty list*/
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