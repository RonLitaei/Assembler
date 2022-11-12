#include "constants.h"
#include "pre_process_data_structs.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"

void print_macro(macros *head){
    macros *temp = head;
    while(temp != NULL){
        printf("%s: %s\n",temp->name,temp->macro);
        temp = temp->next;
    }
}

/*deletes a macro from the linked list by name*/
void deleteMacro(macros **head, char *name)
{
    macros *temp = *head, *prev;
 
    /*if head of list is the one to be deleted*/
    if (temp != NULL && !strcmp(temp->name, name)) {
        *head = temp->next;
        free(temp);
        return;
    }
 
    /*searching for the name*/
    while (temp != NULL && strcmp(temp->name, name)) {
        prev = temp;
        temp = temp->next;
    }
 
    /*if the name was not found in the list*/
    if (temp == NULL)
        return;
 
    /*unlink macro*/
    prev->next = temp->next;
    free(temp);
}
/*adds macro and its name to the linked list*/
void add_macro(macros **head,char *macro, char *name){
    /*create a new macros*/
    struct macros *newMacro = malloc(sizeof(struct macros));
    /*delete a previously declared macro, if not previously declarted, does nothing*/
    check_mem_alloc(newMacro);
    deleteMacro(head,name);
    /*check_mem_alloc(newMacro);*/
    strncpy(newMacro->macro,macro,MAX_MACRO_MEM);
    strncpy(newMacro->name,name,MAX_LABEL_NAME);
    newMacro->macro[strlen(newMacro->macro)] = newMacro->name[strlen(newMacro->name)] = '\0';
    newMacro->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newMacro;
    /*if not empty, find the last macros and add the new macros*/
    else
    {
        macros *lastMacro = *head;

        while(lastMacro->next != NULL)
            lastMacro = lastMacro->next;
            
        /*add the new macros at the end of the list*/
        lastMacro->next = newMacro;
    }
}

