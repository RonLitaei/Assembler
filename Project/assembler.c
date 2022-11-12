#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"
#include "pre_process.h"
#include "operation_manager.h"

int main(int argc, char *argv[])
{
    int i;
    if(argc < 2){
        printf("No files were entered\n");
        exit(0);
    }
    for(i = 1; i < argc;i++){
        int is_macro = 0, result = 1;
        /*setting linked lists for main use*/
        resrvedNames *rn_head = NULL;
        Line *line_head = NULL;
        /*copying file name and adding .as extension to it*/
        char *temp = (char*) calloc(strlen(argv[i]), sizeof(char));
        check_mem_alloc(temp);
        strncpy(temp,argv[i],strlen(argv[i]));
        strcat(temp,".as");
        temp[strlen(temp)] = '\0';
        
        
        printf("\nStarting assembler process for file: \"%s\"\n",temp);
        /*if reading file was successfull*/
        if(readFile(temp,&line_head)){
            /*if there are macros*/
            is_macro = pre_process(&line_head,&rn_head);
            /*managing operation based on if there were macros or not*/
            result = operation_manager(line_head,&rn_head,argv[i],is_macro);
            /*freeing lists*/
            FREE_LIST(Line,line_head);
            FREE_LIST(resrvedNames,rn_head);
        }
        /*if everything worked correctly*/
        if(!result)
            printf("File: \"%s\" assembled successfully\n",temp);
        /*if an issue occured*/    
        else
            printf("File: \"%s\" failed to assemble\n",temp);
        /*freeing temp file name*/    
        free(temp);
        temp = NULL;
    }

    return 0;
}
