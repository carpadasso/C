#include <stdio.h>
#include "arfflib.h"

int main()
{
   FILE *file;
   atributo *atributos;

   file = fopen("arff.txt", "r");
   //-------------------------------------
   // Manipulação de Arquivo

   atributos = percorreArquivoARFF(file);
   
   //-------------------------------------
   fclose(file);

   return (0);
}