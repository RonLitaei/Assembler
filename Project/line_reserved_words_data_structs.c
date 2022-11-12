#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"

/*fixes lines after various function ruins their order*/
void fixLineNum(Line **head)
{
    int i = 1;
    Line *temp = *head;
    while(temp != NULL){
        if(temp->line_i != i)
            temp->line_i = i;
        temp = temp->next;
        i++;
    }
}

/* returns the size of the linked list*/
int getSize(Line* head){
    int size = 0;
    Line *temp = head;
    while(temp != NULL){
        temp = temp->next;
        size++;
    }
    return size;
}

/*inserts a struct Line node after the given line_number*/
void insertNodeAfterLine(int line_number, char* line_str, Line** head)
{
    int i, size = getSize(*head);

    Line* newLine = (Line*) malloc(sizeof(Line));
    check_mem_alloc(newLine);
    for(i = 0; i < strlen(line_str); i++)
        newLine->line[i] = line_str[i];
    newLine->line[i] = '\0';    
    newLine->line_i = line_number+1;
    newLine->og_line = line_number+1;
    newLine->next = NULL;
    
    /*checks out of bonds line number*/
    if(line_number <= 0 || line_number > size) 
        return; 
        
    /*inserting first node*/
    else if(line_number == 1){
        newLine->next = *head; 
        *head = newLine;
    }
    
    else 
    {
        Line* temp = *head; 
        /*finding desired node*/
        while(--line_number)
            temp=temp->next;
        /*switching positions*/    
        newLine->next= temp->next;
        temp->next = newLine;
    }
    /*fixing damaged lines*/
    fixLineNum(head);
}

/*deletes a desired line*/
void deleteLine(Line** head, int line_number)
{
    Line *temp = *head, *prev;
    int size = getSize(*head);
    
    /*if the line number is out of bond*/
    if(line_number <= 0 || line_number > size) 
        return;
        
    /*if the first line is to be deleted*/
    if (temp != NULL && temp->line_i == line_number) {
        *head = temp->next;
        fixLineNum(head);
        free(temp);
        return;
    }
    /*searching for the desired line number while keeping its previous
    on sight for later use*/
    while (temp != NULL && temp->line_i != line_number) {
        prev = temp;
        temp = temp->next;
    }
 
    /* disconnecting node*/
    prev->next = temp->next;
    free(temp);
    /*fixing damaged lines*/
    fixLineNum(head);
}

/*add line to the last of the linked list*/
void addLast(Line **head, int i, char *stri)
{
    /*create a new Line*/
    struct Line *newLine = malloc(sizeof(struct Line));
    check_mem_alloc(newLine);
    newLine->line_i = i;
    newLine->og_line = i;
    for(i = 0; i < strlen(stri) ;i++)
        newLine->line[i] = stri[i];
    newLine->line[i] = '\0';    
    newLine->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newLine;
    /*if not empty, find the last Line and add the new Line*/
    else
    {
        Line *lastLine = *head;

        while(lastLine->next != NULL)
            lastLine = lastLine->next;
            
        /*add the new Line at the end of the list*/
        lastLine->next = newLine;
    }
}

/*adding nodes to the reserved words linked list*/
void addLastResv(resrvedNames **head,int reserved, char *name){
     /*create a new Line*/
    int i; 
    struct resrvedNames *newNode = malloc(sizeof(struct resrvedNames));
    check_mem_alloc(newNode);
    newNode->reserved = reserved;
    for(i = 0; i < strlen(name) ;i++)
       newNode->name[i] = name[i];
    newNode->name[i] = '\0';   
    newNode->next = NULL;
    
    /*if list is empty*/
    if(*head == NULL)
         *head = newNode;
    /*if not empty, find the last Line and add the new Line*/
    else
    {
        resrvedNames *lastNode = *head;

        while(lastNode->next != NULL)
            lastNode = lastNode->next;
            
        /*add the new Line at the end of the list*/
        lastNode->next = newNode;
    }
}
