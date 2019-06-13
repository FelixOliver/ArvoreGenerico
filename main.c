#include <stdio.h>
#include <stdlib.h>
#include "arvores.h"
#include <ctype.h>

/*
(a) buscar figuras geométricas, por meio de um código único; < 
(b) imprimir informações relevantes, tanto da árvore, quanto das figuras, incluindo-se sua área; < 
(c) inserir novas figuras; < 
(d) retirar figuras, passando seus descendentes para outro pai; <
(e) destruir a árvore; <
(f) alterar as dimensões de figuras; <
*/

//******************************MENUS**************************************

void a_buscar(TAG * arv)
{
    printf(">-1------------------------------Buscar---------------------------------------------< \n");
    
    if(arv == NULL){
        printf("######################################################\n");
        printf("Arvore Generico nao tem dados o foi destruido!!!\n");
        printf("######################################################\n");
        return;
    }
    int cod;
    TAG * aux;
    while (1)
    {
        printf("> Insere o codigo do nó: \n >");
        scanf("%d",&cod);
        aux = buscar_pelo_codigo(arv , cod);
        if(aux!=NULL)
            break;
        if(cod == -1)return;
    }

    print_figurinha(aux);

}

void b_imprimir(TAG * arv)
{
    if(arv!=NULL){
        printf(">-1------------------------------ImprimirArvore----------------------------------------< \n");
        print_ag(arv);
        // falta imprimir con DOT
        generate_dot_ag(arv);
    }
    else{
        printf("######################################################\n");
        printf("Arvore Generico nao tem dados o foi destruido!!!\n");
        printf("######################################################\n");
    }
}

TAG * c_inserir(TAG * arv)
{
    printf(">-1------------------------------Inserir---------------------------------------------< \n");
    
    TAG * aux = NULL;
    int cod, cod_pai;
    while (1)
    {
        
        printf("> Insere o codigo do nó: \n >");
        scanf("%d",&cod);
        //para salir de menu////////////
        if(cod == -1)return arv;
        
        aux = buscar_pelo_codigo(arv, cod);
        if(aux==NULL && cod >0 && arv!=NULL){
            TAG * aux2 = NULL;
            while (1)
            {
                printf("> Insere o codigo do pai do nó: \n >");
                scanf("%d",&cod_pai);
                //para salir de menu////////////
                if(cod_pai == -1)return arv;
                
                aux2 = buscar_pelo_codigo(arv, cod_pai);
                if (aux2 != NULL && cod_pai > 0)
                {
                    break;
                }
                
            }        
            
            break;
        }
        else if(arv==NULL){
            cod_pai=0;
            break;
        }
    }
    ////////////
        
    char * str_t = (char *)malloc(sizeof(char)*10);
    
    while (1)
    {
    
        printf("Insere tipo do figurinha \nOpcoes do figurinha; TRI, QUA, RET, CIR, TRA\n >");
        
               
        scanf("%s", str_t);       
        //para salir de menu////////////
        if(strcmp(str_t, "-1") == 0)return arv;
        /////////////
        if(strcmp(str_t, "CIR") == 0){
            
            printf("escriva dimensoes \n");
                        
            int * dados;
            dados = (int *) malloc(sizeof(int));
            printf("radio: ");
            scanf("%d", &dados[0]);
            
            arv = insere(arv, cod, cod_pai, str_t, dados);
            
            break;
        }
        else if(strcmp(str_t, "QUA") == 0){
            
            printf("escriva dimensoes \n");
            
            
            int * dados;
            dados = (int *) malloc(sizeof(int));
            printf("lado: \n");
            scanf("%d", &dados[0]);

            arv = insere(arv, cod, cod_pai, str_t, dados);
                
            break;
        }
        else if(strcmp(str_t, "RET") == 0){
            
            printf("escriva dimensoes \n");
            
            int * dados;
            dados = (int *) malloc(sizeof(int) * 2);
            printf("base: ");
            scanf("%d", &dados[0]); 
            printf("altura: ");
            scanf("%d", &dados[1]);
            
            arv = insere(arv, cod, cod_pai, str_t, dados);                
            
            break;
        }
        else if(strcmp(str_t, "TRA") == 0){
            
            printf("escriva dimensoes \n");
            
            int * dados;
            dados = (int *) malloc(sizeof(int) * 3 );

            printf("base menor: ");
            scanf("%d", &dados[0]);
            printf("base mayor: ");
            scanf("%d", &dados[1]);
            printf("altura: ");
            scanf("%d", &dados[2]);

            arv = insere(arv, cod, cod_pai, str_t, dados);
                 
            break;            
        }
        else if(strcmp(str_t, "TRI") == 0){
            
            printf("escriva dimensoes \n");
            int * dados;
            dados = (int *) malloc(sizeof(int) * 2);            

            printf("base: ");
            scanf("%d", &dados[0]);
            printf("altura: ");
            scanf("%d", &dados[1]);
                
            arv = insere(arv, cod, cod_pai, str_t, dados);
            break;        
        }
        
    }   
    
    return arv;
}

TAG * d_retirar(TAG * arv)
{
    if(arv!=NULL){
        printf(">-1------------------------------Retirar-----------------------------------------------< \n");
        printf(".Novo pai nao poder ser filho do nó para retirar \n");
        TAG * aux = NULL;
        int cod, cod_pai;
        while (1)
        {
            
            printf("> Insere o codigo do nó: \n >");
            scanf("%d",&cod);
            //para salir de menu////////////
            if(cod == -1)return arv;
            
            aux = buscar_pelo_codigo(arv, cod);
            if(aux!=NULL && cod > 1) // el nodo a retirar no puede ser ROOT
            {
                TAG * pai = NULL;
                while (1)
                {
                    printf("> Insere o codigo do novo pai do nó: \n >");
                    scanf("%d",&cod_pai);
                    //para salir de menu////////////
                    if(cod_pai == -1)return arv;

                    pai = buscar_pelo_codigo(arv, cod_pai);
                    if (pai != NULL && buscar_pelo_codigo(aux->filho , cod_pai) == NULL && cod_pai > 0)//novo pai nao pode ser filho do -> nos filhos a retirar
                    {
                        arv = retirar_figura(arv, cod, pai);
                        break;
                    }
                    
                }        
                
                break;
            }
        }
        return arv;
    }
    else{
        printf("######################################################\n");
        printf("Arvore Generico nao tem dados o foi destruido!!!\n");
        printf("######################################################\n");
        return NULL;

    }
}

void e_destruir(TAG ** arv)
{
    if(*arv!=NULL){
        printf(">-1------------------------------Destruir---------------------------------------------< \n");
        destruir_ag(*arv);
        *arv=NULL;
    }
    else{
        printf("######################################################\n");
        printf("Arvore Generico nao tem dados o foi destruido!!!\n");
        printf("######################################################\n");
    }
    
}

TAG * f_alterar(TAG * arv)
{
     if(arv!=NULL){
        printf(">-1------------------------------Alterar-----------------------------------------------< \n");
        TAG * aux = NULL;
        int cod;
        while (1)
        {
            
            printf("> Insere o codigo do nó: \n >");
            scanf("%d",&cod);
            //para salir de menu////////////
            if(cod == -1)return arv;

            aux = buscar_pelo_codigo(arv, cod);
            if(aux!=NULL){
                break;
            }
        }

        //printf("prueba %d \n", aux->cod);
        print_figurinha(aux);

        int * dados;
        if(strcmp(aux->info->tipo, "CIR") == 0){
            printf("seu figurinha é %s \n", aux->info->tipo);
            printf("escriva novas dimensoes \n");
            dados = (int *) malloc(sizeof(int) * aux->info->size_d);
            
            printf("radio: ");
            scanf("%d", &dados[0]);

            aux->info->area = M_PI * powf(dados[0], 2.0);
        }
        if(strcmp(aux->info->tipo, "QUA") == 0){
            printf("seu figurinha é %s \n", aux->info->tipo);
            printf("escriva novas dimensoes \n");
            dados = (int *) malloc(sizeof(int) * aux->info->size_d);
            
            printf("lado: \n");
            scanf("%d", &dados[0]);

            aux->info->area = powf(dados[0], 2.0);           
        }
        if(strcmp(aux->info->tipo, "RET") == 0){
            printf("seu figurinha é %s \n", aux->info->tipo);
            printf("escriva novas dimensoes \n");
            dados = (int *) malloc(sizeof(int) * aux->info->size_d);
            
            printf("base: ");
            scanf("%d", &dados[0]); 
            printf("altura: ");
            scanf("%d", &dados[1]);

            aux->info->area = dados[0] * dados[1];
        }
        if(strcmp(aux->info->tipo, "TRA") == 0){
            printf("seu figurinha é %s \n", aux->info->tipo);
            printf("escriva novas dimensoes \n");
            dados = (int *) malloc(sizeof(int) * aux->info->size_d);
            
            printf("base menor: ");
            scanf("%d", &dados[0]);
            printf("base mayor: ");
            scanf("%d", &dados[1]);
            printf("altura: ");
            scanf("%d", &dados[2]);

            aux->info->area = (dados[1] + dados[0]) * dados[2] / 2.0 ;
            
        }
        if(strcmp(aux->info->tipo, "TRI") == 0){
            printf("seu figurinha é %s \n", aux->info->tipo);
            printf("escriva novas dimensoes \n");
            dados = (int *) malloc(sizeof(int) * aux->info->size_d);
            
            printf("base: ");
            scanf("%d", &dados[0]); 
            printf("altura: ");
            scanf("%d", &dados[1]);

            aux->info->area = dados[0] * dados[1];
        }
        alterar_dimensoes(aux, dados);

        print_figurinha(aux);

        return arv;
    }
    else{
        printf("######################################################\n");
        printf("Arvore Generico nao tem dados o foi destruido!!!\n");
        printf("######################################################\n");
        return NULL;
    }
}
/*TAG * reload_tree(){
    char name[20];
    printf("Insere nome de arquivo:\n");
    scanf("%s",name);
    printf("%s\n",name);
    TAG * arv=NULL;
    arv = load_tree(arv,name);
    return arv;

}*/
int main (int argc, char *argv[] ){
    
    TAG * arv = NULL;

    //printf(" name: %s \n", argv[1]);
    if(!argv[1]){
        printf("Insere nome do arquivo, por exemplo: file.txt \n");
        return 0;
    }
    
    arv = load_tree(arv, argv[1]);
    int num;
    
    while(num != -1){
        printf("###################################################################################################\n");
        printf("##################################### SISTEMA DO ARVORE GENERICO ##################################\n");
        printf("###################################################################################################\n");
        printf("ESCOGA UNA DE AS OPCOES ABAXO.\n");
        //printf("COM -1 EXIT TO MENU PRINCIPAL E SAIR DO SISTEMA.\n");
        printf("SOAMENTE ACEITAMOS NUMEROS PARA OS CODIGOS.\n");
        printf(">-------------------------------Operacoes do arvore--------------------------------------------- < \n");
        printf("(1) buscar figuras geométricas, por meio de um código único < \n");
        printf("(2) imprimir informações relevantes, tanto da árvore, quanto das figuras, incluindo-se sua área e generacao do grafico dot< \n"); 
        printf("(3) inserir novas figuras < \n"); 
        printf("(4) retirar figuras, passando seus descendentes para outro pai < \n");
        printf("(5) destruir a árvore < \n");
        printf("(6) alterar as dimensões de figuras < \n");
        printf(">-------------------------------Transformacoes-------------------------------------------------- < \n");
        printf("(7) transformar a árvore genérica numa árvore binária de busca balanceada, baseando-se no código único e generacao do grafico dot< \n");
        printf("(8) converter a árvore genérica numa árvore B, baseando-se no código único e generacao do grafico dot < \n");
        //printf("(9) reload arvore from arquivo < \n");

        printf("(-1) SAIR DO SISTEMA E SAIR A MENU< \n >");

        scanf("%i", &num);
        
        switch (num)
        {
        case 1:
                a_buscar(arv);
            break;
        case 2:
                b_imprimir(arv);
            break;
        case 3:
                arv = c_inserir(arv);            
            break;
        case 4:
                arv = d_retirar(arv);
            break;
        case 5:
                if(arv!=NULL) e_destruir(&arv);
                else{
                    printf("#################################\n");
                    printf("->>>>>>arvore ja foi destruido>>>>\n");
                    printf("#################################\n");

                } 
            break;
        case 6:
                arv = f_alterar(arv);            
            break;
        case 7:
                // to -> abb
                create_and_print_avl(arv);
            break;
        case 8:
                // to -> aB
                create_and_print_b(arv);
            break;
        
        /*case 9:
                if(arv==NULL) arv=reload_tree();
                else{
                    printf("#################################\n");
                    printf("->>>>>>arvore ja criado>>>>\n");
                    printf("#################################\n");
                }
                break;*/
        case -1:
            destruir_ag(arv);
            return 0;
            break;        
        default:
            break;
        }
       
    }

    //print_figurinha(arv);
    return 0;
}

