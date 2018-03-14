#ifndef ERRORS_H
#define ERRORS_H
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
/*
TODO:
add errors definitions
add global errors such as is a string a keyword / is legal number/ is valid opcode .... 
#define ERRORNAME "error description"
*/
#define OPEN_FILE_ERROR "could not open the file %s"

#define MISSING_INPUT_FILE "missing input files %s"

#define ERRORS_IN_FIRST_PASS "errrors found on the first pass, output files will not be created for the input file - %s"

#define ERRORS_IN_SECOND_PASS "errrors found on the second pass, output files will not be created for the input file - %s"

#define MISSING_SPACE_AFTER_ENTRY "[ in line %d ] A space is missing after declaration of an entry"

#define MISSING_SPACE_AFTER_EXTERN "[ in line %d ] A space is missing after declaration of an extern"

#define LABEL_BEGINS_WITH_NUMBER "[ in line %d ] Label should not begin with a number"

#define ENTRY_MORE_THEN_ONE_LABEL "[ in line %d ] only one label can be declared in entry declaration"

#define EXTERN_MORE_THEN_ONE_LABEL "[ in line %d ] only one label can be declared in extern declaration"

#define NUMBER_IS_OUT_OF_RANG "the number is out of range, the language range is -256 to 256"

#define LABEL_NOT_START_WITH_ALPHA "The label should start with an alphabetical letter"

#define LABEL_CONTAINS_NON_ALPHA_OR_DIGIT "The label should contain only alphabetical letters and digits"

#define MISSSING_COLON_ERROR "A colon is missing after a definition of a label name"

#define OPEN_INPUT_FILE_ERROR "the input file - %s does not exist"



/*functions to find errors*/


/*positive and negative integer */


/*
TODO:
1. add references for <errno.h> standard library macros when generating error logs.
*/


void printError(char*);

#endif
