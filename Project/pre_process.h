#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

/*
reading macros from the linked list and deletes the line they occured at
in an issue with macro found will copy the reserved keyword "macro" to its name
if a macro was found with issues, it will still be deleted for correct error handling of other functions
*/
char * read_macro(Line **head, char *macro_name,resrvedNames **rnHead);

/*laying out the macros and deleteing their reference*/
void macro_layout(char *macro, char *macro_name, Line **head);

/*
reading macros and laying them out in the line linked list
retruns 1 if .am file is needed for opening - meaning there are macros
returns 0 if there were no macros
returns -1 if a macro was found with an issue
*/
int pre_process(Line **head, resrvedNames **rnHead);

#endif
