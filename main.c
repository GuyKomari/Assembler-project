#include "main.h"

int main(int argc, char** argv)
{
	int i;
	i = 0;
	if(argc < 2)
		printError(MISSING_INPUT_FILE);
	for (i = 1; i < argc; i++)/*for each input file, run the assembler*/
	{
		runAssembler(argv[i]);
	}
	return 0;
}