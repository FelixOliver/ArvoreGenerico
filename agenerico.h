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

#define MAXCHAR 50
char str[MAXCHAR];

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


TAG * load_tree(TAG * tag, char * nfile){
    FILE *fptr;
    if ((fptr = fopen(nfile ,"r")) == NULL){
        printf("Error! opening file \n");
        exit(1);
    }
    while(fgets(str,MAXCHAR,fptr) != NULL){
        //token(str);
        int i=0,j=0,v=0,k,d,*values;
        char * name = (char *) malloc(sizeof(char));
        values = (int *) malloc(sizeof(int));
        char * tmp=(char *) malloc(sizeof(char));
        while(str[i]!='/'){
            j++;
            tmp=realloc(tmp,(sizeof(char)*j));
            tmp[i]=str[i];
            i++;
        }
        k=atoi(tmp);  
        //printf("%d ",k);
        i++;
        j=0;
        tmp=(char *) malloc(sizeof(char));
        while(str[i]!='/'){
            j++;
            tmp=realloc(tmp,(sizeof(char)*j));
            tmp[j-1]=str[i];
            i++;
        }
        d=atoi(tmp);
        //printf("%d ",d);
        i++;
        j=0;
        tmp=(char *) malloc(sizeof(char));
        while(str[i]!=' '){
            j++;
            tmp=realloc(tmp,(sizeof(char)*j));
            tmp[j-1]=str[i];
            i++;
        }
        strcpy(name,tmp);
        int s=strlen(name);
               
        //printf(" %s",name);
        i++;
        j=0;
        tmp=(char *) malloc(sizeof(char));
        while(str[i]!='\0'){
            if(str[i]==' '){
                values=realloc(values,(sizeof(int)*v+1));
                values[v]=atoi(tmp);
                j=0;
                v++;        }
            else{
                j++;
                tmp=realloc(tmp,(sizeof(char)*j));
                tmp[j-1]=str[i];
            }
            i++;
        }
        
        values=realloc(values,(sizeof(int)*v+1));
        values[v]=atoi(tmp);
        v++;
       // for(i=0;i<v;i++)
        //    printf(" %d",values[i]);
        //printf("\n");
        tag=insere(tag,k,d,name,values);

        free(tmp);
        free(name);
    }
    fclose(fptr);

    return tag;
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
        
        //printf("primer if\n");
        //print_figurinha(pai);
        
        //PARA NUEVO PAI
        if(pai->filho != NULL)
        {   
            //printf("primer if 1\n");
            aux = pai->filho;
            while (aux->prox_irmao != NULL ){
                aux = aux->prox_irmao;
            }
            aux->prox_irmao = temp->filho;
            // liberar temp
            free(temp);
        }else if (pai->filho == NULL){
            //printf("primer if 2\n");
            pai->filho = temp->filho;
            //print_figurinha(pai->filho);
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
        //printf("segundo if\n");
        //print_figurinha(pai);

            //PARA NUEVO PAI
        if(pai->filho != NULL){
            //printf("segundo if 1\n");
            aux = pai->filho;
            while (aux->prox_irmao != NULL ){
                aux = aux->prox_irmao;
            }
            aux->prox_irmao = temp->filho;//filho del q eliminare  sera nuevo hermano de los hijos de mi nuevo pai
            // liberar temp
            free(temp);

        }else if (pai->filho == NULL){
            //printf("segundo if 1\n");

            pai->filho = temp->filho;
            //print_figurinha(pai->filho);
            // liberar temp
            free(temp);
        }

    }
    return arv;
}

////////////////

//(e) destruir a árvore;

void destruir_ag(TAG * no)
{
    if(no != NULL){    
        destruir_ag(no->filho);
        destruir_ag(no->prox_irmao);
        
        print_figurinha(no);
        
        free(no);
    }    
    return;
}