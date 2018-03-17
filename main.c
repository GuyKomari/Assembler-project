#include "main.h"

int main(int argc, char** argv)
{
	int i = 0;

	if (argc < 2)
	{
		printError(MISSING_INPUT_FILE);
	}
	for (i = 1; i < argc; i++)
	{
		runAssembler(argv[i]);
	}
	return 0;
}
