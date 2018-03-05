

typedef enum {
	positiveNumber,
	negativeNumber,
	character,
	eof
} dataType;

typedef struct dataTableNode * dataPtr;

typedef struct {
	int dataCounter;
	dataType type;
	int asciiCode;
	dataPtr next;
}dataTableNode;


int addTodataList(int, int, int);

void addNodeToDataList(dataPtr, ptr *, ptr *);

void freeList(dataPtr *head)
