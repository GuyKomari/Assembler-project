#ifndef ERRORS_H
#define ERRORS_H
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "globalVariables.h"
/*
TODO:
add errors definitions
add global errors such as is a string a keyword / is legal number/ is valid opcode ....
#define ERRORNAME "error description"
*/

#define FILE_NAME_MEM_ALLOCATION_FAILURE "Could not allocate memory for the file name you provided"

#define OPEN_FILE_ERROR "could not open the file "

#define ERROR_CREATE_FILE	"Could not create file"

#define MISSING_INPUT_FILE "missing input files "

#define ERRORS_IN_FIRST_PASS "errors found on the first pass, output files will not be created for the input file "

#define ERRORS_IN_SECOND_PASS "errrors found on the second pass, output files will not be created for the input file "

#define MISSING_SPACE_AFTER_ENTRY "A space is missing after declaration of an entry"

#define MISSING_SPACE_AFTER_EXTERN "A space is missing after declaration of an extern"

#define LABEL_BEGINS_WITH_NUMBER  "Label should not begin with a number"

#define ENTRY_MORE_THEN_ONE_LABEL "only one label can be declared in entry declaration"

#define EXTERN_MORE_THEN_ONE_LABEL "only one label can be declared in extern declaration"

#define NUMBER_IS_OUT_OF_RANG "the number is out of range, the language range is -256 to 256"

#define LABEL_NOT_START_WITH_ALPHA "The label should start with an alphabetical letter"

#define LABEL_CONTAINS_NON_ALPHA_OR_DIGIT "The label should contain only alphabetical letters and digits"

#define MISSSING_COLON_ERROR "A colon is missing after a definition of a label name"

#define OPEN_INPUT_FILE_ERROR "the input file does not exist"

#define LABEL_DECLARED_BUT_NOT_DEFINED	"An undefined label has been declared"

#define ERROR_CONVERT_LABEL_ADDRESS_TO_WEIRD	"Cannot convert label address to weird base"

#define ALLOCATE_MEMORY_ERROR "cannot allocate memory for the data"

#define STRUCT_DEFINITION_ERROR "Unrecognized Type in struct definition"

#define INVALID_NUMBER_IN_DATA_LABEL "Invalid number in .data label"

#define MISSING_SPACE_AFTER_COLON	"Missing space after colon"

#define LABEL_IS_ALREADY_DEFINED	"Label is already defined"

#define VARIABLE_NAME_EQUALS_KEYWORD_NAME	"A variable name has the same name as a keyword"

#define OPERAND_NAME_EQUALS_KEYWORD_NAME	"An operand name cannot be the same as a keyword name"

#define TOO_FEW_OPERANDS_IN_COMMAND	"Too few operands in the command"

#define INVALID_SRC_OPERAND	"Source operand is invalid"

#define INVALID_DEST_OPERAND	"Destination operand is invalid"

#define LEA_COMMAND_ADDRESSING_MODE_ERROR	"\"lea\" opcode cannot have an immediate or immediate register addressing mode"

#define CMP_COMMAND_ADDRESSING_MODE_ERROR	"Only \"cmp\" opcode can use immediate addressing mode in the destination operand"

#define PRN_COMMAND_ADDRESSING_MODE_ERROR	"Only \"prn\" opcode can use immediate addressing mode in the destination operand"

#define TOO_MUCH_WORDS_IN_COMMAND	"Too much words in the command"

#define SYMBOLS_LIST_MEM_ALLOCATION_FAILURE	"Memory allocation failed in symbols list addition"

#define MISSING_SPACE_AFTER_STRING_DECLARATION	"Missing space after .string declaration"

#define MISSING_SPACE_AFTER_DATA_DECLARATION	"Missing space after .data declaration"

#define MISSING_SPACE_AFTER_STRUCT_DECLARATION	"Missing space after .struct declaration"

/*/
TODO:
1. add references for <errno.h> standard library macros when generating error logs.
*/


void printError(char*);

void printFileError(char*, char*);

void printWarning(char*);



#endif
