#ifndef UTILS_H
#define UTILS_H

/*
checks if a line contains only white spaces
returns 0 if not
retruns 1 if so
*/
int is_empty_sent(char *line);
/*
checks if a line is a comment line, a line that it's first char is ';'(exluding white spaces)
returns 0 if not
retruns 1 if so
*/
int is_comment_sent(char *line);
/*reads the entire file and adds each line to a linked list*/
/*returns 1 if file opend successfully and 0 if not*/
int readFile(char *fileName, Line **head);
/*checks if there are only white spaces after the word*/
int isEmptyAfr(char *line, char *word);
/*checks if there are only white spaces before the word*/
int isEmptyBfr(char *line, char *word);
/*checks if the word is the only word in the line*/
int isAlone(char *line, char *word);
/*checks if there are quotes surrouding the wordm like: "word" in the given line, if so retruns 1 else 0*/
int isInString(char *line, char *word);
/*checks if allocated memory pointer was initialized successfully*/
void check_mem_alloc(void *p);
/*takes a pointer that points to a number, moving it forward until end of line or comma is reached*/
/*also takes line number in case there is an error to report at the same line*/
/*returns a single int*/
/*if an issue was foind returns INT_MIN*/
int num_input(char **p, int line_num);
/*
checks if a label is valid, meaning that it only contains alphabetic chars and numbers
if a resrvedNames head is sent, will also check if the label given is not a resrved names
"char *type" could be "label" or "macro", it is only used for proper error handling
returns 0 for bad label
returns 1 for valid label
*/
int check_label(char *label,resrvedNames *head, int line_num, char *type);
/*
checks if there is a label from a given position to an ending position
returns NULL if there is no label
returns the label if it was found
*/
char * is_label(char *beginning, char *current, int line_num, resrvedNames *head);

#endif

