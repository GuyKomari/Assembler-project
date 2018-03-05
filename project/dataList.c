#include "list.h"

int addToDataList(dataPtr *head ,dataPtr *tail ,int dc, int dType, int ascii)
{
	dataPtr temp;
	temp = (dataTableNode)(malloc(sizeof(dataTableNode)));
	if(!temp)
	{
		fprintf(stderr , "cannot allocate memory in line - %d for the data - %s \n",num , ch);
		return FALSE;
	}
	temp->dataCounter = dc;
	temp->address = dType;
	temp->asciiCode = ascii;
	temp->next = NULL;
	addToList(temp , head , tail);
}


void addNodeToDataList(dataPtr temp, dataPtr *head, dataPtr *tail)
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


void freeList(dataPtr *head)
{
	dataPtr temp;
	while(*head)
	{
		temp=*head;
		*head=(*head)->next;
		free(temp);
	}
}