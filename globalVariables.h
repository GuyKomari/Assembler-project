#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H


/*
Here are all the global variables
*/

/*
TODO:
structures and variables for the second pass
*/

#define REGISTER_SIZE 10

#define ERACODE_SIZE	2

#define MEMORY_SIZE 256

#define WORD_SIZE 10

#define OPCODE_SIZE 4

#define BASE_LENGTH 32

#define OPERAND_SIZE 2

#define ENCODING_SIZE 2

#define OPCODE_SIZE 4

#define MAX_LINE_LENGTH 80

#define NUM_OF_REGISTERS 8

#define NUM_OF_OPCODES 16

#define IC_START 100

#define DC_START 0

#define FIRST_GROUP 1

#define SECOND_GROUP 2

#define THIRD_GROUP 3

/*Minimum machine instructions*/
#define MIN_ROWS 1

/*maximum machine instructions*/
#define MAX_ROWS 5


#define ENTRY_LENGTH 6

#define EXTERN_LENGTH 7

#define MULT 16

#define MAX10BITS 1024

#define MAX8BITS 256

#define NUM_OF_KEYWORDS	21

#define uint unsigned int

typedef enum {FALSE, TRUE} bool;

typedef enum {IMMEDIATE, DIRECT_MEMORY, STRUCT_ACCESS, DIRECT_REGISTER} AddressingMode;

const char* WIERD_32_BASE[BASE_LENGTH] = {
	"!","@","#","$","%","^","&","*",
	"<",">","a","b","c","d","e","f",
	"g","h","i","j","k","l","m","n",
	"o","p","q","r","s","t","u","v" };

const char* Keywords[] = { "mov","cmp","add","sub","not","clr","inc", "lea","dec","jmp","bne","red","prn","jsr","rts","stop",
".data", ".string", ".extern", ".entry", ".struct" };

const char* Registers[NUM_OF_REGISTERS] = { "r0","r1","r2","r3","r4","r5","r6","r7" };
/*
typedef enum {
	mov,
	cmp,
	add,
	sub,
	not,
	clr,
	lea,
	inc,
	dec,
	jmp,
	bne,
	red,
	prn,
	jsr,
	rts,
	stop
} opcodeValue;

typedef enum {
	emptySentence,
	comment,
	declaration,
	command
}sentenceType;
*/

typedef enum {
	Absolute,
	External,
	Relocatable
} encodingType;

typedef struct ERAStructure
{
	uint num;
	encodingType ERAName;
	char* binaryVal;
}ERAStructure;


const ERAStructure ERATypes[] = {
	{ 0	,	Absolute	,	"00" },
	{ 1	,	External	,	"01" },
	{ 2	,	Relocatable	,	"10" }
};


typedef struct opcode_structure
{
	int DecimalCode;
	char* opcodeName;
	int binaryCode[WORD_SIZE];
	int group;
} opcodeStructure;


typedef struct word 
{
	int opcode[OPCODE_SIZE];
	int sourceOperand[OPERAND_SIZE];
	int ERAcode[ERACODE_SIZE];
} word;


const opcodeStructure opcodes[NUM_OF_OPCODES] = {
	{ 0	,	"mov"	, { 0,0,0,0 }	,	FIRST_GROUP },
	{ 1	,	"cmp"	, { 0,0,0,1 }	,	FIRST_GROUP },
	{ 2	,	"add"	, { 0,0,1,0 }	,	FIRST_GROUP },
	{ 3	,	"sub"	, { 0,0,1,1 }	,	FIRST_GROUP },
	{ 4	,	"not"	, { 0,1,0,0 }	,	SECOND_GROUP },
	{ 5	,	"clr"	, { 0,1,0,1 }	,	SECOND_GROUP },
	{ 6	,	"lea"	, { 0,1,1,0 }	,	FIRST_GROUP },
	{ 7	,	"inc"	, { 0,1,1,1 }	,	SECOND_GROUP },
	{ 8	,	"dec"	, { 1,0,0,0 }	,	SECOND_GROUP },
	{ 9	,	"jmp"	, { 1,0,0,1 }	,	SECOND_GROUP },
	{ 10	,	"bne"	, { 1,0,1,0 }	,	SECOND_GROUP },
	{ 11	,	"red"	, { 1,0,1,1 }	,	SECOND_GROUP },
	{ 12	,	"prn"	, { 1,1,0,0 }	,	SECOND_GROUP },
	{ 13	,	"jsr"	, { 1,1,0,1 }	,	SECOND_GROUP },
	{ 14	,	"rts"	, { 1,1,1,0 }	,	THIRD_GROUP },
	{ 15	,	"stop"	, { 1,1,1,1 }	,	THIRD_GROUP }
};


/*
typedef struct sentence{
	int id;
	char* sentenceStr;
	int rowsNeeded;  
	word wordByte;
	bol hasError;
	char* errorDescription;
}sentence;


typedef struct cmdTable{
	char* labelName;
	int decimalAddress;
	char* base32Address;
	char* operation;
	char* opperand;
	char* binaryCode;
} cmdTable;
*/


#endif