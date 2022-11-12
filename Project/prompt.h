#ifndef PROMPT_H
#define PROMPT_H

/*if there is a .data prompt*/
/*returns 0 if there was an issue, returns the amount of data if all worked well*/
/*adds the number data to list aswell*/
int config_data(data_list **head, int *DC, char *p, int line_num);
/*
checking if a string input is valid
if not valid retruns NULL
if valid returns the string
*/
char * string_input(char **p, int line_num);
/*
configures the .string input and adds it data to the list
returns 0 if an issue with the string is found
returns 1 for proper string
*/
int config_string(data_list **head, int *DC, char *p, int line_num);
/*
configures .struct entry
returns 0 if an issue was found, otherwise returns 1
*/
int config_struct(data_list **head, int *DC, char *p, int line_num);
/*
configures an .entry data
returns the entry label
*/ 
char * config_entry_extern(char *p);
/*
checks if a line is a data line
returns 1 if all worked well
returns 0 if there was no data prompt
returns -1 if there was a data prompt, but an error was found
*/
int is_prompt(data_list **data_head,symbols **sym_head, resrvedNames **rn_head, entry **ent_head, int *DC, char *line, int line_num);

#endif
