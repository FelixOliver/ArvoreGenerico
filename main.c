#include <stdio.h>
#include <stdlib.h>
#include "agenerico.h"

/*
tipo ->
* 1 = CIR , 2 = QUA, 3 = RET, 4 = TRA, 5 = TRI

*/

/*
1/0/TRI 3 2
2/1/RET 3 3
3/1/TRA 2 3 4
4/1/CIR 4
5/4/QUA 3
6/2/RET 2 2
7/5/TRA 1 2 3
8/5/CIR 2
9/4/QUA 1
10/1/TRI 1 2

*/

int main(){
    //char * ex = (char *)malloc(sizeof(char)*10);
    //ex = "23";
    //int *p;
    //p = pt_dados("23 43 1",3);
    //int * ss = (int *)malloc(sizeof(int));
    //ss[0] = 3;
    //TINF * th = ini_info();
    //th = cria_info(th, 2, ss);
    //printf("a:%f , t:%d , l:%d \n", th->area, th->tipo, th->dados[0]);
    int uno[2] = {3,2};
    int dos[2] = {3,3};
    int tres[3] = {2,3,4};
    int cuatro[1] = {4};
    int cinco[1] = {3};
    int seis[2] = {2,2};
    int siete[3] = {1,2,3};
    int ocho[2] = {2};
    int nueve[1] = {1};
    int diez[2] = {1,2};

    TAG * prin = NULL;
    //prin = ini_ag();
    //prin->cod = 2;
    //printf("%d", !prin);
    prin = insere(prin,1,0,"TRI",uno);
    printf("--------  \n");
    prin = insere(prin,2,1,"RET",dos);
    //printf("2- %d \n",prin->cod);
    printf("--------  \n");
    
    prin = insere(prin,3,1,"TRA",tres);
    prin = insere(prin,4,1,"CIR",cuatro);
    prin = insere(prin,5,4,"QUA",cinco);
    prin = insere(prin,6,2,"RET",seis);
    prin = insere(prin,7,5,"TRA",siete);
    prin = insere(prin,8,5,"CIR",ocho);
    prin = insere(prin,9,4,"QUA",nueve);
    prin = insere(prin,10,1,"TRI",diez);

    printf("hijo de root -> %d \n", prin->filho->cod);
    //printf("%d \n",prin->prox_irmao->cod);
    printf("-----------PRINT ARVORE-------------\n");
    print_ag(prin);

    return 0;
}