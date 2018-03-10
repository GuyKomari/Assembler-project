
typedef struct codeTableNode * codePtr;

typedef struct {
	int address;
	char* base32Address;
	int *binaryCode;
	codePtr next;
}codeTableNode;


int addToCodeList(int, char *, int *);

void addNodeToCodeList(codePtr, codePtr *, codePtr *);

void freeList(codePtr *head)
