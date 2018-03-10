#include "errors.h"

void printError(char* error)
{
	fprintf(stderr, "%s%s\n", "[Error] -> ", error);
}