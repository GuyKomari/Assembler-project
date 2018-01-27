#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

int main(char** args, int argc)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (exists(......))
		{
			doassembly(args[i]);
		}
	}
}