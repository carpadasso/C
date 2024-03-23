#include <stdio.h>
#include <string.h>
#include "arfflib.h"

int abreArquivoARFF(FILE *file)
{
   file = fopen("arff.txt", "r");
   if (!file) return (0);
   return (1);
}

void leAtributo(FILE *file, char *line, atributo *attr)
{
   strtok(line, " ");
   attr->rotulo = strtok(NULL, " ");
   attr->tipo = strtok(NULL, " ");
   if (strcmp(attr->tipo, "numeric") && strcmp(attr->tipo, "string")) {
      strcpy(attr->categorias, attr->tipo);
      strcpy(attr->tipo, "categoric\n");
   }
   else 
      attr->categorias = NULL;
   
   /* Seção de impressão para debugging */
   printf("Rotulo: %s\n", attr->rotulo);
   printf("Tipo: %s", attr->tipo);
   if (attr->categorias)
      printf("Categorias: %s", attr->categorias);
}

atributo* percorreArquivoARFF(FILE *file)
{
   atributo *arrayAttr;
   char line[1025], *str;
   int i = 0, achouData = 0;

   while (!feof(file)) {
      fgets(line, 1025, file); /* Lê uma linha do arquivo */
      str = strtok(line, " ");
      if (!strcmp(str, "@attribute")) {
         leAtributo(file, line, &arrayAttr[i]);
         i++;
      }
      else if (!strcmp(str, "@data"))
         achouData = 1;
      else {
         printf("Erro na formatação do arquivo.\n");
         return NULL;
      }
   }
   
   if (!achouData)
      return NULL;

   return arrayAttr;
}

void imprimeAtributos(atributo* atributos)
{
   atributo atr;

   atr = atributos[0];
   if (atr.rotulo) printf("%s\n", atr.rotulo);
   if (atr.tipo) printf("%s\n", atr.tipo);
   if (atr.categorias) printf("%s\n", atr.categorias);
}

void fechaArquivoARFF(FILE *file)
{
   fclose(file);
}