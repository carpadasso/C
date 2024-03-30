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

   if (arvVazia(tree)) return 0;

   valEsq = valorMinimo(tree->esq);
   valDir = valorMinimo(tree->dir);
   valRaiz = tree->item;

   if (valEsq < valDir) {
      if (valEsq < valRaiz) return valEsq;
      else return valRaiz;
   }
   else {
      if (valDir < valRaiz) return valDir;
      else return valRaiz;
   }
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

/*----   chamada das funcoes da lista ----
  ... paiMaior(...)
  escreveArv( arv );

  ... maiorNaRaiz(...)
  escreveArv( raiz );

  ... ordenaPeloMaior(...)
  escreveArv( raiz );

  ... dobraArvore(...)
  escreveArv( raiz );
  --------------------------------------------- */

  free( arv );
  return 0;
}