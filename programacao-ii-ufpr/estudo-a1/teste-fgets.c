#include <stdio.h>
#include <string.h>

int main()
{
   FILE *file;
   char str[50], *token;

   file = fopen("teste.txt", "r");

   fgets(str, 51, file);
   token = strtok(str, " ");
   printf("O valor de token é: %s\n", token);

   fclose(file);

   return (0);
}