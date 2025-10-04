#include "complex.h"
#include<stdio.h>

void print_Complex(COMPLEX a){
    printf("%d + %dj\n",a.real,a.imaginar);
}

COMPLEX add(COMPLEX a, COMPLEX b){
    COMPLEX c;
    c.real = a.real + b.real;
    c.imaginar = a.imaginar + b.imaginar;
    return c;
}