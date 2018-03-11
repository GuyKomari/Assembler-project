#include "dataList.h"

int addToDataList(dataPtr *head ,dataPtr *tail ,int dc, int dType, int ascii)
{
	dataPtr temp;
	temp = (dataPtr)(malloc(sizeof(dataTableNode)));
	if(!temp)
	{
		printError("cannot allocate memory for the data");
		return FALSE;
	}
	((dataTableNode*)temp)->dataCounter = dc;
	((dataTableNode*)temp)->type = dType;
	((dataTableNode*)temp)->asciiCode = ascii;
	((dataTableNode*)temp)->next = NULL;
	addNodeToDataList(temp , head , tail);
}


void addNodeToDataList(dataPtr temp, dataPtr *head, dataPtr *tail)
{
	if(*head==NULL)/*case- empty list*/
	{
		*head = temp;
		*tail = temp;
		((dataTableNode*)temp)->next = NULL;
		return;
	}
	else
	{
		(*((dataTableNode**)tail))->next=temp;
		*tail=temp;
		((dataTableNode*)temp)->next = NULL;
		return;
	}
}


void freeList(dataPtr *head)
{

	dataPtr* temp;
	while(*head)
	{
		temp = *head;

		*head = (*head)->next;
		free(temp);
	}
}