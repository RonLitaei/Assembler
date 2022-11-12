#ifndef CONVERSIONS_H
#define CONVERSIONS_H

/*converts a binary numbers string(bin) to imaginary base 32*/
/*returns a string to represent the base 32 number*/
/*when using the function make sure to check the returned result != NULL*/
char * binaryTo32(char *bin);
/*converts binary numbers string(bin) to decimal, returns an int as decimal num*/
int binaryToDecimal(char *bin);
/*converts decimal numbers(num) to imaginary base 32*/
/*returns a string to represent the base 32 number*/
/*when using the function make sure to check the returned result != NULL*/
char * decimalTo32(long int num);
/*a function that converts decimal to binary*/
/*it returns a pointer to char of a binary represntation, num is the decimal number*/
/*to be converted and bits is the amount of bits to represent the binary outcome*/
/*if int is signed - can only support 8 bit (-128 to 127)*/
/*if int is unsigned - can only support 10 bits(0 to 1023)*/

char * decimalToBinary(int num, int bits);

#endif
