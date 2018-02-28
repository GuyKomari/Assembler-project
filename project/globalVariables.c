#include "globalVariables.h"

int IC = IC_START;

int DC = DC_START;

int PSU = 0; 

bol errorFlag = FALSE;

const char WIERD_32_BASE [BASE_LENGTH] = {
'!','@','#','$','%','^','&','*',
'<','>','a','b','c','d','e','f',
'g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v'	};

const opcodeStructure opcodes = {
	{	0	,	"mov"	,	"0000"	,	FIRST_GROUP		}			
	{	1	,	"cmp"	,	"0001"	,	FIRST_GROUP		}	
	{	2	,	"add"	,	"0010"	,	FIRST_GROUP		}	
	{	3	,	"sub"	,	"0011"	,	FIRST_GROUP		}		
	{	4	,	"not"	,	"0100"	,	SECOND_GROUP	}	
	{	5	,	"clr"	,	"0101"	,	SECOND_GROUP	}	
	{	6	,	"lea"	,	"0110"	,	FIRST_GROUP		}	
	{	7	,	"inc"	,	"0111"	,	SECOND_GROUP	}	
	{	8	,	"dec"	,	"1000"	,	SECOND_GROUP	}	
	{	9	,	"jmp"	,	"1001"	,	SECOND_GROUP	}	
	{	10	,	"bne"	,	"1010"	,	SECOND_GROUP	}	
	{	11	,	"red"	,	"1011"	,	SECOND_GROUP	}	
	{	12	,	"prn"	,	"1100"	,	SECOND_GROUP	}	
	{	13	,	"jsr"	,	"1101"	,	SECOND_GROUP	}
	{	14	,	"rts"	,	"1110"	,	THIRD_GROUP		}	
	{	15	,	"stop"	,	"1111"	,	THIRD_GROUP		}	
};

const char* registers[NUM_OF_REGISTERS] = {"r0","r1","r2","r3","r4","r5","r6","r7"};


const char* Keywords[] = {"mov","cmp","add","sub","not","clr","lea","dec","jmp","bne","red","prn","jsr","rts","stop",
						  "r0","r1","r2","r3","r4","r5","r6","r7",
						  ".data",".string",".extern",".entry",".struct"};

const ERAStructure ERATypes ={
	{	0	,	Absulote	,	"00"	},
	{	1	,	External	,	"01"	},
	{	2	,	Relocatable	,	"10"	}
};

const char* DataSymbols[] = {".struct",".data",".string"};