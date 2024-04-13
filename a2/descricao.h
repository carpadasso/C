#ifndef __DESCRICAO__
#define __DESCRICAO__

#include "io.h"

int particionaFloat(float* v, int ini, int fim);
void ordenaFloat(float* v, int ini, int fim);
int particionaString(char** v, int ini, int fim);
void ordenaString(char** v, int ini, int fim);
void imprimeUnicosNum(char** valUnicos, int contaUnicos);
void descricaoNum(aqvCSV* csv, int indVar);
void descricaoString(aqvCSV* csv, int indVar);

#endif