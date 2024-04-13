#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "descricao.h"
#include "io.h"

#define MAX_BUF 1024

int particionaFloat(float* v, int ini, int fim)
{
   float x = v[fim], troca;
   int i = ini - 1;

   for (int j = ini; j < fim; j++){
      if (v[j] <= x){
         i++;
         troca = v[i];
         v[i] = v[j];
         v[j] = troca;
      }
   }
   troca = v[i + 1];
   v[i + 1] = v[fim];
   v[fim] = troca;

   return (i + 1);
}

void ordenaFloat(float* v, int ini, int fim)
{
   int q;

   if (ini < fim){
      q = particionaFloat(v, ini, fim);
      ordenaFloat(v, ini, q - 1);
      ordenaFloat(v, q + 1, fim);
   }
}

int particionaString(char** v, int ini, int fim)
{
   char x[MAX_BUF], troca[MAX_BUF];
   int i = ini - 1;
   
   strcpy(x, v[fim]);
   for (int j = ini; j < fim; j++){
      if (strcmp(v[j], x) <= 0){
         i++;
         strcpy(troca, v[i]);
         strcpy(v[i], v[j]);
         strcpy(v[j], troca);
      }
   }
   strcpy(troca, v[i + 1]);
   strcpy(v[i + 1], v[fim]);
   strcpy(v[fim], troca);

   return (i + 1);
}

void ordenaString(char** v, int ini, int fim)
{
   int q;

   if (ini < fim){
      q = particionaString(v, ini, fim);
      ordenaString(v, ini, q - 1);
      ordenaString(v, q + 1, fim);
   }
}

void imprimeUnicosNum(char** valUnicos, int contaUnicos)
{
   printf("[ ");
   for (int i = 0; i < contaUnicos; i++)
      if (i != contaUnicos - 1)
         printf("%.1f, ", atof(valUnicos[i]));
      else
         printf("%.1f ", atof(valUnicos[i]));
   printf("]\n");
}

void imprimeUnicosStr(char** valUnicos, int contadorUnicos)
{
   printf("[ ");
   for (int i = 0; i < contadorUnicos; i++){
      if (i != contadorUnicos - 1)
         printf("'%s', ", valUnicos[i]);
      else
         printf("'%s' ", valUnicos[i]);
   }
   printf("]\n");

}

//---------------------------
//    DESCRIÇÃO NUMÉRICA
//---------------------------
void descricaoNum(aqvCSV* csv, int indVar)
{
   int i, j;
   int moda, maior, contador, contadorUnicos, *qntAparece;
   float soma, media, mediana, valMin, valMax;
   float *valores;
   double variancia, desvioPadrao;
   char buffer[MAX_BUF], *substr;
   char **valUnicos;
  
   /* Conta quantos valores não nulos a variável tem 
    * A variável 'contador' recebe o valor */
   contador = 0;
   for (i = 1; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL) contador++;
   }
   
   if (contador == 0) return; /* Não possui valores não nulos */

   valores = (float*) malloc (contador * sizeof(float));
   if (valores == NULL) return;

   /* Faz a leitura dos valores da variável em um
    * vetor de floats 'valores' */
   j = 0;
   for (i = 1; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL){
         valores[j] = atof(substr);
         j++;
      }
   }

   /* Cálculo da Média */
   soma = 0;
   for (i = 0; i < contador; i++)
      soma += valores[i];
   media = soma / contador;
   
   /* Cálculo do Desvio */
   variancia = 0;
   for (i = 0; i < contador; i++)
      variancia += (valores[i] - media) * (valores[i] - media);
   desvioPadrao = sqrt(variancia / (contador - 1.0));
   
   /* Cálculo da Mediana */
   ordenaFloat(valores, 0, contador - 1);
   if (contador % 2 == 1) mediana = valores[(contador - 1) / 2];
   else mediana = (valores[(contador / 2)] + valores[(contador / 2) - 1]) / 2;
   
   /* Cálculo do Valor Mínimo e Máximo */
   valMin = valores[0];
   valMax = valores[contador - 1];
   
   free(valores);

   /* Cálculo da Moda e dos Valores Únicos */
   valUnicos = (char**) malloc (contador * sizeof(char*));
   if (valUnicos == NULL) { return; }

   strcpy(buffer, csv->tabela[1].lin);
   substituiVirgulas(buffer);

   valUnicos[0] = strdup(separaString(buffer, indVar));
   contadorUnicos = 1;
   for (i = 2; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL){
         for (j = 0; j < contadorUnicos && strcmp(valUnicos[j], substr); j++) ;
         if (j == contadorUnicos){ 
            valUnicos[contadorUnicos] = strdup(substr);
            contadorUnicos++;
         }
      }
   }
   
   /* Calcula a Moda */
   ordenaString(valUnicos, 0, contadorUnicos - 1);

   qntAparece = (int*) malloc (contadorUnicos * sizeof(int));
   if (qntAparece == NULL) { free(valUnicos); return; }
   
   for (i = 0; i < contadorUnicos; i++)
      qntAparece[i] = 0;

   for (i = 0; i < contadorUnicos; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL)
         for (j = 0; j < contadorUnicos; j++)
            if (!strcmp(substr, valUnicos[j])) qntAparece[j]++;
   }
   
   maior = qntAparece[0];
   moda = 0;
   for (i = 1; i < contadorUnicos; i++)
      if (qntAparece[i] > maior){
         maior = qntAparece[i];
         moda = i;
      }

   printf("Contador: %d\n", contador);
   printf("Media: %.1f\n", media);
   printf("Desvio: %.1f\n", desvioPadrao);
   printf("Mediana: %.1f\n", mediana);
   printf("Moda: %.1f %d vezes\n", atof(valUnicos[moda]), qntAparece[moda]);
   printf("Min.: %.1f\n", valMin);
   printf("Max.: %.1f\n", valMax);
   printf("Valores unicos: ");
   imprimeUnicosNum(valUnicos, contadorUnicos);

   for (i = 0; i < contadorUnicos; i++)
      free(valUnicos[i]);
   free(valUnicos);
   free(qntAparece);
}

// ------------------------
//    DESCRIÇÃO STRING
// ------------------------
void descricaoString(aqvCSV* csv, int indVar)
{
   int i, j;
   int moda, maior, contador, contadorUnicos;
   int *qntAparece;
   char buffer[MAX_BUF], *substr;
   char **valUnicos;

   /* Conta quantos dados a variável possui
    * Armazena a quantidade na variável contador */
   contador = 0;
   for (i = 1; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL) contador++;
   }

   valUnicos = (char**) malloc (contador * sizeof(char*)); 
   if (valUnicos == NULL) return;

   strcpy(buffer, csv->tabela[1].lin);
   substituiVirgulas(buffer);

   valUnicos[0] = strdup(separaString(buffer, indVar));
   contadorUnicos = 1;
   for (i = 2; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL){
         for (j = 0; j < contadorUnicos && strcmp(valUnicos[j], substr); j++) ;
         if (j == contadorUnicos){ 
            valUnicos[contadorUnicos] = strdup(substr);
            contadorUnicos++;
         }
      }
   }

   /* Calcula a Moda */
   ordenaString(valUnicos, 0, contadorUnicos - 1);

   qntAparece = (int*) malloc (contadorUnicos * sizeof(int));
   if (qntAparece == NULL) { free(valUnicos); return; }
   
   for (i = 0; i < contadorUnicos; i++)
      qntAparece[i] = 0;

   for (i = 0; i < contadorUnicos; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL)
         for (j = 0; j < contadorUnicos; j++)
            if (!strcmp(substr, valUnicos[j])) qntAparece[j]++;
   }
   
   maior = qntAparece[0];
   moda = 0;
   for (i = 1; i < contadorUnicos; i++)
      if (qntAparece[i] > maior){
         maior = qntAparece[i];
         moda = i;
      }

   printf("Contador: %d\n", contador);
   printf("Moda: %s %d vezes\n", valUnicos[moda], qntAparece[moda]);
   printf("Valores unicos: ");
   imprimeUnicosStr(valUnicos, contadorUnicos);

   for (i = 0; i < contadorUnicos; i++)
      free(valUnicos[i]);
   free(valUnicos);
}