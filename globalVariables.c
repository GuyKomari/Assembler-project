#include "globalVariables.h"

/*
TODO:
1. Move everything to the header file??
*/








const ERAStructure ERATypes ={
	{	0	,	Absulote	,	"00"	},
	{	1	,	External	,	"01"	},
	{	2	,	Relocatable	,	"10"	}
};

const char* DataSymbols[] = {".struct",".data",".string"};