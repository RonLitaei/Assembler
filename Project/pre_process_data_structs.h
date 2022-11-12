#ifndef PRE_PROCESS_DATA_STRUCTS_H
#define PRE_PROCESS_DATA_STRUCTS_H

/*struct to contain macros and their names*/
typedef struct macros{
    char macro[MAX_MACRO_MEM+1];
    char name[MAX_LABEL_NAME+1];
    struct macros *next;
}macros;

void print_macro(macros *head);

/*deletes a macro from the linked list by name*/
void deleteMacro(macros **head, char *name);

/*adds macro and its name to the linked list*/
void add_macro(macros **head,char *macro, char *name);

#endif
