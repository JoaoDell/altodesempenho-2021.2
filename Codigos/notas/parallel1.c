#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(){

#pragma omp parallel for default(none) shared(a, b) reduction(+:s) schedule(static, 10)
{
/*Assim como devemos dar diretivas ao parallel -> default, shared, reduction, etc; o "for" também recebe diretivas 
(caso) o usuário queira, nesse caso, for -> schedule(), que recebe como serão divididos os blocos de processamento, 
e em quantas iterações n cada bloco será dividido o processamento do for pra cada thread.
No caso de como serão divididos, temos: 
static = os blocos são divididos no começo da execução exatamente em blocos de n 
dynamic = os blocos serão divididos dinamicamente de acordo com o programa

Exemplo: 
schedule(static, 10) = o for será dividido exatamente em blocos de 10 iterações para cada thread*/


/*O reduction(operação : variável) fará um processamento sem disputa de variáveis, distribuindo a variável incialmente
para cada thread como uma variável local (private), fazendo os calculos nela, e ao final realizando a operação especificada
com todas essa variáveis locais em uma variável global.
Exemplo:

reduction(+:s)
s += a*b 

No final, o processamento irá somar todas a variáveis em uma s só final*/

}

/*COMO RESOLVER DISPUTAS DE ACESSO A UM DADO NO OPENMP

O OpenMP, em sua biblioteca omp.h, possui uma série de funções e dados que recebem o nome de "lock". Essa propriedade lock é responsável por "trancar"
o acesso a um dado específico enquanto uma thread acessa ele, barrando todas as threads que acessariam ele ao mesmo tempo, e as colocando em uma fila
de espera.

Exemplo:

Suponha que eu tenho um dado:

typedef struct data{

    int buffer[BUFFER_SIZE];
    int n;
    omp_lock_t lock;

} data;

a variável "lock" acima fará o papel dessa tranca.

Ao processar esse dado "data" por várias threads, para evitar que várias threads a acessem ao mesmo tempo, se usa:

omp_set_lock(data.lock);

Processa-se os dados pela primeira thread que acessar. Enquanto isso, outras threads que tentarem acessar o data para processar irão verificar que 
o dado já está sendo acessado, entrarão na fila e esperarão pela sua vez de acessá-lo e processá-lo.

Ao final do processamento de uma thread:

omp_unset_lock(data.lock);

Para liberar aquele dado para o próximo processamento.


while(true){ <-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-
     ↓                                                                                                            ↑
     ↓                                                                                                            ↑
     ↓                                                                                                            ↑
omp_set_lock(data.lock)  -> (está trancado?) -sim-> entra na fila de processamento, aguarda a vez ->->->->->->->->↑
                                  ↓ não                                                                           ↑
                             tranca a fechadura                                                                   ↑
                                  ↓                                                                               ↑
                             processa dados                                                                       ↑      
                                  ↓                                                                               ↑          
omp_unset_lock(data.lock) <- terminou de processar                                                                ↑
    ↓                                                                                                             ↑
    ↓                                                                                                             ↑
    ↓                                                                                                             ↑ 
} ->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->↑


Como o Gonzalo disse, se assemelha ao uso de um banheiro: enquanto estiver alguém dentro, o banheiro está ocupado

*/






}