#ifndef __IO__
#define __IO__

#define _CTR_SECURE_NO_WARNINGS
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
} variavel;

/* ------------------------------
 *     Funções 
 ----------------------------*/
void sumario(FILE* aqv);

#endif  
