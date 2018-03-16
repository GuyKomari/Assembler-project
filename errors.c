#include "errors.h"

void printFileError(char* error, char* fileName)
{
	fprintf(stderr, "%s %s %s\n", "[Error] -> ", error, fileName);
}

void printError(char* error)
{
	fprintf(stderr, "[ERROR IN LINE - %d] --> %s\n", lineCounter, error);
}

void printWarning(char* warn)
{
	fprintf(stderr, "%s %s\n", "[Warning] -> ", warn);
}
