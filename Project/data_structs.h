#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

/*linked list to contain all the data words*/
typedef struct data_list{
    int data_word;
    int DC;
    struct data_list *next;
} data_list;

/*
linked list that contains the symbol list
*/
typedef struct symbols{
    char label[MAX_LABEL_NAME+1];
    char bin_adress[MAX_BINARY_SIZE+1];
    short data_type;/* 0 - .data, 1 - .entry, 2 - .external, 3 - code*/
    short are_type;/* 0 - absolute, 1 - external, 2 - relocatable*/
    struct symbols *next;
}symbols;

/*struct to contain only defined entries the user wrote*/
typedef struct entry{
    char label[MAX_LABEL_NAME+1];
    int adress;
    struct entry *next;
}entry;

/*
linked list to contain all the code that is not data
*/
typedef struct code_list{
    char bin_code[MAX_BINARY_SIZE+1];
    int IC;
    struct code_list *next;
} code;

/*
a linked list that contaisn symbols that needs to be reallocated
*/
typedef struct realoc_sym{
    char label[MAX_LABEL_NAME+1];
    int adress;
    int line;
    struct realoc_sym *next;
}realoc_sym;

/*adds a data node to the data list*/
void add_data_to_Last(data_list **head, int data, int *DC);

/*
adds a symbol to the symbol list
*/
void add_symbol(symbols **head, char *label, char* bin_adress, short data_type,short are_type);
/*
adds an entry to the entry linked list
*/
void add_entry(entry **head, char *label, int adress);
/*
deletes an entry from the entry linked list
*/
void delete_entry(entry **head, char *label);
/*
adds code to the linked list
*/
void add_code_to_Last(code **head, char *bin_code, int *IC);
/*
adds a realoc_sym node to the list
*/
void add_realoc_sym(realoc_sym **head, char *label, int adress, int line);

void printDataList(data_list *head);
void print_symbol(symbols *head);
void print_entry(entry *head);
void print_rel_sym(realoc_sym *head);
void print_code(code *head);

#endif

