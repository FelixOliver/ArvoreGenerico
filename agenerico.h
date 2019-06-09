#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
tipo ->
* 1 = CIR , 2 = QUA, 3 = RET, 4 = TRA, 5 = TRI


(a) buscar figuras geométricas, por meio de um código único; < 
(b) imprimir informações relevantes, tanto da árvore, quanto das figuras, incluindo-se sua área; < 
(c) inserir novas figuras; < 
(d) retirar figuras, passando seus descendentes para outro pai; <
(e) destruir a árvore; <
(f) alterar as dimensões de figuras; <
*/
typedef struct info
{
    char * tipo;
    int * dados;
    int size_d;
    float area;
}TINF;
// 


// cria info y calcula seu area
// os dados 12 3 4 , b B h

TINF * ini_info()
{
    TINF * novo = (TINF *)malloc(sizeof(TINF));
    novo->dados = NULL;
    novo->tipo = (char *)malloc(sizeof(char)*3);
    
    return novo;
}

TINF * cria_info(TINF * inf,char * tipo, int * dados)
{
    //TINF * inf = (TINF *)malloc(sizeof(TINF));
    strcpy(inf->tipo , tipo);

    inf->dados = dados;    

   if(strcmp(tipo, "CIR") == 0){
           inf->size_d = 1;
           inf->area =  M_PI * powf(dados[0], 2.0 );      
   }
    if(strcmp(tipo, "QUA") == 0){
           inf->size_d = 1;
           inf->area = dados[0] * dados[0];
    }
    if(strcmp(tipo, "RET") == 0){
            inf->size_d = 2;
            inf->area = dados[0] * dados[1];
    }
    if(strcmp(tipo, "TRA") == 0){
            inf->size_d = 3;
            inf->area = (dados[1]+dados[0]) * dados[2] / 2.0;
    }
    if(strcmp(tipo, "TRI") == 0){
            inf->size_d = 2;
            inf->area = dados[0] * dados[1];
    }

    return inf;
}


/*******************************************/

typedef struct ag{
    int cod;
    TINF * info;
    struct ag * filho;
    struct ag * prox_irmao;
}TAG;

/** tecnicas de AG**/

TAG * ini_ag()
{
    TAG * novo = (TAG *)malloc(sizeof(TAG));    
    novo->cod = 0;
    novo->filho = NULL;
    novo->prox_irmao = NULL;
    novo->info = NULL;
    return novo;
}

TAG * insere(TAG * ag, int cod, int cod_pai, char * tipo, int * dados){
    if(ag == NULL)
    {    
        //printf("entro if !ag \n");   
        if(cod_pai == 0)
        {
            //root
            ag = ini_ag();
            ag->cod = cod;
            ag->info = ini_info();
            ag->info = cria_info(ag->info, tipo, dados);
            
            //printf("Creo ARBOL y inserto: %d \n",ag->cod);
            //return ag;
        }else{
            ag = NULL;
            //printf("Retornare NULL\n");
        }  
    }
    else if(ag->cod == cod_pai )
    {   
        TAG * novo = ini_ag();
        novo->cod = cod;
        novo->info = ini_info();
        novo->info = cria_info(novo->info, tipo, dados);

        if(ag->filho == NULL)
        {
            ag->filho = novo;
            //printf("Agrego hijo %d a %d \n",ag->filho->cod,ag->cod);
            //return ag;
        }
        else
        {
            TAG * p = ini_ag();
            p = ag->filho;
            //printf("antes de while p filho: %d\n",p->cod);
            while (p->prox_irmao != NULL) p = p->prox_irmao;
            
            //printf("antes de isertar ultimo irmao: %d\n",p->cod);
            
            p->prox_irmao = novo;
            
            //printf("inserto hermano -> %d \n",p->prox_irmao->cod);
        }
        //return ag;
    }
    else if(ag->filho != NULL || ag->prox_irmao != NULL )
    {
        //printf("Ingreso a funcion para inserar en hijo\n");
        ag->filho =  insere(ag->filho, cod, cod_pai, tipo, dados);
        //printf("retorne de hijo \n");
        //printf("Ingreso a funcion para inserar al hermano\n");
        ag->prox_irmao =  insere(ag->prox_irmao, cod, cod_pai, tipo, dados);
        //printf("retorne de hermano \n");
    }
    
    return ag;
}


void print_figurinha(TAG * no)
{
    printf("-----------------------------------------------------------------------------\n");
    printf("Cod nó: %d \t Figurinha: %s \n", no->cod, no->info->tipo);
    printf("Dimensoes => ");
    
    if(strcmp(no->info->tipo, "CIR") == 0){
        printf("radio: %d \n",no->info->dados[0]);
    }
    if(strcmp(no->info->tipo, "QUA") == 0){
        printf("lado: %d \n",no->info->dados[0]);   
    }
    if(strcmp(no->info->tipo, "RET") == 0){
        printf("base: %d , altura: %d \n",no->info->dados[0], no->info->dados[1] );
    }
    if(strcmp(no->info->tipo, "TRA") == 0){
        printf("b-menor: %d , b-mayor: %d , altura: %d \n",no->info->dados[0], no->info->dados[1], no->info->dados[2]);
    }
    if(strcmp(no->info->tipo, "TRI") == 0){
        printf("base: %d , altura: %d \n",no->info->dados[0], no->info->dados[1] );
    }

    printf("Area: %f \n", no->info->area);
    printf("-----------------------------------------------------------------------------\n");
}

void print_ag(TAG * a)
{
    if(a == NULL)
    {
        return;
    }

    print_ag(a->filho);
    print_figurinha(a);// imprimir dado do figurinha
    if(a->filho != NULL)
        printf("nodo %d ->hijo-> %d\n",a->cod ,a->filho->cod);
    print_ag(a->prox_irmao);
    if(a->prox_irmao != NULL)
        printf("nodo %d ->irmao-> %d\n",a->cod ,a->prox_irmao->cod);
    return;
}

TAG * buscar_pelo_codigo(TAG * arv, int cod){
    TAG * rpta;

    if(arv == NULL) return NULL;
    if(arv->cod == cod) return arv;
    
    rpta =  buscar_pelo_codigo(arv->filho, cod);
    if(rpta!=NULL) return rpta;
    rpta = buscar_pelo_codigo(arv->prox_irmao, cod);
    if(rpta!=NULL) return rpta;

    return NULL;
}

//return pai *

TAG * get_pai(TAG * arv, TAG * filho)
{   // no existe arv, no existo filho, arv no tiene filjho, cuando filho = arv ,,
    //if(arv == NULL || filho == NULL || arv->filho == NULL || arv->cod == filho->cod) return NULL;
    

}


//(f) alterar as dimensões de figuras;

void alterar_dimensoes(TAG * no, int * dados)
{
    
    if (dados == NULL || !dados) return;
    int i;
    for ( i = 0; i < no->info->size_d; i++)
    {
        if(!dados[i]) return;
    }
    for (i = 0; i < no->info->size_d; i++)
        no->info->dados[i] = dados[i];
    return;    
}

void menu_alterar_dimensoes(TAG * arv)
{
    int cod;
    printf("(f)Alterar dimensoes \n Escriba codigo do figurinha: ");
    scanf("%d", &cod);
    // buscar no
    printf("cod: %d\n",cod);
    TAG * aux = buscar_pelo_codigo(arv, cod);

    printf("prueba %d \n", aux->cod);
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
    return;
}

///////////////////

//(d) retirar figuras, passando seus descendentes para outro pai;

// retirar figura, passando seus decentes para outro pai, 1 si retiro, 0 no retiro


TAG * retirar_figura(TAG * arv,int cod, TAG * pai)
{ 
    TAG * rpta;

    if(arv == NULL) return NULL;
    //if(arv->cod == cod) return arv;
    
    rpta = retirar_figura(arv->filho, cod, pai);
    
    if(rpta!= NULL && rpta->cod == cod)
    {   // cambiar hijo de pai, é o irmao
        
        //print_figurinha(rpta);
        //print_figurinha(arv->filho);
        
        TAG * temp, * aux; 
        temp = rpta;
        arv->filho = rpta->prox_irmao;
        
        // trocar do pai
        //aux = buscar_pelo_codigo(pai);//buscar novo pai
        
        printf("primer if\n");
        print_figurinha(pai);
        
        if(pai->filho != NULL)
        {   
            printf("primer if 1\n");
            aux = pai->filho;
            while (aux->prox_irmao != NULL ){
                aux = aux->prox_irmao;
            }
            aux->prox_irmao = temp->filho;
            // liberar temp
            free(temp);
        }else if (pai->filho == NULL){
            printf("primer if 2\n");
            pai->filho = temp->filho;
            print_figurinha(pai->filho);
            // liberar temp
            free(temp);
        }
        
        
    } //return rpta;
    rpta = retirar_figura(arv->prox_irmao, cod, pai);
    
    if(rpta!= NULL && rpta->cod == cod) //return rpta;
    {// cambiar info
        TAG * temp, * aux;
        temp = rpta;
        arv->prox_irmao = rpta->prox_irmao;

        // trocar do pai
        //aux = buscar_pelo_codigo(pai);//buscar novo pai
        printf("segundo if\n");
        print_figurinha(pai);
        if(pai->filho != NULL){
            printf("segundo if 1\n");
            aux = pai->filho;
            while (aux->prox_irmao != NULL ){
                aux = aux->prox_irmao;
            }
            aux->prox_irmao = temp->prox_irmao;
            // liberar temp
            free(temp);

        }else if (pai->filho == NULL){
            printf("segundo if 1\n");

            pai->filho = temp->prox_irmao;
            print_figurinha(pai->filho);
            // liberar temp
            free(temp);
        }

    }
    return arv;
}

////////////////

//(e) destruir a árvore;

void destruir_ag(TAG * a)
{
    if(a == NULL)
    {
        return;
    }

    destruir_ag(a->filho);
    //print_figurinha(a);// imprimir dado do figurinha
    //if(a->filho != NULL)
    //    printf("nodo %d ->hijo-> %d\n",a->cod ,a->filho->cod);
    destruir_ag(a->prox_irmao);
    print_figurinha(a);
    free(a);
    //if(a->prox_irmao != NULL)
     //   printf("nodo %d ->irmao-> %d\n",a->cod ,a->prox_irmao->cod);
    return;
}