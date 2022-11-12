#include "constants.h"
#include "line_reserved_words_data_structs.h"
#include "utils.h"
#include "conversions.h"

/*converts a binary numbers string(bin) to imaginary base 32*/
/*returns a string to represent the base 32 number*/
/*when using the function make sure to check the returned result != NULL*/
char * binaryTo32(char *bin)
{
    int k;
    char *result = (char*) calloc(MAX_LABEL_NAME,sizeof(char));
    char *temp = NULL;
    check_mem_alloc(result);
    
    /*first converting the binary to decimal*/
    k = binaryToDecimal(bin);
    /*second converting the decimal to 32*/
    temp = decimalTo32(k);
    strcpy(result,temp);
    free(temp);
    
    return result;
}

/*converts binary numbers string(bin) to decimal, returns an int as decimal num*/
int binaryToDecimal(char *bin)
{
    int dec = 0, base = 1, r, num = 0,i = 0;
    
    /*passing all left unused bits*/
    while(i < strlen(bin) && bin[i] != '0')
        i++;
    num = atoi(bin);
    
    while (num > 0)  
    {  
        r = num % 10;  
        dec = dec + r * base;  
        num = num / 10;  
        base = base * 2;  
    }
    return dec;
}

/*converts decimal numbers(num) to imaginary base 32*/
/*returns a string to represent the base 32 number*/
/*when using the function make sure to check the returned result != NULL*/
char * decimalTo32(long int num)
{
    /*our imaginary 32 base*/
    char base_arr[32] =
	    {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>', 'a', 'b', 'c', 'd', 'e', 'f','g','h'
	    ,'i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
    int conv_num[10];
    int base = 32, i=0, len = 0;
    char *result = (char*) calloc(MAX_LABEL_NAME,sizeof(char));
    char c[2], temp;
    
    /*if calloc failed*/
    check_mem_alloc(result);
    
    /*reseting array*/
    for(; i < MAX_BINARY_SIZE; i++)
        conv_num[i] = 0;
    i = 0;    

    /* the conversion */
    while (num != 0)
    {
	    conv_num[i] = num % base;
	    num = num / base;
	    i++;
    }
     /* dec "i" since the last element does not exists */
    if(i) 
        --i;
    
    /* traversing the array backwards */
    for(; i>=0; i--)
    {
        c[0] = base_arr[conv_num[i]];
	    strncat(result,c,1);/*adding each "digit" to the result*/
	    len++;
    }
    
    /*null terminator*/
    result[len] = '\0';
    if(strlen(result) <= 1){
        temp = result[0];
        result[0] = '!';
        result[1] = temp;
    }
    return result;	 
}

/*a function that converts decimal to binary*/
/*it returns a pointer to char of a binary represntation, num is the decimal number*/
/*to be converted and bits is the amount of bits to represent the binary outcome*/
/*limit for this function is -513<x<1024*/

char * decimalToBinary(int num, int bits){
    unsigned int n = num;
    int i = 0;
    char temp, *result = (char *) calloc((bits + 1), sizeof(char));
    check_mem_alloc(result);
    /*if there is not enough bits to display the number*/
    if(abs(num) >= pow(2,bits) || bits > 10 || num > 1023 || num < -512){
        if(bits > 10 || num > 1023 || num < -512)
            printf("max bits is 10\n");
        else    
            printf("%d has more bits than %d can provide\n",n,bits);
        return NULL;
    }
    /*assigning bits to *result*/
    while(i < bits){
        result[i] = n % 2 + '0';
        n=n/2;
        i++;
    }
    /*swapping positions from start to end*/
    for (i = 0; i < bits /2 ;i++){
        temp = result[bits - 1 - i];
        result[bits - 1 - i] = result[i];
        result[i] = temp;
    }
    if(num < 0)
        result[0] = '1';
    /*null terminator*/
    result[bits] = '\0';
    return result;

}
