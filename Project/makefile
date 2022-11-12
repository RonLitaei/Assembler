assembler: assembler.o operation_manager.o action.o prompt.o file_opening.o conversions.o pre_process.o data_structs.o pre_process_data_structs.o line_reserved_words_data_structs.o utils.o
	gcc -g -ansi -Wall -pedantic assembler.o operation_manager.o action.o prompt.o file_opening.o conversions.o pre_process.o data_structs.o pre_process_data_structs.o line_reserved_words_data_structs.o utils.o -lm -o assembler
assembler.o: assembler.c constants.h line_reserved_words_data_structs.h utils.h pre_process.h operation_manager.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
operation_manager.o: operation_manager.c constants.h utils.h line_reserved_words_data_structs.h data_structs.h conversions.h prompt.h action.h file_opening.h
	gcc -c -ansi -Wall -pedantic operation_manager.c -o operation_manager.o
action.o: action.c constants.h utils.h line_reserved_words_data_structs.h data_structs.h conversions.h
	gcc -c -ansi -Wall -pedantic action.c -o action.o
prompt.o: prompt.c constants.h utils.h line_reserved_words_data_structs.h data_structs.h conversions.h prompt.h
	gcc -c -ansi -Wall -pedantic prompt.c -o prompt.o
file_opening.o: file_opening.c constants.h utils.h line_reserved_words_data_structs.h data_structs.h file_opening.h conversions.h
	gcc -c -ansi -Wall -pedantic file_opening.c -o file_opening.o
conversions.o: conversions.c constants.h utils.h conversions.h line_reserved_words_data_structs.h
	gcc -c -ansi -Wall -pedantic conversions.c -o conversions.o
pre_process.o: pre_process.c pre_process.h pre_process_data_structs.h line_reserved_words_data_structs.h constants.h
	gcc -c -ansi -Wall -pedantic pre_process.c -o pre_process.o
data_structs.o: data_structs.c constants.h utils.h data_structs.h
	gcc -c -ansi -Wall -pedantic data_structs.c -o data_structs.o
pre_process_data_structs.o: pre_process_data_structs.c constants.h utils.h pre_process_data_structs.h line_reserved_words_data_structs.h
	gcc -c -ansi -Wall -pedantic pre_process_data_structs.c -o pre_process_data_structs.o
line_reserved_words_data_structs.o: line_reserved_words_data_structs.c line_reserved_words_data_structs.h utils.h constants.h
	gcc -c -ansi -Wall -pedantic line_reserved_words_data_structs.c -o line_reserved_words_data_structs.o	
utils.o: utils.c action.h constants.h conversions.h data_structs.h line_reserved_words_data_structs.h operation_manager.h pre_process.h pre_process_data_structs.h prompt.h utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o
