#include "main.h"
#include "assembler.h"

int main(int argc, char** argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		runAssembler(argv[i]);
	}
	return 0;
}