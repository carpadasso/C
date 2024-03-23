#ifndef __ARFFLIB__
#define __ARFFLIB__

#include <stdio.h>

typedef struct {
   char *rotulo;
   char *tipo;
   char *categorias;
} atributo;

int abreArquivoARFF(FILE *file);

void fechaArquivoARFF(FILE *file);

void leAtributo(FILE *file, char *line, atributo *attr);

atributo* percorreArquivoARFF(FILE *file);

void imprimeAtributos(atributo *atributos);

#endif