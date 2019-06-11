#include "agenerico.h"

/***********ARBORE BB -> AVL********************************/

typedef struct no{
    int info;
    TINF * pointer_info;
    struct no*  esq;
    struct no*  dir;
    int alt;
}no;


void libera(no* t){
    if( t != NULL ){
        libera( t->esq );
        libera( t->dir );
        free( t );
    }
}


no* busca(int e, no* t ){
    if( t == NULL ) return NULL;
    if( e < t->info ) return busca( e, t->esq );
    if( e > t->info ) return busca( e, t->dir );
    return t;
}


static int calc_alt( no* n ){
    if( n == NULL ) return -1;
    return n->alt;
}


static int max( int l, int r){
    return l > r ? l: r;
}


static no* rot_dir( no* k2 ){
    no* k1 = NULL;
    k1 = k2->esq;
    k2->esq = k1->dir;
    k1->dir = k2;
    k2->alt = max( calc_alt( k2->esq ), calc_alt( k2->dir ) ) + 1;
    k1->alt = max( calc_alt( k1->esq ), k2->alt ) + 1;
    return k1; /* nova raiz */
}


static no* rot_esq( no* k1 ){
    no* k2;
    k2 = k1->dir;
    k1->dir = k2->esq;
    k2->esq = k1;
    k1->alt = max( calc_alt( k1->esq ), calc_alt( k1->dir ) ) + 1;
    k2->alt = max( calc_alt( k2->dir ), k1->alt ) + 1;
    return k2;  /* nova raiz */
}


static no* rot_esq_dir( no* k3 ){
    k3->esq = rot_esq( k3->esq );
    return rot_dir( k3 );
}


static no* rot_dir_esq( no* k1 ){
    k1->dir = rot_dir( k1->dir );
    return rot_esq( k1 );
}


no* insere_avl(int e, no* t ,TINF* t_inf){
    if( t == NULL ){
        t = (no*)malloc(sizeof(no));
        if( t == NULL ){
            fprintf (stderr, "Out of memory!!! (insere)\n");
            exit(1);
        }
        else{
            t->info = e;
            t->alt = 0;
            t->esq = t->dir = NULL;
            t->pointer_info=t_inf;
        }
    }
    else if( e < t->info ){
        t->esq = insere_avl( e, t->esq,t_inf );
        if( calc_alt( t->esq ) - calc_alt( t->dir ) == 2 )
            if( e < t->esq->info )
                t = rot_dir( t );
            else
                t = rot_esq_dir( t );
    }
    else if( e > t->info ){
        t->dir = insere_avl( e, t->dir,t_inf  );
        if( calc_alt( t->dir ) - calc_alt( t->esq ) == 2 )
            if( e > t->dir->info )
                t = rot_esq( t );
            else
                t = rot_dir_esq( t );
    }
    t->alt = max( calc_alt( t->esq ), calc_alt( t->dir ) ) + 1;
    return t;
}


int calc_alt_retira(no *T){
    int lh,rh;
    if(T==NULL) return(0);
    if(T->esq==NULL) lh=0;
    else lh=1+T->esq->alt;
    if(T->dir==NULL) rh=0;
    else rh=1+T->dir->alt;
    if(lh>rh) return(lh);
    return(rh);
}


int FB(no *T){
    int lh,rh;
    if(T==NULL) return(0);
    if(T->esq==NULL) lh=0;
    else lh=1+T->esq->alt;
    if(T->dir==NULL) rh=0;
    else rh=1+T->dir->alt;
    return(lh-rh);
}


void imprime_aux(no *a, int andar){
  if(a){
      int j;
      imprime_aux(a->esq,andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->info);
      imprime_aux(a->dir,andar+1);
  }
}


void imprime_avl(no *a){
  imprime_aux(a, 1);
}

void print_nodo(no * a, FILE * fptr)
{
  if(a==NULL) return;
  else
     fprintf(fptr, "\"%d\" [ label= \" <f0>esq | <f1> %d | <f2>dir \" , shape=Mrecord]",a->info, a->info );
  print_nodo(a->esq, fptr);
  if(a->esq != NULL)
    fprintf(fptr, "\"%d\":f0 -> \"%d\" ", a->info, a->esq->info);
  print_nodo(a->dir, fptr);
  if(a->dir != NULL)
    fprintf(fptr, "\"%d\":f2 -> \"%d\" ", a->info, a->dir->info);
}

void print_dot(no *a)
{
  //fname = "tree.dot";
  FILE * fptr;
  fptr = fopen("tree.dot","w");

  fprintf(fptr, "digraph G{ \n");

  fprintf(fptr, "\"%d\" [ label= \" <f0>esq | <f1> %d | <f2>dir \" , shape=Mrecord]",a->info, a->info );

  print_nodo(a, fptr);

  fprintf(fptr, " } \n");
  fclose(fptr);

  system("dot -Tpdf tree.dot -o tree.pdf");
  system("evince tree.pdf");
}


no * retira_abb(int x, no *T){       
    no *p;
    if(T==NULL)
        return NULL;
    else
        if(x > T->info){
            T->dir=retira_abb(x,T->dir);
            if(FB(T)==2)
                if(FB(T->esq)>=0)
                    T=rot_dir(T);
                else
                    T=rot_esq_dir(T);
        }
        else
            if(x<T->info){
                    T->esq=retira_abb(x,T->esq);
                    if(FB(T)==-2)//Rebalance during windup
                        if(FB(T->dir)<=0)
                            T=rot_esq(T);
                        else
                            T=rot_dir_esq(T);
            }
            else{
                //info to be deleted is found
                  if(T->esq != NULL){
                      p=T->esq;
                      while(p->dir != NULL) p=p->dir;
                      T->info=p->info;
                      T->esq=retira_abb(p->info, T->esq);
                      if(FB(T)== -2)//Rebalance during windup
                        if(FB(T->dir)<=0)
                            T=rot_esq(T);
                        else
                            T=rot_dir_esq(T);
                  }
                  else{
                      no *x = T;
                      T = T->dir;
                      free(x);
                      return(T);
                  }
            }
    T->alt=calc_alt_retira(T);
    return(T);
}


static int min( int l, int r){
    return l < r ? l: r;
}


/***********ARBORE B********************************/

const int t = 2;

typedef struct echave{
    int chave;
    TINF * info;
}TCHAVE;

typedef struct ArvB{
  //int nchaves, folha, *chave;
  int nchaves, folha;
  TCHAVE * chave;
  
  struct ArvB **filho;
}TAB;


TAB *Cria(int t){
  TAB* novo = (TAB*)malloc(sizeof(TAB));
  novo->nchaves = 0;
  novo->chave =(TCHAVE*)malloc(sizeof(TCHAVE)*((t*2)-1));
  novo->folha=1;
  novo->filho = (TAB**)malloc(sizeof(TAB*)*t*2);
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  for(i=0; i<(t*2)-1; i++) novo->chave[i].info = NULL;
  return novo;
}


TAB *Libera(TAB *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) Libera(a->filho[i]);
    }
    free(a->chave);
    free(a->filho);
    free(a);
    return NULL;
  }
}



void Imprime(TAB *a, int andar){
  if(a){
    int i,j;
    //printf("enter imprime \n");
    for(i=0; i<=a->nchaves-1; i++){
      Imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i].chave);
    }
    Imprime(a->filho[i],andar+1);
  }
}

/*
void print_nodo_B(TAB * a, FILE * fptr)
{
  if(a==NULL) return;
  else
     fprintf(fptr, "\"%d\" [ label= \" <f0>esq | <f1> %d | <f2>dir \" , shape=Mrecord]",a->info, a->info );
  print_nodo(a->esq, fptr);
  if(a->esq != NULL)
    fprintf(fptr, "\"%d\":f0 -> \"%d\" ", a->info, a->esq->info);
  print_nodo(a->dir, fptr);
  if(a->dir != NULL)
    fprintf(fptr, "\"%d\":f2 -> \"%d\" ", a->info, a->dir->info);
}
*/

void write_noB(TAB *a, FILE * fptr)
{
  int i;
  fprintf(fptr, "\"M%p\" [ label= \" <f%d> |" , a, 0);
   
  for ( i = 0; i < a->nchaves -1 ; i++)
  {

    fprintf(fptr, "<f%d> %d | <f%d>  | " , -(i+1), a->chave[i].chave , i+1);
  } 

  fprintf(fptr, "<f%d> %d | <f%d> \" , shape=Mrecord]",-(i+1) ,a->chave[i].chave, i+1);
}

void print_noB(TAB *a, FILE * fptr){
  if(a){
    int i,j;
    //printf("enter imprime \n");
    // escribire nó
    write_noB(a, fptr);
    
    for(i=0; i<=a->nchaves-1; i++){
      
      print_noB(a->filho[i], fptr);
      
      //for(j=0; j<=andar; j++) 
      //  printf("   ");
      //fprintf("%d\n", a->chave[i].chave);
      if(a->filho[i]!=NULL)
        fprintf(fptr ,"\"M%p\":f%d -> \"M%p\":f%d ",a , i, a->filho[i], 0);

    }
    print_noB(a->filho[i], fptr);
    if(a->filho[i]!=NULL)
      fprintf(fptr ,"\"M%p\":f%d -> \"M%p\":f%d ",a , i, a->filho[i], 0);
  }
}

void print_dot_B(TAB *a)
{
  //fname = "tree.dot";
  FILE * fptr;
  fptr = fopen("treeB.dot","w");

  fprintf(fptr, "digraph G{ \n");

  print_noB(a, fptr);
  
  //print_no_B();
  fprintf(fptr, " } \n");
  fclose(fptr);

  system("dot -Tpdf treeB.dot -o treeB.pdf");
  system("evince treeB.pdf");
}



TAB *Busca(TAB* x, int ch){
  TAB *resp = NULL;
  if(!x) return resp;
  int i = 0;
  while(i < x->nchaves && ch > x->chave[i].chave) i++;
  if(i < x->nchaves && ch == x->chave[i].chave) return x;
  if(x->folha) return resp;
  return Busca(x->filho[i], ch);
}


TAB *Inicializa(){
  return NULL;
}


TAB *Divisao(TAB *x, int i, TAB* y, int t){
  TAB *z=Cria(t);
  z->nchaves= t - 1;
  z->folha = y->folha;
  int j;
  for(j=0;j<t-1;j++){
      z->chave[j].chave = y->chave[j+t].chave;
      z->chave[j].info = y->chave[j+t].info;
  }
  if(!y->folha){
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--){
      x->chave[j].chave = x->chave[j-1].chave;
      x->chave[j].info = x->chave[j-1].info;
  }
  x->chave[i-1].chave= y->chave[t-1].chave;
  x->chave[i-1].info= y->chave[t-1].info;
  x->nchaves++;
  return x;
}


TAB *Insere_Nao_Completo(TAB *x, int k, int t,TINF *info){
  
  
  int i = x->nchaves-1;
  //printf("i %d\n",i);
  if(x->folha){
    while((i>=0) && (k<x->chave[i].chave)){
      x->chave[i+1].chave = x->chave[i].chave;
      x->chave[i+1].info = x->chave[i].info;
      //printf("Folha Case menor x[%d] %d  %p\n",i+1,x->chave[i+1].chave,x->chave[i+1].info);
      i--;
    }

    // printf("Folha Case!!!\n");
    
    x->chave[i+1].chave = k;
    x->chave[i+1].info = info;
    x->nchaves++;
    //printf("Folha Case mayor x[%d] %d  %p\n",i+1,x->chave[i+1].chave,x->chave[i+1].info);
    return x;
  }
  while((i>=0) && (k<x->chave[i].chave)) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = Divisao(x, (i+1), x->filho[i], t);
    if(k>x->chave[i].chave) i++;
    
  }
  x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t,info);
    
  return x;
}


TAB *Insere(TAB *T, int k, int t,TINF * info){
  
  if(Busca(T,k)) return T;
  
  if(!T){
    T=Cria(t);
    T->chave[0].chave = k;
    T->chave[0].info=info;
    T->nchaves=1;
    //printf("Base Case!!!\n");
    //printf("chave[0].chave %d chave[0].info %p\n",T->chave[0].chave,T->chave[0].info);
    return T;
  }
  if(T->nchaves == (2*t)-1){
    //printf("divisao---chaves=3-------\n");
    TAB *S = Cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;

    S = Divisao(S,1,T,t);
    S = Insere_Nao_Completo(S,k,t,info);
    return S;
  }
  //printf("Insere_Nao_Completo-------\n");
  T = Insere_Nao_Completo(T,k,t,info);
  return T;
}


TAB* remover(TAB* arv, int ch, int t){
  if(!arv) return arv;
  int i;
  printf("Removendo %d...\n", ch);
  for(i = 0; i<arv->nchaves && arv->chave[i].chave < ch; i++);
  if(i < arv->nchaves && ch == arv->chave[i].chave){ //CASOS 1, 2A, 2B e 2C
    if(arv->folha){ //CASO 1
      printf("\nCASO 1\n");
      int j;
      for(j=i; j<arv->nchaves-1;j++) arv->chave[j].chave = arv->chave[j+1].chave;
      arv->nchaves--;
      return arv;      
    }
    if(!arv->folha && arv->filho[i]->nchaves >= t){ //CASO 2A
      printf("\nCASO 2A\n");
      TAB *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[y->nchaves];
      int temp = y->chave[y->nchaves-1].chave;
      arv->filho[i] = remover(arv->filho[i], temp, t); 
      //Eliminar recursivamente K e substitua K por K' em x
      arv->chave[i].chave = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves >= t){ //CASO 2B
      printf("\nCASO 2B\n");
      TAB *y = arv->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[0];
      int temp = y->chave[0].chave;
      y = remover(arv->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
      arv->chave[i].chave = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){ //CASO 2C
      printf("\nCASO 2C\n");
      TAB *y = arv->filho[i];
      TAB *z = arv->filho[i+1];
      y->chave[y->nchaves].chave = ch;          //colocar ch ao final de filho[i]
      int j;
      for(j=0; j<t-1; j++)                //juntar chave[i+1] com chave[i]
        y->chave[t+j] = z->chave[j];
      for(j=0; j<=t; j++)                 //juntar filho[i+1] com filho[i]
        y->filho[t+j] = z->filho[j];
      y->nchaves = 2*t-1;
      for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
        arv->chave[j] = arv->chave[j+1];
      for(j=i+1; j <= arv->nchaves; j++)  //remover ponteiro para filho[i+1]
        arv->filho[j] = arv->filho[j+1];
      arv->filho[j] = NULL; //Campello
      arv->nchaves--;
      arv->filho[i] = remover(arv->filho[i], ch, t);
      return arv;   
    }   
  }

  TAB *y = arv->filho[i], *z = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i menor que nchaves\n");
      z = arv->filho[i+1];
      y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
      y->nchaves++;
      arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
      int j;
      for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
        z->chave[j] = z->chave[j+1];
      //z->chave[j] = 0; Rosseti
      y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
        z->filho[j] = z->filho[j+1];
      z->nchaves--;
      arv->filho[i] = remover(arv->filho[i], ch, t);
      return arv;
    }
    if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i igual a nchaves\n");
      z = arv->filho[i-1];
      int j;
      for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
        y->chave[j] = y->chave[j-1];
      for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
        y->filho[j] = y->filho[j-1];
      y->chave[0] = arv->chave[i-1];              //dar a y a chave i da arv
      y->nchaves++;
      arv->chave[i-1] = z->chave[z->nchaves-1];   //dar a arv uma chave de z
      y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
      z->nchaves--;
      arv->filho[i] = remover(y, ch, t);
      return arv;
    }
    if(!z){ //CASO 3B
      if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
        printf("\nCASO 3B: i menor que nchaves\n");
        z = arv->filho[i+1];
        y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
        y->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          y->chave[t+j] = z->chave[j];     //passar filho[i+1] para filho[i]
          y->nchaves++;
        }
        if(!y->folha){
          for(j=0; j<t; j++){
            y->filho[t+j] = z->filho[j];
          }
        }
        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chave[j] = arv->chave[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        arv->nchaves--;
        arv = remover(arv, ch, t);
        return arv;
      }
      if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){ 
        printf("\nCASO 3B: i igual a nchaves\n");
        z = arv->filho[i-1];
        if(i == arv->nchaves)
          z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
        else
          z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
        z->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          z->chave[t+j] = y->chave[j];     //passar filho[i+1] para filho[i]
          z->nchaves++;
        }
        if(!z->folha){
          for(j=0; j<t; j++){
            z->filho[t+j] = y->filho[j];
          }
        }
        arv->nchaves--;
        arv->filho[i-1] = z;
        arv = remover(arv, ch, t);
        return arv;
      }
    }
  }  
  arv->filho[i] = remover(arv->filho[i], ch, t);
  return arv;
}


TAB* retira_b(TAB* arv, int k, int t){
  if(!arv || !Busca(arv, k)) return arv;
  return remover(arv, k, t);
}

/***********PILHA********************************/

typedef struct nodo{
    TAG* info;
    struct nodo * prox;
}TNO;

typedef struct pilha{
    TNO * prim;
}TP;

TP* cria(void){
    TP* p=(TP*) malloc(sizeof(TP));
    p->prim=NULL;
    return p;
}

int vazia(TP* p){
    //printf("p->prim %p\n",p->prim);
    return p->prim == NULL ? 1 : 0;
}

void push(TP* p, TAG * x){
    //printf("push %d \n",x->cod);
    TNO* novo=(TNO *) malloc(sizeof(TNO));
    novo->info=x;
    novo->prox=p->prim;
    p->prim=novo;
}

TAG * pop(TP* p){
    
    if(vazia(p)) exit(1);
    TAG * resp=p->prim->info;
    //printf("pop %d \n",resp->cod);
    TNO * q =p->prim;
    p->prim=q->prox;
    free(q);
    return resp;
}

void libera_pil(TP* p){
    TNO *q = p->prim, *t;
    while(q){
        t=q;
        q=q->prox;
        free(t);
    }
    free(q);
}

/***********FUNCAO 7********************************/

struct no * create_avl(TAG * ag){
    struct no * arvore=NULL;
    TP * my_pilha=cria();
    //printf("pilha_vazia, %d \n",vazia(my_pilha));
    push(my_pilha,ag);
    
    while(!vazia(my_pilha)){
        //printf("pilha_vazia, %d \n",!vazia(my_pilha));
        TAG *t=pop(my_pilha);  
        arvore=insere_avl(t->cod,arvore,t->info);
        if(t->filho){
            t=t->filho;
            while(t){
                push(my_pilha,t);
                t=t->prox_irmao;
            }
        }
    }
   
    return arvore;
    

      
}
void create_and_print_avl(TAG * ag){
    //struct no * arvore = NULL;
    if(ag == NULL)
    {
        printf("Arvore Generico nao tem dados!!!\n");
        return;
    }
    else
    {
        //struct no * arvore = NULL;
        struct no * arvore=create_avl(ag);
        printf("Arvore de Busqueda Balanceado Criado!!!\n");
        imprime_aux(arvore,1);
        
        print_dot(arvore);

        libera(arvore);
        printf("Arvore de Busqueda Balanceado Liberado!!!\n");
    }
    

}


/***********FUNCAO 8********************************/

TAB* create_b(TAG * ag){
    
    TAB * arvore = Inicializa();
    //int t=2;
    TP * my_pilha=cria();
    //printf("arvore and pila create\n");
    
    push(my_pilha,ag);
    //printf("pilha_vazia, %d \n",vazia(my_pilha));

    while(!vazia(my_pilha)){
        //printf("pilha_vazia, %d \n",!vazia(my_pilha));
        
        TAG *temp=pop(my_pilha);  
        
        arvore=Insere(arvore,temp->cod,t,temp->info);
        
        if(temp->filho){
            temp=temp->filho;
            while(temp){
                push(my_pilha,temp);
                temp=temp->prox_irmao;
            }
        }
    }
    //printf("end stack \n");
    //Imprime(arvore,0);
    return arvore;
    //TAB * arvore = Inicializa();
    //arvore = Insere(arvore, num, t);
}

void create_and_print_b(TAG * ag){
    
    if(ag == NULL)
    {
        printf("Arvore Generico nao tem dados!!!\n");
        return;
    }
    else
    {
        //printf("enter else\n");
        //struct no * arvore = NULL;
        TAB * arvore=create_b(ag);
        printf("Arvore B Criado!!!\n");
        Imprime(arvore,0);
        
        print_dot_B(arvore);
        //libera(arvore);
        printf("Arvore B Liberado!!!\n");


    }
}
