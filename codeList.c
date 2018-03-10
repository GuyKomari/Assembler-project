#include "list.h"

int addToCodeList(codePtr *head ,codePtr *tail ,int ic, char * weirdBase, int *binary)
{
	codePtr temp;
	temp = (codeTableNode)(malloc(sizeof(codeTableNode)));
	if(!temp)
	{
		fprintf(stderr , "cannot allocate memory in line - %d for the data - %s \n",num , ch);
		return FALSE;
	}
	temp->address = ic;
	temp->base32Address = weirdBase;
	temp->binaryCode = binary;
	temp->next = NULL;
	addNodeToCodeList(temp , head , tail);
}


void addNodeToCodeList(codePtr temp, codePtr *head, codePtr *tail)
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


void freeList(codePtr *head)
{
	codePtr temp;
	while(*head)
	{
		temp=*head;
		*head=(*head)->next;
		free(temp);
	}
}