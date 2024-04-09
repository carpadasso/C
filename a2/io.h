#ifndef __IO__
#define __IO__

#define _CTR_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------
 *     Definição de tipos 
 ----------------------------*/
enum tipoVariavel { NUMERIC, STRING };

typedef struct {
   char* nome;
   enum tipoVariavel tipo;
} variavel;

/* ------------------------------
 *     Funções 
 ----------------------------*/
void sumario(FILE* aqv);

#endif  
