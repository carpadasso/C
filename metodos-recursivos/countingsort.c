#include <stdio.h>
#include <stdlib.h>

#define MAX 10

/*
   COUNTING-SORT (A, B, k)
      let C[0..k] be a new array
      for i = 0 to k
         C[i] = 0
      for j = 1 to A.length
         C[A[j]] = C[A[j]] + 1
      // C[i] now contais the number of elements equal to i.
      for i = 1 to k
         C[i] = C[i] + C[i - 1]
      // C[i] now contains the number of elements less than or equal to i.
      for j = A.length downto 1
         B[C[A[j]]] = A[j]
         C[A[j]] = C[A[j]] - 1
*/

void counting_sort (int *A, int *B, int k) {
   int i, j;
   int *C;

   C = malloc ((k + 1) * sizeof (int));
   if (!C) return;

   /*
      A[i] -> i = [0, 9]
      B[i] -> i = [0, 9]
      C[i] -> i = [0, k]
   */
   for (i = 0; i < k + 1; i++)
      C[i] = 0;
   for (j = 0; j < MAX; j++)
      C[A[j]]++;
   for (i = 1; i < k + 1; i++)
      C[i] = C[i] + C[i - 1];
   for (j = MAX - 1; j >= 0; j--) {
      B[C[A[j]] - 1] = A[j];
      C[A[j]]--;
   }
   free (C);
   C = NULL;
}

int main () {
   int A[MAX] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
   int B[MAX];

   printf ("VETOR ANTES DE ORDENAR: [ ");
   for (int i = 0; i < MAX; i++)
      printf ("%d ", A[i]);
   printf ("]\n");

   counting_sort (A, B, 10);

   printf ("VETOR DEPOIS DE ORDENAR: [ ");
   for (int i = 0; i < MAX; i++)
      printf ("%d ", B[i]);
   printf ("]\n");

   return (0);
}