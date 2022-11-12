#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "data_structs.h"
#include "utils.h"
#include "conversions.h"

/*
will determine adressing method, will find some but not all issues with input
returns:
-1 - if any issue found
0...3 - for adressing type 0...3
for 0 - label will be the number received
for 1 - label will be the called label
for 2 - label will be the struct
for 3 - label will be register number
*/
int adressing_method(char **p, int line_num, char *label)
{
	int result = 0, i = 0 , temp, spc_flag = 0;
	while(isspace(**p) || **p == '\t'){
	    (*p)++;
	    if(**p == '\n' || **p == '\0' || **p == ','){
	        if(**p == ',')
	            printf("ERROR IN LINE: %d, Missing first operator\n",line_num);
	        else
	            printf("ERROR IN LINE: %d, Missing second operator\n",line_num);
			return -1;
	    }   
	}
	/*adressing method number 0 - a number*/
	if(isdigit(**p)){
	    printf("ERROR IN LINE: %d, \'#\' Missing from the number\n",line_num);
	    return -1;
	}
	if(**p == '#'){
		(*p)++;
		if(!isdigit(**p) && **p != '+' && **p != '-'){
			printf("ERROR IN LINE: %d, Bad syntax: \"%c\", expecting only numbers\n",line_num,**p);
			return -1;
		}
		/*analyzing input*/
		result = num_input(p,line_num);
		if(result != INT_MIN){/*if input was good*/
		    temp = result;
			while(temp){
				i++;
				temp /= 10;
			}
			sprintf(label, "%d", result);/*label received the number*/
			label[++i] = '\0';
			return 0;
		}
		else
		    return -1;
	}
	/*adressing method number 3 - a register*/
	if(**p == 'r'){
		(*p)++;
		if(**p >= '0' && **p <= '7'){
		    (*p)++;
		    /*if a white char is found then it must be a register, else it might be a substring of a label*/
		    if(isspace(**p) || **p == '\t' || **p == '\n' || **p == '\0' || **p == ','){
		    	    /*entering register at label*/
			    label[0] = *(--*p);
			    label[1] = '\0';
			    if(**p != ',')/*used make sure there is a sperating comma if recived as a first operand*/
			        (*p)++;
			    return 3;
		        }
		    (*p)--;  
		}
		/*assuming that a label is found and not a register*/
	    (*p)--;
	}
	/*figuring out if it is a sturct or a label, meaning, if adressind type 2 or 1*/
	while(**p != '\0' && **p != ',' && **p != '.'){
	    if(isspace(**p) || **p == '\t')
	        spc_flag = 1;
	    if(spc_flag && !isspace(**p) && **p != '\t'){
	        printf("ERROR IN LINE: %d, No space between chars is allowed\n",line_num);
			return -1;
	    }
	    if(!spc_flag)
		    label[i++] = **p;
		(*p)++;
	}
	/*if a struct*/
	if(**p == '.'){
		label[i++] = *(*p)++;
		label[i++] = **p;
		label[i] = '\0';
		if(**p != '1' && **p != '2'){
			printf("ERROR IN LINE: %d, Bad struct reference \"%c\", struct field is only 1 or 2\n",line_num,**p);
			return -1;
		}
		(*p)++;
		return 2; 
	}
	label[i] = '\0';
	return 1;	
}


/*
gets a decimal number, that is converted to binary and then coded into the word from a given position by an amount of bits
*/
void code_word(char *word, int number, int position, int amount){
    char *p = NULL;
    if(word && position < strlen(word))
        strncpy(word+position, p = decimalToBinary(number,amount),amount);
    free(p);    
}
/*
determines if a line is an action line
this function checks input and sets the code in the code linked list
returns 0 if not
returns 1 if it is
returns -1 if it is, but an error was found
*/
int action_manager(char *line, realoc_sym **rlc_sym_head, symbols **sym_head, resrvedNames **rn_head, code **code_head, int *IC, int line_num)
{
    /*
    temp_lab - is used to contain(if fits the case)the first label or struct received as operand
    temp_lab1 - is used to contain the second operand
    */
	char *label = NULL, *p = NULL, *q = NULL, temp_lab[81], temp_lab1[81];
	char *commands[16] = {"mov","cmp","add","sub","lea","not","clr","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"};/*the commands*/
	char words[5][11] = {"0000000000","0000000000","0000000000","0000000000","0000000000"};/*this will stand for the binary codes for the words that needs coding*/
	int words_to_code[5] = {1,0,0,0,0};/*this will stand for the amount of words needed to be coded, first one is always on*/
	/*
	result stands for the first operand(if exsits), result1 stands for the second operand(if exsits)
	in the case of the first 5 commands, result will be the source operand,
	for any other command that receives a single operand, it is the target operand
	result1 will be the target operand for the first 5 commands, and will not be used for any other command
	*/
	int i, command = -1, result = -1, result1 = -1, op_code;
	/*searching for the command*/
	for(i = 0; i < 16; i++){
		if((p = strstr(line,commands[i]))){
			command = i;
			break;
		}
	}
	/*no command found*/
	if(command == -1)
		return 0;
		
	/*if there is a label*/
    if(!isEmptyBfr(line,commands[command])){
        /* if there is an issue with label name*/
        if(!(label = is_label(line,p,line_num,*rn_head)))
            return -1;
        add_symbol(sym_head,label,q = decimalToBinary(*IC,10),3,2);
        addLastResv(rn_head,0,label);
        free(q);
        free(label);
        q = NULL;
    }
        /*skipping the command*/
    	p += strlen(commands[command]);
    	/*checking for space*/
    	if(!isspace(*p) && *p != '\t' && command < 14){
        	printf("ERROR IN LINE: %d, Space is required after \"%s\"\n",line_num,commands[command]);
        	return -1;
    	}
    	/*skip space*/
    	while(isspace(*p) || *p == '\t')
        	p++;
        
    	/*misplaced comma error*/    
    	if(*p == ','){
        	printf("ERROR IN LINE: %d, Misplaced comma after \"%s\"\n",line_num,commands[command]);
        	return -1;
    	}
    	/*reseting the temps*/
    	memset(temp_lab,0,81);
    	memset(temp_lab1,0,81);
    	/*searching for adressing type*/
    	/*if the command is a command that receives an operand*/
    	if(command >= 0 && command <= 13){
    		/*source for mov,cmp,add,sub,lea, for all others this is the target operator*/
    		result = adressing_method(&p, line_num, temp_lab);
    		if(result == -1)
    			return -1;
    		/*if the command is a command that receives 2 operands*/	
    		if(command >= 0 && command <= 4){
    			while(*p != ',' && *p != '\0')
    				p++;
    			if(*p++ != ','){
    				printf("ERROR IN LINE: %d, Missing operator in \"%s\" command\n",line_num,commands[command]);
    				return -1;
    			}
    			while(isspace(*p) || *p == '\t')
    			    p++;
    			if(*p == ','){
    				printf("ERROR IN LINE: %d, Too many consecutive commas\n",line_num);
    				return -1;
    			}
    			/*target operator for mov,cmp,add,sub,lea,*/	
    			result1 = adressing_method(&p, line_num, temp_lab1);
    			if(result1 == -1)
    				return -1;
    		}	
    			
    	}
	while(*p != '\0' && *p != '\n'){
		if(!isspace(*p) && *p != '\t'){
			printf("ERROR IN LINE: %d, Too many parameters\n",line_num);
			return -1; 
		}
		p++;
	}
	/*setting the correct opcode for the command*/
	if(command == 4)
	    op_code = 6;
	else if(command == 5)
	    op_code = 4;
	else if(command == 6)
	    op_code = 5;
	else
	    op_code = command;
	/*setting the opcode in the word*/ 
	code_word(words[0],op_code,0,4);
	/*if it is a command that receives 2 operands*/
	if(command >= 0 && command <= 4)
    	code_word(words[0],result,4,2);
    else if(command > 4 && command < 14)/*command that receives 1 operand*/
        code_word(words[0],result,6,2);
	/*if there is a second operand*/
	if(result1 != -1)
	    code_word(words[0],result1,6,2);
	
	/*check if adressing type fits command*/
    if(result == 0){/*number*/
    	if(command >= 4 && command != 12){
    	    printf("ERROR IN LINE: %d, Adressing type does not fit command: %s \n",line_num, commands[command]);
    	    return -1;
    	}
    	/*copies the number*/
    	code_word(words[1],atoi(temp_lab),0,8);
    	words_to_code[1] = 1;
    }
    if(result == 1){/*label*/
        for(i = 0; i < strlen(temp_lab);i++){
            if(temp_lab[i] == '\n'){
                temp_lab[i] = '\0';
                break;
            }
        }
        /*adding symbol that was received as an operand*/
        add_realoc_sym(rlc_sym_head,temp_lab,(*IC)+1,line_num);
    	words_to_code[1] = 1;
    }
    if(result == 2){/*struct*/
        p = strstr(temp_lab,".")+1;
        i = atoi(p);
        /*need to code label before this is why word1 is saved*/
    	code_word(words[2],i,0,8);
    	words_to_code[2] = words_to_code[1] = 1;
    	for(i = 0; temp_lab[i] != '.';i++)
	            ;
	     temp_lab[i] = '\0';
	    /*adding symbol that was received as an operand*/     
	    add_realoc_sym(rlc_sym_head,temp_lab,(*IC)+1,line_num);     
    }
	if(result == 3){/*register*/
	    if(command == 4){
    	    printf("ERROR IN LINE: %d, Adressing type does not fit command: %s \n",line_num, commands[command]);
    	    return -1;
    	}
	    if(result1 == 3){
	        /*code both in same word*/
	        code_word(words[1],atoi(temp_lab1),4,4);
	        code_word(words[1],atoi(temp_lab),0,4);
	    }
	    else if(result1 != -1){
	        /*code result as source*/
	        code_word(words[1],atoi(temp_lab),0,4);
	    }
	    else if(result1 == -1){
	        /*code result as target*/
	        code_word(words[1],atoi(temp_lab),4,4);
	    }
	    words_to_code[1] = 1;
	}
    	     
	if(result1 == 0){/*number*/
	    if(command != 1 && command != 12){
    	    printf("ERROR IN LINE: %d, Adressing type does not fit command: %s \n",line_num, commands[command]);
    	    return -1;
    	}
    	code_word(words[3],atoi(temp_lab1),0,8);
    	words_to_code[3] = 1;
	}
	
	if(result1 == 1){/*label*/
	    for(i = 0; i < strlen(temp_lab1);i++){
            if(temp_lab1[i] == '\n'){
                temp_lab1[i] = '\0';
                break;
            }
        }
        if(result == 2)/*struct*/
            add_realoc_sym(rlc_sym_head,temp_lab1,(*IC)+3,line_num);
        else
	        add_realoc_sym(rlc_sym_head,temp_lab1,(*IC)+2,line_num);
	    words_to_code[3] = 1;
	}
	
	if(result1 == 2){/*struct*/
	     p = strstr(temp_lab1,".")+1;
	     i = atoi(p);
        /*need to code label before this is why word3 is saved*/
	     code_word(words[4],i,0,8);
	     words_to_code[3] = words_to_code[4] = 1;
	     for(i = 0; temp_lab1[i] != '.';i++)
	            ;
	        temp_lab1[i] = '\0';
	    if(result == 2)/*struct*/
            add_realoc_sym(rlc_sym_head,temp_lab1,(*IC)+3,line_num);
        else
	        add_realoc_sym(rlc_sym_head,temp_lab1,(*IC)+2,line_num);
	}
	if(result1 == 3 && result != 3){/*register*/
	     code_word(words[3],atoi(temp_lab1),4,4);
	     words_to_code[3] = 1;
	}
	/*adds all the words that needs coding to the list*/
	for(i = 0; i < 5; i++){
	    if(words_to_code[i])
	        add_code_to_Last(code_head,words[i],IC);
	}
	    
	return 1;
}
