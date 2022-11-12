#ifndef ACTION_H
#define ACTION_H

/*
will determine adressing method, will find some but not all issues with input
returns:
-1 - if any issue found
0...3 - for adressing type 0...3
for 0 - label will be the number received
for 1 - label will the called label
for 2 - label will be the struct
for 3 - label will be register number
*/
int adressing_method(char **p, int line_num, char *label);
/*
determines if a line is an action line
this function checks input and sets the code in the code linked list
returns 0 if not
returns 1 if it is
returns -1 if it is, but an error was found
*/
int action_manager(char *line, realoc_sym **rlc_sym_head, symbols **sym_head, resrvedNames **rn_head, code **code_head, int *IC, int line_num);

#endif
