#include "errors.h"

void printFileError(char* error, char* fileName)
{
	fprintf(stderr, "%s %s %s\n", "[Error] -> ", error, fileName);
}


void printError(char* error)
{
	fprintf(stderr, "[IN LINE - %d] %s %s\n", lineCounter, "[Error] -> ", error);
}

void printWarning(char* warn, char* ref)
{
	fprintf(stderr, "%s %s %s\n", "[Warning] \n-> ", warn, ref);
}