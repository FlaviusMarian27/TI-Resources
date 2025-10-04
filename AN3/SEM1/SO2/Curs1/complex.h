#ifndef __COMPLEX_H
#define __COMPLEX_H

typedef struct{
    int real;
    int imaginar;
}COMPLEX;

void print_Complex(COMPLEX a);
COMPLEX add(COMPLEX a, COMPLEX b);

#endif 