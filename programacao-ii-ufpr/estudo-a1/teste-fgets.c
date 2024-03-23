#include <stdio.h>

int main()
{
   FILE *file;
   char str[50], *buff;

   file = fopen("arff.txt", "r");

   buff = fgets(str, 51, file);

   printf("String str: %s\n", str);
   printf("String buff: %s\n", buff);

   fclose(file);

   return (0);
}