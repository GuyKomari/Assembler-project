/*
Authors:
Gil Mansharov
Guy Komari
*/

#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#define REGISTER_SIZE 10

#define ERACODE_SIZE	2

#define MEMORY_SIZE 256

#define ERA_TYPES_SIZE	3

#define WORD_SIZE 10

#define OPCODE_SIZE 4

#define REGISTER_ADDRESS_SIZE	4

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

#define MAX_32_WEIRD_LENGTH 5

#define MULT 16

#define MAX10BITS 1024

#define MAX8BITS 256

#define NUM_OF_KEYWORDS	21

#define uint unsigned int

#define FIRST_GROUP_SIZE 5

#define SECOND_GROUP_SIZE 9

#define THIRD_GROUP_SIZE 2

#define MAX_LABEL_SIZE 31




typedef enum { FALSE, TRUE } bool;

typedef enum { IMMEDIATE, DIRECT_MEMORY, STRUCT_ACCESS, DIRECT_REGISTER } AddressingMode;

typedef struct opcode_structure
{
	int DecimalCode;
	char* opcodeName;
	int binaryCode[WORD_SIZE];
	int group;
} opcodeStructure;


typedef struct register_structure
{
	const char* registerName;
	int address[REGISTER_ADDRESS_SIZE];
} RegisterStructure;


typedef struct word
{
	int opcode[OPCODE_SIZE];
	int sourceOperand[OPERAND_SIZE];
	int ERAcode[ERACODE_SIZE];
} word;


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



extern ERAStructure ERATypes[ERA_TYPES_SIZE];

extern char* WIERD_32_BASE[BASE_LENGTH];

extern char* Keywords[NUM_OF_KEYWORDS];

extern char* Registers[NUM_OF_REGISTERS];

extern opcodeStructure opcodes[NUM_OF_OPCODES];

extern RegisterStructure registersBinary[NUM_OF_REGISTERS];

extern int lineCounter;

extern char *OpcodesFirstGroup[FIRST_GROUP_SIZE];

extern char *OpcodesSecondGroup[SECOND_GROUP_SIZE];

extern char *OpcodesThirdGroup[THIRD_GROUP_SIZE];


#endif