#ifndef LINE_RESERVED_WORDS_DATA_STRUCTS_H
#define LINE_RESERVED_WORDS_DATA_STRUCTS_H
/*linked list to hold reserved keywords*/
typedef struct resrvedNames{
    int reserved;
    char name[MAX_LABEL_NAME];
    struct resrvedNames *next;
}resrvedNames;

/*linked list to hold each line, each line ends with '\n' and max line length is 81*/
typedef struct Line{
    int line_i;
    int og_line;
    char line[MAX_LINE];
    struct Line *next;
}Line;

/*fixes lines after various function ruins their order*/
void fixLineNum(Line **head);
/* returns the size of the linked list*/
int getSize(Line* head);
/*inserts a struct Line node after the given line_number*/
void insertNodeAfterLine(int line_number, char* line_str, Line** head);
/*deletes a desired line*/
void deleteLine(Line** head, int line_number);
/*add line to the last of the linked list*/
void addLast(Line **head, int i, char *stri);
/*adding nodes to the reserved words linked list*/
void addLastResv(resrvedNames **head,int reserved, char *name);

#endif
