#include <stdio.h>
#include <string.h>

int main()
{
   char string[101], *token;
   int i;
   
   printf("Digite a string CSV: ");
   scanf("%[^\n]", string);
   getchar();
   
   i = 1;
   token = strtok(string, ", ");
   do {
      printf("Elemento da coluna %d: %s\n", i, token);
      i++;
      token = strtok(NULL, ", ");
   } while (token);

   return (0);
}