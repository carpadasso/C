/*---------------------------------------------------------
PROGRAMA: cliente TAD ArvBin
-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

typedef int ItemArv;
#include "arvBin.h"

/* Recebe uma árvore binária como parâmetro.
   Retorna a soma dos valores das chaves da árvore. */
int somaChave(ArvBin tree)
{
   if (tree == NULL) return 0;
   return tree->item + somaChave(tree->esq) 
                     + somaChave(tree->dir);
}

/* Recebe uma árvore binária como parâmetro.
   Retorna o menor valor de uma chave da árvore, ou 
   zero se a árvore estiver vazia.*/
int valorMinimo(ArvBin tree)
{  
   int valEsq, valDir, valRaiz;

   if (tree == NULL) return 0;

   valEsq = valorMinimo(tree->esq);
   valDir = valorMinimo(tree->dir);
   valRaiz = tree->item;

   if (valEsq == 0 && valDir != 0)
      if (valDir < valRaiz) return valDir;
      else return valRaiz;
   
   if (valEsq != 0 && valDir == 0)
      if (valEsq < valRaiz) return valEsq;
      else return valRaiz;
   
   if (valEsq == 0 && valDir == 0)
      return valRaiz;
   
   if (valEsq < valDir) {
      if (valEsq < valRaiz) return valEsq;
      else return valRaiz;
   }
   else {
      if (valDir < valRaiz) return valDir;
      else return valRaiz;
   }
}

void percursoArv(ArvBin tree)
{
   if (tree == NULL) return;
   percursoArv(tree->esq);
   percursoArv(tree->dir);
   printf("%d ", tree->item); 
}

void paiMaior(ArvBin arv)
{
   int itemE, itemD;
   if (arv == NULL) return;

   if (arv->esq == NULL && arv->dir != NULL)
      arv->item = arv->dir->item;
   
   if (arv->esq != NULL && arv->dir == NULL)
      arv->item = arv->esq->item;
   
   if (arv->esq == NULL && arv->dir == NULL) return;

   itemE = arv->esq->item;
   itemD = arv->dir->item;
   if (itemE > itemD) arv->item = itemE;
   else arv->item = itemD;

   paiMaior(arv->esq);
   paiMaior(arv->dir);
}

void printnode(char c, int val, int h)
{
   int i;
   for (i = 0; i < h; i++) printf("\t");
   if (val != 0) printf("%d\n", val);
   else printf("%c\n", c);
}

void show(ArvBin arv, int h)
{
   if (arv == NULL) { printnode('*', 0, h); return; }
   show(arv->dir, h + 1);
   printnode('0', arv->item, h);
   show(arv->esq, h + 1);
}

/* ----------------------------------------------------- */
/* Programa: cria uma árvore binária e imprime a árvore
*/
int main(int argc, char *argv[]){
  ArvBin arv;
  ItemArv v;

  criaArv( &arv );
  printf("Insira uma sequencia de chaves. Finalize a sequencia com 0: \n");
  scanf( "%d", &v );
  while( v != 0 ){
    insereArvNivel( v, &arv );
    scanf( "%d", &v );
  }
  printf( "\nArvore\n" );
  escreveArv( arv );

  printf("%d\n", somaChave(arv));

  printf("%d\n", valorMinimo(arv));
  
  paiMaior(arv);
  show(arv, alturaArv(arv));

  /*----   chamada das funcoes da lista ----
  ... maiorNaRaiz(...)
  escreveArv( raiz );

  ... ordenaPeloMaior(...)
  escreveArv( raiz );

  ... dobraArvore(...)
  escreveArv( raiz );
  --------------------------------------------- */

  free( arv );
  return (0);
}
