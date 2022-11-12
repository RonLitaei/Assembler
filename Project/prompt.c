#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "data_structs.h"
#include "utils.h"
#include "conversions.h"
#include "prompt.h"

/*if there is a .data prompt*/
/*returns 0 if there was an issue, returns the amount of data if all worked well*/
/*adds the number data to list aswell*/
int config_data(data_list **head, int *DC, char *p, int line_num)
{
    int i = 0, j = 0, arr[50] , num;
    /*checking num input in the .data field*/
    while(*p != '\n' && *p != '\0' && num != INT_MIN){
        arr[j++] = num = num_input(&p,line_num);
        if(num == INT_MIN)
            return 0;
    }
    /*adding data to list*/    
    for(i = 0; i < j; i++)
        add_data_to_Last(head,arr[i],DC);
    return j;    
}

/*
checking if a string input is valid
if not valid retruns NULL
if valid returns the string
*/
char * string_input(char **p, int line_num){
    char *res = (char*) malloc((MAX_LINE+1)*sizeof(char)), *q = *p , temp[81];
    int i = 0;
    check_mem_alloc(res);
    /*skip space*/
    while(isspace(**p) || **p == '\t')
        (*p)++;
        
    /*if no opening quote was found*/
    if(**p != '"' || **p == '\n' || **p == '\0'){
        printf("ERROR IN LINE: %d, Opening quote(\") is missing\n",line_num);
        free(res);
        return NULL;
    }

    (*p)++;
    /*copying the string to res*/
    while(**p != '\0'){
        if(**p == '"'){
            int j;
            for(j = 1; (*p)[j] != '\0';j++){
                if((*p)[j] == '"')
                    break;
            }
            if((*p)[j] == '\0')
                break;
        }
        res[i] = **p;
        temp[i++] = **p;
        (*p)++;
    }
    res[i] = '\0';
    
    temp[i++] = '"';/*temp is used to simulate the string in the line for further inspection*/
    temp[i] = '\0';
    /*if no closing quote was found*/    
    if(**p != '"' || **p == '\n' || **p == '\0'){
        printf("ERROR IN LINE: %d, Closing quote(\") is missing\n",line_num);
        free(res);
        return NULL;
    }
    
    (*p)++;
    
    /*if there is text after the string*/
    if(!isEmptyAfr(q,temp)){
        printf("ERROR IN LINE: %d, Extraneous text after string\n",line_num);
        free(res);
        return NULL;
    }
    return res;
}
/*
configures the .string input and adds it data to the list
returns 0 if an issue with the string is found
returns 1 for proper string
*/
int config_string(data_list **head, int *DC, char *p, int line_num)
{
    char *str = NULL;
    int i;
    /*if an issue with the .string was found*/
    if(!(str = string_input(&p,line_num)))
        return 0;
    /*adding each char to the data list*/
    for(i = 0; i < strlen(str);i++)
        add_data_to_Last(head,(int)str[i],DC);
    /*adding null terminator to the list*/
    free(str);
    add_data_to_Last(head,0,DC);    
    return 1;    
}


/*
configures .struct entry
returns 0 if an issue was found, otherwise returns 1
*/
int config_struct(data_list **head, int *DC, char *p, int line_num)
{
        int num,i;
        char *str = NULL;
        
        /*first stage, if num input is valid*/
        num = num_input(&p,line_num);
        if(num == INT_MIN)
            return 0;
        /*num was valid, so now searching for the seperating comma*/    
        while(*p != ','){
            if(*p == '\n' || *p == '\0'){
                printf("ERROR IN LINE: %d, Missing string parameter in struct\n",line_num);
                return 0;
            }
            if(*p == '"'){
                printf("ERROR IN LINE: %d, Missing comma after integer in struct\n",line_num);
                return 0;
            }
            p++;
        }
        /*comma was found, skipping it*/
        p++;
        
        /*second stage, if string input is valid*/
        if(!(str = string_input(&p,line_num)))
            return 0;
        
        /*all input was valid, entering the data to the list*/    
        add_data_to_Last(head,num,DC);
        for(i = 0; i < strlen(str);i++)
            add_data_to_Last(head,(int)str[i],DC);
        /*adding null terminator to the list*/
        add_data_to_Last(head,0,DC);    
        free(str);    
        return 1;    
}
/*
configures an .entry data
returns the entry label
*/ 
char * config_entry_extern(char *p)
{
    int i;
    char *temp = (char*) malloc(81 * sizeof(char));
    
    check_mem_alloc(temp);
    /*simply copying anything after entry or extern*/
    for(i = 0; !isspace(p[i]) && p[i] != '\t' && p[i] != '\n' && p[i] != '\0';i++){
        temp[i] = p[i];
    }
    
    temp[i] = '\0';
    return temp;
}


/*
checks if a line is a data line
returns 1 if all worked well
returns 0 if there was no data prompt
returns -1 if there was a data prompt, but an error was found
*/
int is_prompt(data_list **data_head,symbols **sym_head, resrvedNames **rn_head, entry **ent_head, int *DC, char *line, int line_num){
    char *p = NULL, *q = NULL, *label = NULL, *str = NULL;
    char *prompts[5] = {".data",".string",".struct",".entry",".extern"};
    int prompt = -1, config_temp = 0, entryy = 0, curr_DC = *DC, flag = 0;
    
    /*searching for a prompt in the line*/
    if((p = strstr(line,prompts[0])))
        prompt = 0;
    if(prompt == -1 && (p = strstr(line,prompts[1])))
        prompt = 1;
    if(prompt == -1 && (p = strstr(line,prompts[2])))
        prompt = 2;
    if(prompt == -1 && (p = strstr(line,prompts[3])))
        prompt = 3;
    if(prompt == -1 && (p = strstr(line,prompts[4])))
        prompt = 4;
    if(prompt == -1)
        return 0;
        
    /*if there is a label*/
    if(!isEmptyBfr(line,prompts[prompt])){
        /* if there is an issue with label name*/
        if(!(label = is_label(line,p,line_num,*rn_head)))
            return -1;
        if(prompt >= 0 && prompt < 3){
            add_symbol(sym_head,label,q = decimalToBinary(curr_DC,10),0,2);
            addLastResv(rn_head,0,label);
            free(q);
            q = NULL;
        }    
    }
        
    p += strlen(prompts[prompt]);
    if(!isspace(*p) && *p != '\t'){
        printf("ERROR IN LINE: %d, Space is required after \"%s\"\n",line_num,prompts[prompt]);
        free(label);
        return -1;
    }
    /*skip space*/
    while(isspace(*p) || *p == '\t')
        p++;
        
    /*misplaced comma error*/    
    if(*p == ','){
        printf("ERROR IN LINE: %d, Misplaced comma after \"%s\"\n",line_num,prompts[prompt]);
        free(label);
        return -1;
    }
    switch(prompt){
        case 0:/*data*/
            config_temp = config_data(data_head,DC,p,line_num);
            break;
        case 1:/*string*/
            config_temp = config_string(data_head,DC,p,line_num);
            break;
        case 2:/*struct*/
            config_temp = config_struct(data_head,DC,p,line_num);
            break;
        case 3:/*entry*/
            entryy++;
            /*add to entry list*/
        case 4:/*extern*/
            config_temp++;
            str = config_entry_extern(p);
            check_label(str,NULL,line_num,"label");/*NULL - rn_head*/
            if(!isEmptyAfr(p,str)){
                printf("ERROR IN LINE: %d, Too many arguments after \"%s\"\n",line_num,str);
                free(str);
                free(label);
                return -1;
            }
            if(!entryy){/*is extern*/
                /*making sure that extern wasnt declared by entry*/
                entry *tmp_ent = *ent_head;
                while(tmp_ent != NULL){
                    if(!strcmp(tmp_ent->label,str)){
                        printf("ERROR IN LINE: %d, Label: \"%s\" cannot be declared as extern, it was declared by entry\n",line_num,str);
                        flag++;
                        break;
                    }
                    tmp_ent = tmp_ent->next;
                }
                if(!flag){/*symbol declared correctly*/
                    add_symbol(sym_head,str,"0000000000",2,1);
                    addLastResv(rn_head,0,str);
                }
            }
            else{/*will add the entry for the second stage*/
                symbols *sym_temp = *sym_head;
                while(sym_temp != NULL){
                    if(sym_temp->data_type == 2)
                        if(!strcmp(sym_temp->label,str)){
                            printf("ERROR IN LINE: %d, Label: \"%s\" cannot be declared as entry, it was declared by extern\n",line_num,str);
                            flag++;
                            break;
                        }
                    sym_temp = sym_temp->next;
                }
                if(!flag)/*symbol declared correctly*/
                    add_entry(ent_head,str,0);
            }
                
            if(label){
                printf("Warning in line: %d, Label: \"%s\" is not needed and will be disregarded\n",line_num,label);
                /*if enabled, will not overide entry or extern label*/
                /*addLastResv(rn_head,0,label);*/
            }
            free(str);
            break;
    }
    if(!config_temp || flag){
        free(label);
        return -1;
    }
  
    free(label);
    return 1;
    
}
