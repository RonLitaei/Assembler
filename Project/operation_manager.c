#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "data_structs.h"
#include "utils.h"
#include "conversions.h"
#include "prompt.h"
#include "action.h"
#include "file_opening.h"
/*
controls the entire code, manages each line and its type, sets the symbol table and code linked list
if no errors were found will open all the needed files
*/
int operation_manager(Line *line_head, resrvedNames **rn_head, char *file_name, int open_am)
{
    /*
    open_am stands for macro handling status
    -1 for an error with macros
    0 for no macros
    1 for macros with no errors
    */
    int DC = 0, IC = 100, result, error = 0, flag = 0;/*error will represent if there is an error or not, 1 for error, 0 for no error*/
    Line *temp_line = line_head;
    data_list *data_head = NULL, *data = NULL;
    entry *ent_head = NULL, *ent = NULL;
    symbols *sym_head = NULL,*sym = NULL;
    code *code_head = NULL, *cde = NULL;
    realoc_sym *rlc_sym = NULL,*rlc_temp = NULL;
    char *p = NULL;
    
    /*going through each line and determning its purpose*/
    while(temp_line != NULL){
        if(!(result = is_empty_sent(temp_line->line)))/*if a line is a white line*/
            if(!(result = is_comment_sent(temp_line->line)))/*if a line is a comment line*/
                if(!(result = is_prompt(&data_head,&sym_head,rn_head,&ent_head,&DC,temp_line->line,temp_line->line_i)))/*if a line is a data line*/
                    result = action_manager(temp_line->line,&rlc_sym,&sym_head,rn_head,&code_head,&IC,temp_line->line_i);/*if a line is an action line*/
        /*if the line type was not recognized*/
        if(!result)
            printf("ERROR IN LINE: %d, Cannot recognize line\n",temp_line->line_i);
        /*if there was an error*/    
        if(result == -1 || !result)
            error = 1;/*error*/
        temp_line = temp_line->next;
    }
    
    data = data_head;
    ent = ent_head;
    sym = sym_head;
    /*updating data type symbol adresses by adding IC to them*/
    while(sym != NULL){
        if(sym->data_type == 0){
            int k = binaryToDecimal(sym->bin_adress) + IC;
            strcpy(sym->bin_adress,p = decimalToBinary(k,10));
            free(p);
            p = NULL;
        }
        sym = sym->next;
    }
    
    /*adding data words after the action words*/
    while(data != NULL){
        p = decimalToBinary(data->data_word,10);
        add_code_to_Last(&code_head,p,&IC);
        free(p);
        p = NULL;
        data = data->next;
    }
    /*updating entry adresses, and determines if an entry has no source*/
    while(ent != NULL){
        sym = sym_head;
        while(sym != NULL){
            if(!strcmp(sym->label,ent->label)){
                ent->adress = binaryToDecimal(sym->bin_adress);
                break;
            }
            sym = sym->next;
        }
        if(sym == NULL){
            printf("WARNING: Entry \"%s\" has no source\n", ent->label);
            delete_entry(&ent_head,ent->label);
        }
        ent = ent->next;
    }
    
    /*second pass*/
    rlc_temp = rlc_sym;
    while(rlc_temp != NULL){
        flag = 0;
        sym = sym_head;
        cde = code_head;
        while(sym != NULL){
            /*finding the reallocated symbol in the symbol list*/
            if(!strcmp(rlc_temp->label,sym->label)){
                while(cde != NULL){
                    /*finding matching IC in the code list*/
                    if(cde->IC == rlc_temp->adress){
                        /*updating adress*/
                        /*if external*/
                        if(sym->data_type == 2)
                            strcpy((cde->bin_code),"0000000001");
                        else{    
                            strncpy(cde->bin_code, (sym->bin_adress)+2,8);
                            strcpy((cde->bin_code)+8,"10");
                        }
                        flag = 1;
                        break;
                    }
                    cde = cde->next;
                }
            }
            if(flag)
                break;
            sym = sym->next;
        }
        if(sym == NULL){
                    printf("ERROR IN LINE: %d, Label: \"%s\", declared as operand was not found\n",rlc_temp->line,rlc_temp->label);
                    error = 1;
                }
        rlc_temp = rlc_temp->next;
    }
    
    /*opening all relevant files if there were no errors*/
    if(!error && open_am != -1){
        open_ext_file(sym_head,rlc_sym,file_name);
        open_ent_file(ent_head,file_name);
        open_obj_file(code_head,DC,IC-100-DC,file_name);
        if(open_am)
            open_am_file(line_head,file_name);
    }
    
    
    FREE_LIST(realoc_sym,rlc_sym);
    FREE_LIST(data_list,data_head);
    FREE_LIST(entry,ent_head);
    FREE_LIST(symbols,sym_head);
    
    return error;
}
