
/*
TODO:
Responsible: GUY
add errors definitions
add global errors such as is a string a keyword / is legal number/ is valid opcode .... 
#define ERRORNAME "error description"
*/
#define OPEN_FILE_ERROR "could not open the file %s"


/*functions to find errors*/


/*positive and negative integer */


/*
TODO:
1. add references for <errno.h> standard library macros when generating error logs.
*/

int legalNumber(void * num);

int isValdidOpcode(char* token);

void printError(char*);

int hasError();

int errorTypes();