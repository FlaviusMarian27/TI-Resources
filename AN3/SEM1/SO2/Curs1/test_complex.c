#include<stdio.h>
#include "complex.h"

int main(void){
    COMPLEX number;
    scanf("%d %d",&number.real,&number.imaginar);
    print_Complex(number);

    COMPLEX number1;
    scanf("%d %d",&number1.real,&number1.imaginar);
    print_Complex(number1);

    printf("ADD:\n");
    print_Complex(add(number,number1));

    return 0;
}