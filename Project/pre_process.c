#include "constants.h"
#include "pre_process_data_structs.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"
#include "pre_process.h"
/*
reading macros from the linked list and deletes the line they occured at
in an issue with macro found will copy the reserved keyword "macro" to its name
if a macro was found with issues, it will still be deleted for correct error handling of other functions
*/

char * read_macro(Line **head, char *macro_name,resrvedNames **rnHead)
{
	Line *temp = *head;
	char *p = NULL;
	int i, j,space_flag,size = 0, lines_to_delete[2] = {0,0}, macro_memdef = 100, overideFlag = 0, error = 0;
	char temp_line[MAX_LINE], *macro = NULL;
	/*reseting temp_line and macro_name*/
	memset(temp_line,0,MAX_LINE);
	memset(macro_name,0,MAX_LABEL_NAME);
	
	while(temp != NULL)
	{
		/*if endmacro was found before macro*/
		if(!(is_comment_sent(temp->line)) && (p = strstr(temp->line,"endmacro")) && !(isInString(temp->line,"endmacro"))){
		        printf("endmacro inserted without macro\n");
		        p = NULL;
		        break;
		}
		/*finding "macro" in each line*/
		if((p = strstr(temp->line,"macro")) && !(is_comment_sent(temp->line)) && !(isInString(temp->line,"macro")))
		{
			i = 5;
			j = space_flag = 0;
			/*copying the linked list line to a tempoary string so it will be easier to understand*/
			strcpy(temp_line,p);
			if(!isEmptyBfr(temp->line,"macro")){
				error++;
				printf("ERROR IN LINE: %d, garbage text before macro\n",temp->og_line);
			}
			/*no space after "macro"*/
   			if(!isspace(temp_line[i]) && temp_line[i] != '\t')
   			{
        		error++;
				printf("ERROR IN LINE: %d, no space after macro\n",temp->og_line);
    			}
    			
			/*skipping white spaces*/
			while(isspace(temp_line[i]) || temp_line[i] == '\t')
				i++;
				
			/*if \n encountered that means that no arguments were inserted afer macro - error*/	
			if(temp_line[i] == '\n')
			{
				error++;
				printf("ERROR IN LINE: %d, macro name not inserted\n",temp->og_line);
			}
			
			/*copying macro name to macro_name*/
			while(temp_line[i] != '\n' && temp_line[i] != '\0')
			{
				/*since this loop already starts at char that is not a white space*/
				/*if one is encountered that means that macro_name is already set*/	
				if(isspace(temp_line[i]) || temp_line[i] == '\t')
					space_flag = 1;
				else{
					/*if space_flag is on that means that there is another set of chars*/
					/*after macro name - error*/
					if(!space_flag)
						macro_name[j++] = temp_line[i];
					else
					{
						error++;
						printf("ERROR IN LINE: %d, too many arguments after macro\n",temp->og_line);
						break;
					}	
				}
				i++;		
			}
			
			macro_name[j] = '\0';	
			/*no need to find another macro reference*/
			lines_to_delete[0] = temp->line_i;
			memset(temp_line,0,MAX_LINE);
			break;
		}
		memset(temp_line,0,MAX_LINE);
		temp = temp->next;
		p = NULL;
	}
	
	/*if no macro was found*/
	if(p == NULL)
		return NULL;
	
	/*if macro name is forbidden*/
	if(macro_name[strlen(macro_name) - 1] == ':'){
		printf("ERROR IN LINE: %d, forbidden macro name\n",temp->og_line);
		/*error++;
		return NULL;*/
	}
	check_label(macro_name,*rnHead,temp->og_line,"macro");
	/*if macro call occoured before macro defenition*/
	temp = *head;
	for(i = 1; i < lines_to_delete[0];i++)
	{
		if((p = strstr(temp->line,macro_name)))
		{
			error++;
			printf("ERROR IN LINE: %d, macro call before macro defenition\n",temp->og_line);
		}
		temp = temp->next;	
	}
	temp = temp->next;
	macro = (char*) calloc(macro_memdef, sizeof(char));
	check_mem_alloc(macro);
	p = NULL;
	
	/*inserting data to macro until endmacro is found
	!!endmacro is always assured in the file so I do not check it!!
	*/
	while(1)
	{
		if((p = strstr(temp->line,"endmacro"))){
			if(!(is_comment_sent(temp->line)) && !(isInString(temp->line,"endmacro")))
				break;
		}
		strcpy(temp_line,temp->line);
		for(i = 0; i < strlen(temp_line); i++)
		{
			if(size >= macro_memdef)
			{
				macro_memdef *= 2;
				macro = (char*) realloc(macro,macro_memdef * sizeof(char));
				check_mem_alloc(macro);
			}
			if(size > 800 && !error){
				printf("ERROR IN LINE: %d, Macro max chars(%d) limit reached:\n",temp->og_line,MAX_MACRO_MEM);
				error++;
				
			}
			if(size <= 800)	
				macro[size++] = temp_line[i];	
		}
		memset(temp_line,0,MAX_LINE);
		temp = temp->next;
	}
	if(!overideFlag)
		addLastResv(rnHead,0,macro_name);
	macro[size] = '\0';
	/*deleteing lines from macro (including) until endmacro (including)*/
	lines_to_delete[1] = temp->line_i;
	for(j = i = lines_to_delete[0]; j <= lines_to_delete[1];j++)
		deleteLine(head,i);
	
	if(error){
	    strcpy(macro_name,"macro");
	    macro_name[5] = '\0';
	}
	return macro;
} 

/*laying out the macros and deleteing their reference*/
void macro_layout(char *macro, char *macro_name, Line **head)
{
	char *p = NULL, str[MAX_LINE];
	int i = 0, j, line,ma_len = strlen(macro_name);
	Line *temp = *head;
	/*reseting str*/
	memset(str,0,MAX_LINE);
	
	while(temp != NULL)
	{
		/*finding macro name call*/
		if((p = strstr(temp->line,macro_name)) && temp->line[0] != ';' && !(isInString(temp->line,macro_name)))
		{
			/*if p found just a substring that contains macro name*/
			if(!isspace(p[ma_len]) && p[ma_len] != '\t' && p[ma_len] != '\n' && p[ma_len] != '\0'){
				temp = temp->next;
				continue;
			}
			/*checking single char before macro name to see its not a substring*/
			if(p != temp->line){
				--p;
				if(!isspace(*p) && *p != '\t'){
					temp = temp->next;
					continue;
				}
			}
			/*marking the line the call was found*/
			line = temp->line_i;
			
			/*if any chars are before or after macro call*/
			if(!isAlone(temp->line,macro_name)){
			        	printf("ERROR IN LINE: %d macro call for \"%s\" should be the only thing in the row\n",temp->og_line,macro_name);
			        temp = temp->next;
				continue;
			    }
			        
			/*reseting str*/
			memset(str,0,MAX_LINE); 
			/*laying the macro*/
			for(i = j = 0; j < strlen(macro); i++,j++)
			{
				/*if macro contents contains multiply '\n' each line has to be entered individually*/
				if(macro[j] == '\n'){
					str[i] = macro[j];
					/*inserting line at the desired location*/
					insertNodeAfterLine(line++,str,head);
					/*reseting str*/
					memset(str,0,MAX_LINE);
					/*i is set to minus one becuase it is increased by one at the next iteration which will
					set it to 0*/
					i = -1;	
				}
				else
					str[i] = macro[j];		
			}
			/*deleteing macro call line*/	
			deleteLine(head,temp->line_i);	
		}
	temp = temp->next;
	}	
}

/*
reading macros and laying them out in the line linked list
retruns 1 if .am file is needed for opening - meaning there are macros
returns 0 if there were no macros
returns -1 if a macro was found with an issue
*/
int pre_process(Line **head, resrvedNames **rnHead)
{
	char *macro = NULL,macro_name[MAX_LABEL_NAME];
	int i, error = 0;
    	char *badmac_names[37] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","get",
	                          "prn","jsr","rts","hlt","r0","r1","r2","r3","r4","r5","r6","r7",".data",
	                         "data",".struct","struct",".string","string",".entry","entry",".extern","extern","ERROR","macro","endmacro"};
	macros *macHead = NULL, *mac_temp = NULL;
	
	/*adding badmac_names to the reserved keywords linked list*/
	for(i = 0; i < 37;i++)
	    addLastResv(rnHead,1,badmac_names[i]);

	/*reading macros*/
	while((macro = read_macro(head,macro_name,rnHead)))
	{
		/*if macro reading was good*/
		if(strcmp(macro_name,"macro"))
			add_macro(&macHead,macro,macro_name);
		else
		    error++;
		free(macro);
		macro = NULL;
	}
	/*macro layout*/
	mac_temp = macHead;
	while(!error && mac_temp != NULL){
		macro_layout(mac_temp->macro,mac_temp->name,head);
		mac_temp = mac_temp->next;
	}
	
	if(macHead != NULL){
	    FREE_LIST(macros,macHead);
	    macHead = NULL;
	    if(error)
	        return -1;
	    return 1;
	}
	if(error)
	        return -1;
	return 0;    
}

