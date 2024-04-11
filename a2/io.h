#ifndef __IO__
#define __IO__

#define MAX_BUF 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------
 *     Definição de tipos 
 ----------------------------*/
enum tipoVariavel { NUMERIC, STRING };

typedef struct {
   char nome[MAX_BUF];
   enum tipoVariavel tipo;
   int maxLen;
} variavel;

/* ------------------------------
 *     Funções 
 ----------------------------*/
int verificaArquivo(FILE* aqv);
void sumario(FILE* aqv);
void mostrar(FILE* aqv);
#endif  
