#include <stdio.h>

//É possível usar esse define para juntar funções direto no compilador
#define FMULT(f1, f2, x) f1(x)*f2(x)


int main(){

    printf("%f", FMULT(sin, cos, 0.7));

}