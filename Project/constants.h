#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>

#define MAX_LABEL_NAME 30
#define MAX_LINE 80
#define MAX_BINARY_SIZE 10
#define MAX_MACRO_MEM 800

#define FREE_LIST(type, head)\
do {\
    type *temp;\
    while (head != NULL) {\
        temp = (head)->next;\
        free(head);\
        head = temp;\
    }\
} while(0)


#endif
