/*
Authors:
Gil Mansharov
Guy Komari
*/

#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "GlobalsFunctions.h"

/*
Description: the main function of the first pass.
parses from the input file the commands and their sizes and updates IC.
parses from the input file the labels, entries, and externals, and updates DC.
*/
bool firstpass(char*);

/*
Description: parses the data and inserts it to the data list, and updates DC.
*/
bool ParseData(dataPtr*, dataPtr*, char *);

/*
Description: gets a line that contains an extern label (.extern) and adds it to the symbols list.
*/
bool externLabels(char *line);

/*
gets a line with a command (for example "mov r1, r2"), and if it has a label, adds it to the symbols list.
afterwards, calculates the size of the command in memory, and increments IC with the command size.
*/
bool parseCommand(char *line);

#endif
