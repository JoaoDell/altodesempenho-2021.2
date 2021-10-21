#include <stdio.h>
//Um uso interessante de pre-processadores:
#define str(s) #s
//Definiu-se uma função "str" na etapa de pré-processamento que pega uma entrada s e transforma ela em string literal de C

int main(){


    printf(str(carai));
}