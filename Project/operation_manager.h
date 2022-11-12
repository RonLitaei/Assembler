#ifndef OPERATION_MANAGER_H
#define OPERATION_MANAGER_H
/*
controls the entire code, manages each line and its type, sets the symbol table and code linked list
if no errors were found will open all the needed files
*/
int operation_manager(Line *line_head, resrvedNames **rn_head, char *file_name, int open_am);

#endif
