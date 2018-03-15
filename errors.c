#include "errors.h"

void printFileError(char* error, char* fileName)
{
	fprintf(stderr, "%s %s\n", "[Error] -> ", error, fileName);
}


void printError(char* error)
{
	fprintf(stderr, "[IN LINE - %d] %s %s\n", lineCounter, "[Error] -> ", error);
}

void printWarning(char* warn)
{
	fprintf(stderr, "%s %s\n", "[Warning] -> ", warn);
}
