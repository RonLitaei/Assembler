#ifndef FILE_OPENING_H
#define FILE_OPENING_H 

/*
opens an .ent extension file and wrties the entries and their adresses in it
will also check if there are any entries, if not will not create file
*/
void open_ent_file(entry *ent_head, char *file_name);
/*
opens an .ext extension file and wrties the externs and the references to it
will also check if there are any externs, if not will not create file
*/
void open_ext_file(symbols *sym_head, realoc_sym *rlc_sym_head, char *file_name);
/*
opens an .ob extension file and wrties the each word and its adress in the unique 32 base
if no code exsits, will not create file
*/
void open_obj_file(code *cde_head, int DC, int IC,char *file_name);
/*opens .am extension file, this file is only opened if macros were defined and defined correctly*/
void open_am_file(Line *line_head,char *file_name);

#endif
