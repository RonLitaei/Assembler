#include "constants.h"
#include "data_structs.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"
#include "conversions.h"

/*adds a data node to the data list*/
void add_data_to_Last(data_list **head, int data, int *DC)
{
    /*create a new data node*/
    data_list *new_data_list = (data_list*)malloc(sizeof(data_list));
    check_mem_alloc(new_data_list);
    new_data_list->data_word = data;
    new_data_list->DC = *DC;
    new_data_list->next = NULL;
    (*DC)++;
    /*if list is empty*/
    if(*head == NULL)
         *head = new_data_list;
    /*if not empty, find the last node and add the new node*/
    else
    {
        data_list *last_data_list = *head;

        while(last_data_list->next != NULL)
            last_data_list = last_data_list->next;
            
        /*add the new node at the end of the list*/
        last_data_list->next = new_data_list;
    }
}

/*
adds code to the linked list
*/
void add_code_to_Last(code **head, char *bin_code, int *IC)
{
    /*create a new Line*/
    code *new_code = (code*)malloc(sizeof(code));
    check_mem_alloc(new_code);
    strncpy(new_code->bin_code,bin_code,MAX_BINARY_SIZE);
    new_code->bin_code[MAX_BINARY_SIZE] = '\0';
    new_code->IC = *IC;
    new_code->next = NULL;
    (*IC)++;
    /*if list is empty*/
    if(*head == NULL)
         *head = new_code;
    /*if not empty, find the last Line and add the new Line*/
    else
    {
        code *last_code = *head;

        while(last_code->next != NULL)
            last_code = last_code->next;
            
        /*add the new Line at the end of the list*/
        last_code->next = new_code;
    }
}

/*
adds a symbol to the symbol list
*/
void add_symbol(symbols **head, char *label, char* bin_adress, short data_type,short are_type)
{
    /*create a new symbol*/
    struct symbols *newSymbol = NULL;
    struct symbols *temp = *head;
    /*avoiding duplicates*/
    while(temp != NULL){
        if(!strcmp(temp->label,label))
            return;
        temp = temp->next;
    }
    
    newSymbol = malloc(sizeof(struct symbols));
    check_mem_alloc(newSymbol);
    if(label){
        strncpy(newSymbol->label,label,MAX_LABEL_NAME);
        newSymbol->label[strlen(label)] = '\0';
    }
    if(bin_adress){
        strncpy(newSymbol->bin_adress,bin_adress,MAX_BINARY_SIZE);
        newSymbol->bin_adress[strlen(bin_adress)] = '\0';
    }
    newSymbol->data_type = data_type;
    newSymbol->are_type = are_type;
    newSymbol->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newSymbol;
    /*if not empty, find the last symbol and add the new symbol*/
    else
    {
        symbols *lastSymbol = *head;

        while(lastSymbol->next != NULL)
            lastSymbol = lastSymbol->next;
            
        /*add the new symbol at the end of the list*/
        lastSymbol->next = newSymbol;
    }
}

/*
adds an entry to the entry linked list
*/
void add_entry(entry **head, char *label, int adress)
{
    /*create a new symbol*/
    struct entry *newEntry = malloc(sizeof(struct entry));
    check_mem_alloc(newEntry);
    if(label){
        strncpy(newEntry->label,label,MAX_LABEL_NAME);
        newEntry->label[strlen(label)] = '\0';
    }
    newEntry->adress = adress;
    newEntry->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newEntry;
    /*if not empty, find the last symbol and add the new symbol*/
    else
    {
        entry *lastEntry = *head;

        while(lastEntry->next != NULL)
            lastEntry = lastEntry->next;
            
        /*add the new symbol at the end of the list*/
        lastEntry->next = newEntry;
    }
}

/*
deletes an entry from the entry linked list
*/
void delete_entry(entry **head, char *label){
    entry *temp = *head, *prev;
        
    /*if the first line is to be deleted*/
    if (temp != NULL && !strcmp(temp->label,label)) {
        *head = temp->next;
        free(temp);
        return;
    }
    /*searching for the desired line number while keeping its previous
    on sight for later use*/
    while (temp != NULL && strcmp(temp->label,label)) {
        prev = temp;
        temp = temp->next;
    }
 
    /* disconnecting node*/
    prev->next = temp->next;
    free(temp);
}

/*
adds a realoc_sym node to the list
*/
void add_realoc_sym(realoc_sym **head, char *label, int adress, int line){
    /*create a new node*/
    realoc_sym *new_sym = (realoc_sym*)malloc(sizeof(realoc_sym));
    check_mem_alloc(new_sym);
    if(label){
        strncpy(new_sym->label,label,MAX_LABEL_NAME);
        new_sym->label[strlen(label)] = '\0';
    }
    new_sym->adress = adress;
    new_sym->line = line;
    new_sym->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = new_sym;
    /*if not empty, find the last node and add the new node*/
    else
    {
        realoc_sym *last_sym = *head;

        while(last_sym->next != NULL)
            last_sym = last_sym->next;
            
        /*add the new node at the end of the list*/
        last_sym->next = new_sym;
    }
}

void printDataList(data_list *head){
    data_list *temp = head;
    while(temp){
        printf("(%d,%d)\n",temp->data_word,temp->DC);
        temp=temp->next;
    }
}

void print_symbol(symbols *head){
    symbols *temp = head;
    while(temp != NULL){
        printf("label: %s, adress: %s, decimal: %d, ARE: %d, data_type: %d\n",temp->label,temp->bin_adress,binaryToDecimal(temp->bin_adress),temp->are_type,temp->data_type);
        temp = temp->next;
    }
}
void print_entry(entry *head){
    entry *t = head;
    while(t != NULL){
        printf("entry label:%s, adress: %d\n",t->label,t->adress);
        t = t->next;
    }
}

void print_rel_sym(realoc_sym *head){
    realoc_sym *tmp = head;
    while(tmp != NULL){
        printf("label: %s, adress: %d, line: %d\n",tmp->label,tmp->adress,tmp->line);
        tmp = tmp->next;
    }
}

void print_code(code *head){
    code *temp = head;
    while(temp){
        printf("IC: %d ,code: %s\n",temp->IC,temp->bin_code);
        temp = temp->next;
    }
}
