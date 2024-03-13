#include <stdio.h>

/*
   Troca os valores de a1 e a2 entre si.
*/
void troca (int *a1, int *a2) {
   int aux;

   aux = *a1;
   *a1 = *a2;
   *a2 = aux;
}

/*
   PARTITION (A, p, r)
      x = A[r]
      i = p - 1
      for j = p to r - 1
         if A[j] <= x
            i = i + 1
            exchange A[i] with A[j]
      exchange A[i + 1] with A[r]
      return i + 1
*/

int partition (int v[], int p, int r) {
   int x, i, j, aux;

   x = v[r];
   i = p - 1;
   for (j = p; j < r; j++)
      if (v[j] <= x) {
         i++;
         troca (&v[i], &v[j]);
      }
   troca (&v[i + 1], &v[r]);

   return (i + 1);
}

/*
   QUICKSORT (A, p , r)
      if p < r
         q = PARTITION (A, p , r)
         QUICKSORT (A, p , q - 1)
         QUICKSORT (A, q + 1, r)
*/

void quicksort (int v[], int p, int r) {
   int q;

   if (p < r) {
      q = partition (v, p, r);
      quicksort (v, p, q - 1);
      quicksort (v, q + 1, r);
   }
}

int main () {
   int v[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

   printf ("VETOR ANTES DE ORDENAR: [ ");
   for (int i = 0; i < 10; i++)
      printf ("%d ", v[i]);
   printf ("]\n");

   quicksort (v, 0, 9);
   printf ("VETOR DEPOIS DE ORDENAR: [ ");
   for (int i = 0; i < 10; i++)
      printf ("%d ", v[i]);
   printf ("]\n");

   return (0);
}