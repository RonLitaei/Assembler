#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"

/*
checks if a line contains only white spaces
returns 0 if not
retruns 1 if so
*/
int is_empty_sent(char *line){
    int i = 0;
    while(line[i] != '\0' && line[i] != '\n'){
        if(!isspace(line[i]) && line[i] != '\t')
            return 0;
        i++;    
    }
    return 1;
}
/*
checks if a line is a comment line, a line that it's first char is ';'(exluding white spaces)
returns 0 if not
retruns 1 if so
*/
int is_comment_sent(char *line){
    int i = 0;
    while(line[i] != '\0' && line[i] != '\n'){
        if(line[i] == ';')
            return 1;
        if(!isspace(line[i]) && line[i] != '\t')
            return 0;
        i++;    
    }
    return 0;
}

/*reads the entire file and adds each line to a linked list*/
/*returns 1 if file opend successfully and 0 if not*/
int readFile(char *fileName, Line **head) {
    /*opening file*/
    FILE *file = fopen(fileName, "r");
    long int f_size = 1;
    int counter = 1, c, i = 0;
    char temp[MAX_LINE];
    
    /*reseting temp*/
    memset(temp,0,MAX_LINE);
    /*if file opening failed*/
    if (file == NULL){
        printf("File: \"%s\" could not be opend\n",fileName);
        return 0;
    } 
        
    /*analyzing the file size*/    
    fseek(file, 0, SEEK_END);
    f_size += ftell(file);
    fseek(file, 0, SEEK_SET);
    
    /*reading file contents and writing it in the linked list*/
    while ((c = fgetc(file)) != EOF){
        if(i > MAX_LINE){
            printf("Cannot open file: \"%s\", MAX LINE LENGTH IS 80\n",fileName);
            FREE_LIST(Line,*head);
            fclose(file);
            return 0;
        }
    	/*adding each line to the linked list*/
        if(c != '\n'){
            temp[i++] = (char)c;    
        }
        else{
            temp[i++] = (char)c;
            temp[i] = '\0';
            addLast(head,counter++,temp);
            memset(temp,0,MAX_LINE+1);
            i = 0;
        }
    }
    temp[i] = '\0';
    /*this is needed for windows only*/
    addLast(head,counter++,temp);
    
    fclose(file);
    return 1;
}

/*checks if there are only white spaces after the word*/
int isEmptyAfr(char *line, char *word){
    char *p, *q = line + strlen(line) - 1;
    p = strstr(line,word);
    /*if word found*/
    if(p){
        /*going forward after the word*/
        p += strlen(word);
        /*if end is met*/
        if(*p == '\0' || *p == '\n')
            return 1;
        /*going forward until the end*/
        while(p != q){
            if(!isspace(*p) && *p != '\t')
                return 0;
        p++;    
        }
        /*final check when p is at the last char*/
        if(!isspace(*p) && *p != '\t')
                return 0;
        return 1;
    }
    return 0;
}

/*checks if there are only white spaces before the word*/
int isEmptyBfr(char *line, char *word){
    char *p, *q = line;
    p = strstr(line,word);
    /*if word found*/
    if(p){
        /*if word is at the begning*/
        if(p == q)
            return 1;
        /*going back before the word*/
        p--;
        /*going back until the first char*/
        while(p != q){
            if(!isspace(*p) && *p != '\t')
                return 0;
        p--;    
        }
        /*final check when p is at the first char*/
        if(!isspace(*p) && *p != '\t')
                return 0;
        return 1;
    }
    return 0;
}

/*checks if the word is the only word in the line*/
int isAlone(char *line, char *word){
    if(isEmptyAfr(line,word) && isEmptyBfr(line,word))
        return 1;
    return 0;    
}

/*checks if there are quotes surrouding the wordm like: "word" in the given line, if so retruns 1 else 0*/
int isInString(char *line, char *word)
{
	int i, flag = 0, qutAmount = 0;
	char *p = NULL, temp[81];
	for(i = 0; i < strlen(line); i++){
		temp[i] = line[i];
		if(temp[i] == '"')
			qutAmount++;
		if(qutAmount % 2 != 0){
			if((p = strstr(temp,word))){
				while(i < strlen(line)){
					if(temp[i++] == '"'){
						flag = 1;
						break;
					}
					temp[i] = line[i];
				}
			if(flag)
				break;	
			}
		}
	}
	return flag;	
}

/*checks if allocated memory pointer was initialized successfully*/
void check_mem_alloc(void *p){
    if(!p){
        printf("Memory allocation failed, exiting program...\n");
        exit(1);
    }
}

/*takes a pointer that points to a number, moving it forward until end of line or comma is reached*/
/*also takes line number in case there is an error to report at the same line*/
/*returns a single int*/
/*if an issue was foind returns INT_MIN*/
int num_input(char **p, int line_num)
{
    int spc_flag = 0, i = 0 , digit = 0 ,sign = 0, result;
    char num[30];
    while(isspace(**p) || **p == '\t')
        (*p)++;
        
    if(**p == ','){
        (*p)++;
        while(isspace(**p) || **p == '\t')
            (*p)++;
        if(**p == '\n' || **p == '\0'){
            printf("ERROR IN LINE: %d, Illegal comma at end of line\n",line_num);
            return INT_MIN;
        }    
        if(**p == ','){
            printf("ERROR IN LINE: %d, Too many consecutive commas\n",line_num);
            return INT_MIN;
        }   
    }    
    if(**p == '-')
        sign++;
    if(**p == '-' || **p == '+'){
        (*p)++;
        digit++;
    }
        
    while(**p != ',' && **p != '\n' && **p != '\0'){
        if(isspace(**p) || **p == '\t'){
            if(digit)
                spc_flag = 1;
            (*p)++;
            continue;
        }
        /*only expecting a digit at this stage*/
        if(!isdigit(**p)){
            printf("ERROR IN LINE: %d, Bad syntax: \"%c\", expecting only digits\n",line_num,**p);
            return INT_MIN;
        }
        if(spc_flag){
            printf("ERROR IN LINE: %d, No space allowed between digits\n",line_num);
            return INT_MIN;
        }
        digit = 1;
        num[i++] = **p;
        (*p)++;
    }
        
    num[i] = '\0';
    
    result = atoi(num);
    if(result > 1023 || (sign && result*-1 < -512)){
        printf("ERROR IN LINE: %d, Number: %d is to big, numbers are limited to 10 bits maximum\n",line_num,result);
        return INT_MIN;
    }
    if(sign)
        result *= -1;
    return result;
}

/*
checks if a label is valid, meaning that it only contains alphabetic chars and numbers
if a resrvedNames head is sent, will also check if the label given is not a resrved names
"char *type" could be "label" or "macro", it is only used for proper error handling
returns 0 for bad label
returns 1 for valid label
*/
int check_label(char *label,resrvedNames *head, int line_num, char *type){
    resrvedNames *temp = head;
    int i = 0;
    
    if(!isalpha(label[i])){
        printf("ERROR IN LINE: %d, Illegal %s name: \"%s\"\n",line_num,type,label);
        return 0;
    }
    
    for(i = 1; i < strlen(label) && label[i] != ':';i++){
        if(!isalpha(label[i]) && !isdigit(label[i])){
            printf("ERROR IN LINE: %d, Illegal %s name: \"%s\"\n",line_num,type,label);
            return 0;
        }
    }
    
    while(temp != NULL){
        if(!strcmp(temp->name,label)){
            if(temp->reserved == 1){
                printf("ERROR IN LINE: %d, %s name: \"%s\" is a reserved keyword\n",line_num,type,label);
                return 0;
            }
            if(!strcmp(type,"macro"))
            	printf("WARNING IN LINE: %d, %s name: \"%s\" already defined, %ss can't be named as previously declared macros or labels\n",line_num,type,label,type);
            else	
            	printf("ERROR IN LINE: %d, %s name: \"%s\" already defined, %ss can't be named as previously declared macros or labels\n",line_num,type,label,type);
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

/*
checks if there is a label from a given position to an ending position
returns NULL if there is no label
returns the label if it was found
*/
char * is_label(char *beginning, char *current, int line_num, resrvedNames *head){
    char *label = (char*) malloc((MAX_LABEL_NAME+1)*sizeof(char));
    int i = 0, space = 0;
    
    check_mem_alloc(label);
    /*skip space*/
    while((isspace(*beginning) || *beginning == '\t') && beginning != current)
        beginning++;
        
    while(beginning != current){
        if(!isspace(*beginning) && *beginning != '\t'){
            /*if space is between chars*/
            if(space){
                printf("ERROR IN LINE: %d, No space allowed between label chars/ too many arguments\n",line_num);
                free(label);
                return NULL;
            }
            label[i++] = *beginning;
        }
        else
            space = 1;
        beginning++;    
    }
    if(i){
        if(label[--i] != ':'){
            label[++i] = '\0';
            printf("ERROR IN LINE: %d, Missing \':\' in label name: \"%s\"\n",line_num,label);
            free(label);
            return NULL;
        }
    }
    label[i] = '\0';
    
    if(!check_label(label,head,line_num,"label")){
        free(label);
        return NULL;
    }
    return label;    
}
