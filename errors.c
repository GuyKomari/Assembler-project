#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "errors.h"

void printError(char* error)
{
	fprintf(stderr, "%s %s\n", "[Error] -> ", error);
}

void printWarning(char* warn)
{
	fprintf(stderr, "%s %s\n", "[Warning] -> ", warn);
}