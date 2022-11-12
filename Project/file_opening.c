#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "data_structs.h"
#include "utils.h"
#include "conversions.h"
#include "file_opening.h"
/*
opens an .ent extension file and wrties the entries and their adresses in it
will also check if there are any entries, if not will not create file
*/
void open_ent_file(entry *ent_head, char *file_name){
    entry *ent = ent_head;
    FILE *fptr;
    char *p = NULL, *tmp_file_name = (char*) calloc(strlen(file_name), sizeof(char));/* creating a temp string to hold file name with .ent etension*/
    
    check_mem_alloc(tmp_file_name);
    strncpy(tmp_file_name,file_name,strlen(file_name));
    strcat(tmp_file_name,".ent");
    tmp_file_name[strlen(tmp_file_name)] = '\0';
    
    if(ent_head == NULL){
        free(tmp_file_name);
        return;
    }
        
    fptr = fopen(tmp_file_name,"w");
    if(fptr == NULL)
    {
        printf("Failed opening \"%s\" file\n",tmp_file_name);
        free(tmp_file_name);
        return;
    }
    free(tmp_file_name);
    while(ent != NULL){
        fprintf(fptr,"%-10s%s\n",ent->label,p = decimalTo32(ent->adress));
        free(p);
        p = NULL;
        ent = ent->next;
    }
    fclose(fptr);
}

/*
opens an .ext extension file and wrties the externs and the references to it
will also check if there are any externs, if not will not create file
*/
void open_ext_file(symbols *sym_head, realoc_sym *rlc_sym_head, char *file_name){
    FILE *fptr;
    symbols *sym = sym_head;
    realoc_sym *rlc = rlc_sym_head;
    char *p = NULL,*tmp_file_name = (char*) calloc(strlen(file_name), sizeof(char));
    
    check_mem_alloc(tmp_file_name);
    strncpy(tmp_file_name,file_name,strlen(file_name));
    strcat(tmp_file_name,".ext");
    tmp_file_name[strlen(tmp_file_name)] = '\0';
    
    /*checks if there is an extern in the file*/
    while(sym != NULL){
        if(sym->data_type == 2)
            break;
        sym = sym->next;    
    }
    /*if no extern was found*/
    if(sym == NULL){
        free(tmp_file_name);
        return;
    }
        
    fptr = fopen (tmp_file_name,"w");
    if(fptr == NULL)
    {
        printf("Failed opening \"%s\" file\n",tmp_file_name);
        free(tmp_file_name);
        return;
    }
    free(tmp_file_name);
    sym = sym_head;
    while(sym != NULL){
        /*found extern*/
        if(sym->data_type == 2){
            rlc = rlc_sym_head;
            /*finding matching label in the reallocated symbol list*/
            while(rlc != NULL){
                if(!strcmp(sym->label,rlc->label)){
                    fprintf(fptr,"%-10s%s\n",sym->label,p = decimalTo32(rlc->adress));
                    free(p);
                    p = NULL;
                }
                rlc = rlc->next;
            }
        }
        sym = sym->next;
    }
    fclose(fptr);
}

/*
opens an .ob extension file and wrties the each word and its adress in the unique 32 base
if no code exsits, will not create file
*/
void open_obj_file(code *cde_head, int DC, int IC,char *file_name){
    code *cde = cde_head;
    FILE *fptr;
    char *p = NULL, *q = NULL,*tmp_file_name = (char*) calloc(strlen(file_name), sizeof(char));/*creating a temp string to hold file name with .ob extension*/
    check_mem_alloc(tmp_file_name);
    strncpy(tmp_file_name,file_name,strlen(file_name));
    strcat(tmp_file_name,".ob");
    tmp_file_name[strlen(tmp_file_name)] = '\0';
    
    if(cde_head == NULL)
        return;
        
    fptr = fopen (tmp_file_name,"w");
    if(fptr == NULL)
    {
        printf("Failed opening \"%s\" file\n",tmp_file_name);
        return;
    }
    free(tmp_file_name);
    /*prints to file the DC & IC*/
    fprintf(fptr,"   %s %s\n",q = decimalTo32(IC),p = decimalTo32(DC));
    free(p);
    free(q);
    q = p = NULL;
    
    while(cde != NULL){
        fprintf(fptr,"%s\t %s\n",q = decimalTo32(cde->IC),p = binaryTo32(cde->bin_code));
        free(p);
        free(q);
        q = p = NULL;
        cde = cde->next;
    }
    fclose(fptr);
}
/*opens .am extension file, this file is only opened if macros were defined and defined correctly*/
void open_am_file(Line *line_head,char *file_name){
    FILE *file;
    int i;
    Line *temp = line_head;
    /*creating a temp string to hold file name with .am extension*/
    char *tmp_file_name = (char*) calloc(strlen(file_name), sizeof(char));
    check_mem_alloc(tmp_file_name);
    strncpy(tmp_file_name,file_name,strlen(file_name));
    strcat(tmp_file_name,".am");
    tmp_file_name[strlen(tmp_file_name)] = '\0';
	
	file = fopen(tmp_file_name,"w");
	free(tmp_file_name);
	tmp_file_name = NULL;
	
	while(temp != NULL)
	{
		for(i = 0; i < strlen(temp->line);i++)
			fputc(temp->line[i],file);/*each char in the line*/
		temp = temp->next;/*next line*/
	}
		
	fclose(file);
}
