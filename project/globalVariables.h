/*
Here are all the global variables
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

#define IC_START 100

#define DC_START 0

#define FIRST_GROUP 1

#define SECOND_GROUP 2

#define THIRD_GROUP 3

/*Minimum machine instructions*/
#define MIN_ROWS 1

/*maximum machine instructions*/
#define MAX_ROWS 5


#define uint unsigned int

typedef enum {false, true} bool;

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
	uint DecimalCode;
	char* opcodeName;
	char* binaryCode;
	uint group;
}opcodeStructure;

typedef struct firstWord 
{
	uint opcode : OPCODE_SIZE;
	uint sourceOperand : OPERAND_SIZE;
	uint sourceOperand : OPERAND_SIZE;
	uint ERAcode : ERACODE_SIZE;
}firstWord;

typedef struct sentence{
	int id;
	char* sentenceStr;
	int rowsNeeded;  
	word wordByte;
	bol hasError;
	char* errorDescription;
}sentence;

typedef struct symbolsTable
{
	char* name;
	uint address;
	bol isExteranal;
	bol isCommand;
}symbolsTable;

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


