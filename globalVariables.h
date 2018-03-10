/*
Here are all the global variables
*/

/*
TODO:
structures and variables for the second pass
*/

#define REGISTER_SIZE 10

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


#define uint unsigned int

typedef enum {FALSE, TRUE} bool;

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
	Absulote,
	External,
	Relocatable
} encodingType;

typedef enum {
	emptySentence,
	comment,
	declaration,
	command
}sentenceType;

typedef enum{
	immediate,
	direct,
	accessToStruct,
	directReg
} addressingMode;

typedef struct opcode_structure
{
	int DecimalCode;
	char* opcodeName;
	int binaryCode[WORD_SIZE];
	int group;
}opcodeStructure;
*/

typedef struct word 
{
	int opcode[OPCODE_SIZE];
	int sourceOperand[OPERAND_SIZE];
	int sourceOperand[OPERAND_SIZE];
	int ERAcode[ERACODE_SIZE];
}word;

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

typedef struct ERAStructure
{
	uint num;
	char* ERAName;
	char* binaryVal;
}ERAStructure;


*/