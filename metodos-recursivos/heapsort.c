#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
   Heap:
   - size: Tamanho do vetor
   - heap_size: Tamanho do heap atual
   - v: Vetor de inteiros
*/
struct heap_t {
   int size;
   int heap_size;
   int *v;
} ;

/*
   Aloca memória para um heap, com um vetor de
   tamanho max.
*/
struct heap_t* cria_heap (int max) {
   struct heap_t *heap;

   heap = malloc (sizeof (struct heap_t));
   if (!heap) return NULL;

   heap->size = max;
   heap->v = malloc (max * sizeof (int));
   return (heap);
}

/*
   Lê os dados fornecidos e armazena no vetor
   do heap.
*/
void le_heap (struct heap_t *H) {
   printf ("DADOS DO VETOR: ");
   for (int i = 0; i < H->size; i++)
      scanf ("%d", &H->v[i]);
}

/*
   Imprime os valores do vetor do heap.
*/
void imprime_heap (struct heap_t *H) {
   int i;

   printf ("[ ");
   for (i = 0; i < H->size; i++)
      printf ("%d ", H->v[i]);
   printf ("]\n");
}

/*
   Libera a memória alocada do heap e do vetor.
*/
void destroi_heap (struct heap_t *H) {
   free (H->v);
   H->v = NULL;
   free (H);
   H = NULL;
}

/*
   Calcula e devolve o nó pai de um nó i.
*/
int parent (int i) {
   return ((int) floor ((i - 1) / 2));
}

/*
   Calcula e devole o nó filho da esquerda do
   nó i.
*/
int left (int i) {
   return (2 * i + 1);
}

/*
   Calcula e devolve o nó filho da direita do
   nó i.
*/
int right (int i) {
   return (2 * i + 2);
}

/* 

   Pseudocódigo do livro Introduction to Algorithms,
   de Thomas Cormen:
   MAX-HEAPFY (A, i)
      l = LEFT (i)
      r = RIGHT (i)
      if l <= A.heap-size && A[l] > A[i]
         largest = l
      else largest = i
      if r <= A.heap-size && A[r] > A[largest]
         largest = r
      if largest != i
         exchange A[i] with A[largest]
          MAX-HEAPFY (A, largest)
*/
void max_heapfy (struct heap_t *H, int i) {
   int l, r, maior, aux;

   l = left (i);
   r = right (i);
   if (l < H->heap_size && H->v[l] > H->v[i])
      maior = l;
   else maior = i;
   if (r < H->heap_size && H->v[r] > H->v[maior])
      maior = r;
   if (maior != i) {
      aux = H->v[i];
      H->v[i] = H->v[maior];
      H->v[maior] = aux;
      max_heapfy (H, maior);
   }
}

/*
   Pseudocódigo do livro Introduction to Algorithms,
   de Thomas Cormen:
   BUILD-MAX-HEAP (A)
      A.heap-size = A.length
      for i = floor (A.length / 2) downto 1
         MAX-HEAPFY (A, i)
*/
void build_max_heap (struct heap_t *H) {
   int i;

   H->heap_size = H->size;
   for (i = (int) floor (H->size / 2); i >= 0; i--)
      max_heapfy (H, i);
}

/*
   Pseudocódigo do livro Introduction to Algorithms,
   de Thomas Cormen:
   HEAPSORT (A)
      for i = A.length downto 2
         exchange A[1] with A[i]
         A.heap-size = A.heapsize - 1
         MAX-HEAPFY (A, 1)
*/
void heapsort (struct heap_t *H) {
   int i, aux;

   build_max_heap (H);
   for (i = H->size - 1; i > 0; i--) {
      aux = H->v[0];
      H->v[0] = H->v[i];
      H->v[i] = aux;
      H->heap_size--;
      max_heapfy (H, 0);
   }
}

int main () {
   int N;
   struct heap_t *H;

   printf ("TAMANHO DO VETOR: ");
   scanf ("%d", &N);

   H = cria_heap (N);
   if (!H) return (1);
   le_heap (H);

   printf ("VETOR ANTES DE ORDENAR: \n");
   imprime_heap (H);

   heapsort (H);
   printf ("VETOR DEPOIS DE ORDENAR: \n");
   imprime_heap (H);

   return (0);
}