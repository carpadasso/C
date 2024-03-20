#include <stdio.h>

#define MAX_LEN 100
int main()
{
   FILE* f;
   char c;

   f = fopen("texto.txt", "r+");
   if (!f){
      printf("Erro ao abrir o arquivo.\n");
      return (1);
   }

   c = fscanf(f, "%c", &c);
   printf("%c", c);
   
   fclose(f);

   return (0);
}