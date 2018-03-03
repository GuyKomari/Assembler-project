#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "errors.h"

void printError(char* error)
{
	fprintf(stderr, "%s%s\n", "[Error] -> ", error);
}