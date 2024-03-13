#include <stdio.h>

/*
   Intercala: Faz a intercalação de dois vetores
   L[ini..meio] e R[meio + 1..fim]. 
   Percorre os vetores e verifica a cada iteração qual
   dos dois elementos, um de cada vetor, é o menor, e
   então insere na posição adequada em um vetor resultante.
*/
void intercala (int v[], int ini, int meio, int fim) {
   int i, j, k;
   int l[meio - ini + 1], r[fim - meio];

   for (i = 0; i < meio - ini + 2; i++)
      l[i] = v[ini + i - 1];
   for (j = 0; j < fim - meio + 1; j++)
      r[j] = v[meio + j];

   i = 1;
   j = 1;
   for (k = ini; k < fim + 1; k++) {
      if (l[i] <= r[j]) {
         v[k] = l[i];
         i++;
      }
      else {
         v[k] = r[j];
         j++;
      }
   }
}

/*
   Mergesort: ordena um vetor o dividindo recursivamente
   em duas partes V1[ini..meio] e V2[meio + 1..fim] e depois
   realiza a intercalação dos dois subvetores.
*/
void mergesort (int v[], int ini, int fim) {
   int meio;

   if (ini < fim) {
      meio = (ini + fim) / 2;
      mergesort (v, ini, meio);
      mergesort (v, meio + 1, fim);
      intercala (v, ini, meio, fim);
   }
}

int main () {
   int v[10] = {10, 8, 7, 7, 3, -5, 4, 5, 2, 1};

   printf ("VETOR ANTES DE ORDENAR: [ ");
   for (int i = 0; i < 10; i++)
      printf ("%d ", v[i]);
   printf ("]\n");

   mergesort (v, 0, 9);

   printf ("VETOR DEPOIS DE ORDENAR: [ ");
   for (int i = 0; i < 10; i++)
      printf ("%d ", v[i]);
   printf ("]\n");

   return (0);
}