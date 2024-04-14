/*------------------------------
    Biblioteca io.c  
 ------------------------------*/
/* Esta biblioteca é destinada a funções que realizam as operações
 * de cada opção do programa csvreader. */

/* Includes necessários */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

/* Defines necessários */
#define MAX_BUF 1024
#define _CRT_SECURE_NO_DEPRECATE

/* Tipos de dados */
typedef struct {
   char* valor;
   int vezes;
} dado;

/*------------------------------
      FUNÇÕES DE USO GERAL
 ------------------------------*/
int maior(int a, int b)
{
   if (a > b) return (a);
   return (b);
}

/* Verifica se o índice ind está no vetor de inteiro intVet de tamanho tam. 
 * Retorna 1 se está presente, ou 0 caso contrário. */
int indiceNoVet(int ind, int *intVet, int tam)
{
   if (intVet == NULL) return (0);
   if (ind < 0 || tam < 0) return (0);

   for (int i = 0; i < tam; i++)
      if (intVet[i] == ind) return (1);
   return (0);
}

/* Recebe um número inteiro.
 * Retorna um*/
int digitos(int n)
{
   int numDigitos = 0;

   do { n = n / 10; numDigitos++; } while (n != 0);
   return (numDigitos);
}

/* Recebe uma string e o tamanho da string.
 * Substitui todos os caracteres ',' e '\n' pelo caractere nulo '\0'. */
void substituiVirgulas(char* str)
{
   int tam;
   
   if (str == NULL) return;

   tam = strlen(str);
   for (int i = 0; i < tam; i++)
      if (str[i] == ',' || str[i] == '\n')
         str[i] = '\0';
}

/* Recebe uma linha de um arquivo .csv e o índice da coluna do arquivo. 
 * Retorna o endereço do primeiro caractere da string da coluna ou NULL. */
char* separaString(char* lin, int indCol)
{
   int indice = 0;
   char *inicio;
   
   if (lin == NULL) return NULL;

   inicio = lin;
   while (indice < indCol) {
      if (inicio[0] == '\0') indice++;
      inicio = &inicio[1];
   }
   if (inicio[0] == '\0') inicio = NULL;

   return (inicio);    
}

/* Recebe uma linha de um arquivo .csv.
 * Retorna o número de colunas da linha dada. */
int colunasNaLinha(char* linha)
{
   char* pnt;
   unsigned int numCols = 1;
   
   if (linha == NULL) return (0);

   pnt = linha;
   while (pnt[0] != '\0'){
      if (pnt[0] == ',') numCols++;
      pnt = &pnt[1];
   }

   return (numCols);
}

/* Procura uma variável em um arquivo .csv.
 * Retorna o índice no vetor, ou -1 se não achar. */
int achaVariavel(aqvCSV* csv, char* var)
{
   int i;
   
   if (csv == NULL || var == NULL) return (-1);

   for (i = 0; i < csv->numCols; i++)
      if (!strcmp(var, csv->arrVar[i].nome)) return (i);
   return (-1);
}

/*------------------------------
      FUNÇÕES DE VERIFICAÇÃO
 ------------------------------*/
/* Recebe um arquivo .csv.
 * Verifica se todas as linhas possuem o mesmo número de colunas.
 * Após, verifica se todos os dados da coluna são do mesmo tipo, coluna a coluna.
 * Retorna 0 caso haja um erro no arquivo, e 1 caso contrário. */
int verificaArquivo(aqvCSV* csv){
   int i, len, cols;
   char buffer[MAX_BUF], *substr;
   
   if (csv == NULL) return (0);

   /* Verificação do número de colunas */
   fgets(buffer, MAX_BUF, csv->aqv);
   substituiVirgulas(buffer);
   len = csv->numCols;

   while (!feof(csv->aqv)){
      fgets(buffer, MAX_BUF, csv->aqv);
      cols = colunasNaLinha(buffer);

      if (cols != len) return (0);
   }
   fseek(csv->aqv, 0, SEEK_SET);

   /* Verificação do tipo de variáveis */
   fgets(buffer, MAX_BUF, csv->aqv);
   while (!feof(csv->aqv)){
      fgets(buffer, MAX_BUF, csv->aqv);
      substituiVirgulas(buffer);
      for (i = 0; i < csv->numCols; i++){
         substr = separaString(buffer, i);
         if (substr != NULL){
            if (atof(substr) && csv->arrVar[i].tipo == STRING) return (0);
            if (!atof(substr) && csv->arrVar[i].tipo == NUMERIC) return (0);
         }
      }
   }
   fseek(csv->aqv, 0, SEEK_SET);

   return (1);
}

/*------------------------------
      ARQUIVO CSV
 ------------------------------*/
/* Recebe um arquivo .csv.
 * Retorna o número de colunas do documento, ou 0 caso haja erros. */
int contaColunas(FILE* aqv)
{
   int bufLen;
   int numCols = 1;
   char buffer[MAX_BUF], *str;

   if (aqv == NULL) return (0);

   str = fgets(buffer, MAX_BUF, aqv);
   if (str != buffer) return 0;

   bufLen = strlen(buffer);
   for (int i = 0; i < bufLen; i++)
      if (buffer[i] == ',') numCols++;
   
   fseek(aqv, 0, SEEK_SET);
   return (numCols);
}

/* Recebe um arquivo .csv.
 * Retorna o número de linhas contidas no arquivo. */
int contaLinhas(FILE* aqv)
{
   int numLin = 0;
   char c;
   
   if (aqv == NULL) return (0);

   c = fgetc(aqv);
   while (c != EOF){
      if (c == '\n') numLin++;
      c = fgetc(aqv);
   }

   fseek(aqv, 0, SEEK_SET);
   return (numLin);
}

/* Gera uma tabela com as linhas do arquivo.
 * Guarda o índice original da linha para impressão. */
linha* geraTabela(FILE* aqv)
{
   linha *tabela;
   int indice, numLins;
   char buffer[MAX_BUF];
   
   if (aqv == NULL) return NULL;

   numLins = contaLinhas(aqv);
   tabela = (linha*) malloc ((numLins) * sizeof(linha));
   if (tabela == NULL) return NULL;

   /* Copia as linhas do arquivo para a tabela
    * Além disso, copia o índice original de cada linha */
   indice = 0;
   while (indice < numLins){
      fgets(buffer, MAX_BUF, aqv);
      strcpy(tabela[indice].lin, buffer);
      tabela[indice].indOrig = indice;
      indice++;
   }
  
   fseek(aqv, 0, SEEK_SET);
   return (tabela);
}

/* Recebe um arquivo .csv.
 * Aloca um vetor de variáveis do arquivo e retorna o ponteiro para tal, ou NULL em uma falha. */
variavel* geraVariaveis(FILE* aqv)
{
   char buffer[MAX_BUF], *substr;
   variavel *arrVar;
   int i, numCols;

   if (aqv == NULL) return NULL;

   numCols = contaColunas(aqv);
   if (numCols == 0) return NULL;

   arrVar = (variavel*) malloc (numCols * sizeof(variavel));
   if (arrVar == NULL) return NULL;

   for (i = 0; i < numCols; i++) arrVar[i].tipo = STRING;

   /* Leitura do cabeçalho */
   fgets(buffer, MAX_BUF, aqv);
   substituiVirgulas(buffer);
   for (i = 0; i < numCols; i++){
      substr = separaString(buffer, i);
      strcpy(arrVar[i].nome, substr);
   }

   /* Leitura das linhas restantes 
    * Verifica se a variável é do tipo numérico, se for, atribui NUMERIC 
    * Além disso, calcula a quantidade de caracteres máximos para impressão */
   while (!feof(aqv)){
      fgets(buffer, MAX_BUF, aqv);
      substituiVirgulas(buffer);
      for (int j = 0; j < numCols; j++){
         substr = separaString(buffer, j);
         if (substr != NULL && atof(substr)) arrVar[j].tipo = NUMERIC;
      }
   }
   
   fseek(aqv, 0, SEEK_SET);
   return (arrVar);
}

void imprimeArquivo(aqvCSV* csv)
{
   int i, lin, col; 
   int contador, qntdLinhas, qntdColunas, indiceMaximo;
   int *tamMaximo, *guardaLinha;
   char buffer[MAX_BUF], *substr;

   if (csv == NULL) return;

   /* Conta a quantidade de linhas a serem impressas */
   qntdLinhas = 0;
   for (lin = 1; lin < csv->numLins; lin++)
      if (csv->printLins[lin] == 1) qntdLinhas++;
   
   qntdColunas = 0;
   for (col = 0; col < csv->numCols; col++)
      if (csv->printCols[col] == 1) qntdColunas++;

   if (qntdLinhas == 0 || qntdColunas == 0) return;  /* Não há linhas para imprimir */
   
   indiceMaximo = 0;
   for (lin = 1; lin < csv->numLins; lin++)
         if (csv->printLins[lin] == 1) indiceMaximo = lin;
   
   /* Calcula o Valor Máximo de Caracteres para Imprimir */
   tamMaximo = (int*) malloc (csv->numCols * sizeof(int));
   if (tamMaximo == NULL) return;

   strcpy(buffer, csv->tabela[0].lin);
   substituiVirgulas(buffer);
   for (col = 0; col < csv->numCols; col++){
      substr = separaString(buffer, col);
      if (substr != NULL) tamMaximo[col] = strlen(substr); /* Coloca o tamanho das variáveis no tamMaximo */
   }

   if (qntdLinhas <= 11){
      /* 1 - Calcula o tamanho de impressão dos valores */
      for (lin = 1; lin < csv->numLins; lin++){
         if (csv->printLins[lin] == 1){
            strcpy(buffer, csv->tabela[lin].lin);
            substituiVirgulas(buffer);
            for (col = 0; col < csv->numCols; col++){
               substr = separaString(buffer, col);
               if (substr == NULL && tamMaximo[col] < 3) tamMaximo[col] = 3;
               if (substr != NULL && strlen(substr) > tamMaximo[col]) tamMaximo[col] = strlen(substr);
            }
         }
      }

      /* 2 - Impressão do Cabeçalho do Arquivo */
      strcpy(buffer, csv->tabela[0].lin);
      substituiVirgulas(buffer);
      for (i = 0; i < digitos(indiceMaximo - 1) + 1; i++)
         printf(" "); 
      for (col = 0; col < csv->numCols; col++)
         if (csv->printCols[col] == 1){
            substituiVirgulas(buffer);
            substr = separaString(buffer, col);

            if (substr == NULL)
               printf("%*s ", tamMaximo[col], "NaN");
            else
               printf("%*s ", tamMaximo[col], substr);
         }
      printf("\n");

      /* 3 - Impressão do Conteúdo do Arquivo */
      for (lin = 1; lin < csv->numLins; lin++){
         if (csv->printLins[lin] == 1){
            printf("%-*d ", digitos(indiceMaximo - 1), lin - 1); /* Imprime o índice da linha */

            strcpy(buffer, csv->tabela[lin].lin);
            substituiVirgulas(buffer);
            
            for (col = 0; col < csv->numCols; col++){
               if (csv->printCols[col] == 1){
                  substr = separaString(buffer, col);
                  if (substr == NULL)
                     printf("%*s ", tamMaximo[col], "NaN");
                  else
                     printf("%*s ", tamMaximo[col], substr);
               }
            }
            printf("\n");
         }
      }
   }
   else {
      guardaLinha = (int*) malloc (10 * sizeof(int));
      if (guardaLinha == NULL) { free(tamMaximo); return; }

      contador = 0;
      /* 1 - Pega o índice das primeiras cinco linhas */
      for (lin = 1; lin < csv->numLins && contador < 5; lin++){
         if (csv->printLins[lin] == 1){
            guardaLinha[contador] = lin;
            contador++;
         }
      }
      /* 2 - Pega o índice das últimas cinco linhas */
      contador = 0;
      for (lin = csv->numLins - 1; lin > 0 && contador < 5; lin--){
         if (csv->printLins[lin] == 1){
            guardaLinha[9 - contador] = lin;
            contador++;
         }
      }
      
      /* 3 - Calcula o valor máximo de caracteres para as linhas */
      for (i = 0; i < 10; i++){
         lin = guardaLinha[i];
         strcpy(buffer, csv->tabela[lin].lin);
         substituiVirgulas(buffer);
         for (col = 0; col < csv->numCols; col++){
            substr = separaString(buffer, col);
            if (substr != NULL && strlen(substr) > tamMaximo[col])
               tamMaximo[col] = maior(strlen(substr), tamMaximo[col]);
            if (tamMaximo[col] < 3) tamMaximo[col] = 3;
         }    
      }

      indiceMaximo = guardaLinha[9];
     
      /* 4 - Impressão do Cabeçalho do Arquivo */
      strcpy(buffer, csv->tabela[0].lin);
      substituiVirgulas(buffer);
      for (i = 0; i < maior(digitos(indiceMaximo - 1), 3) + 1; i++)
         printf(" "); 

      for (col = 0; col < csv->numCols; col++)
         if (csv->printCols[col] == 1){
            substituiVirgulas(buffer);
            substr = separaString(buffer, col);

            if (substr == NULL)
               printf("%*s ", tamMaximo[col], "NaN");
            else
               printf("%*s ", tamMaximo[col], substr);
         }
      printf("\n");

      /* 5 - Imprime as cinco primeiras linhas */
      for (i = 0; i < 5; i++){
         lin = guardaLinha[i];
         printf("%-*d ", maior(digitos(indiceMaximo - 1), 3), lin - 1);
         strcpy(buffer, csv->tabela[lin].lin);
         substituiVirgulas(buffer);

         for (col = 0; col < csv->numCols; col++){
            if (csv->printCols[col] == 1){
               substr = separaString(buffer, col);
               if (substr == NULL)
                  printf("%*s ", tamMaximo[col], "NaN");
               else
                  printf("%*s ", tamMaximo[col], substr);
            }
         }
         printf("\n");
      }
      
      /* 6 - Imprime as reticências */
      printf("%-*s ", maior(digitos(indiceMaximo - 1), 3), "...");
      for (col = 0; col < csv->numCols; col++)
         if (csv->printCols[col] == 1)
            printf("%*s ", tamMaximo[col], "...");
      printf("\n");

      /* 7 - Imprime as últimas cinco linhas */
      for (i = 5; i < 10; i++){
         lin = guardaLinha[i];
         printf("%-*d ", maior(digitos(indiceMaximo - 1), 3), lin - 1);
         strcpy(buffer, csv->tabela[lin].lin);
         substituiVirgulas(buffer);

         for (col = 0; col < csv->numCols; col++){
            if (csv->printCols[col] == 1){
               substr = separaString(buffer, col);
               if (substr == NULL)
                  printf("%*s ", tamMaximo[col], "NaN");
               else
                  printf("%*s ", tamMaximo[col], substr);
            }
         }
         printf("\n");
      }
      free(guardaLinha);
   }
   
   printf("\n[%d rows x %d columns]\n", qntdLinhas, qntdColunas);
   free(tamMaximo);
}

aqvCSV* criaAqvCSV(FILE* aqv){
   aqvCSV* csv;
   
   if (aqv == NULL) return NULL;

   /* Alocação de memória */
   csv = (aqvCSV*) malloc (sizeof(aqvCSV));
   if(csv == NULL) return NULL;

   csv->aqv = aqv;

   csv->tabela = geraTabela(aqv);
   if (csv->tabela == NULL) { destroiAqvCSV(csv); return NULL; }

   csv->numCols = contaColunas(aqv);
   if (csv->numCols == 0) { destroiAqvCSV(csv); return NULL; }

   csv->numLins = contaLinhas(aqv);
   if (csv->numLins == 0) { destroiAqvCSV(csv); return NULL; }

   csv->printLins = (int*) malloc (csv->numLins * sizeof(int));
   if (csv->printLins == NULL) { destroiAqvCSV(csv); return NULL; }

   csv->printCols = (int*) malloc (csv->numCols * sizeof(int));
   if (csv->printCols == NULL) { destroiAqvCSV(csv); return NULL; }

   csv->arrVar = geraVariaveis(aqv);
   if (csv->arrVar == NULL){ destroiAqvCSV(csv); return NULL; }

   return (csv);
}

void destroiAqvCSV(aqvCSV* csv)
{
   if (csv == NULL) return;

   if (csv->printLins) free(csv->printLins);
   if (csv->printCols) free(csv->printCols);
   if (csv->tabela) free(csv->tabela);
   if (csv->arrVar) free(csv->arrVar);
   free(csv);
}

int gravaCSV(aqvCSV* csv, char* nomeAqv)
{
   FILE* aqvNovo;
   int lin, col;
   char buffer[MAX_BUF], *substr;

   if (csv == NULL) return (0);

   aqvNovo = fopen(nomeAqv, "w");
   if (aqvNovo == NULL) return (0);

   /* Escrita do cabeçalho */
   strcpy(buffer, csv->tabela[0].lin);
   substituiVirgulas(buffer);

   for (col = 0; col < csv->numCols; col++){
      substr = separaString(buffer, col);
      if (substr != NULL) fprintf(aqvNovo, "%s", substr);
      if (col != csv->numCols - 1) fprintf(aqvNovo, "%c", ',');
   }
   fprintf(aqvNovo, "%c", '\n');

   /* Escrita do conteúdo */
   for (lin = 1; lin < csv->numLins; lin++){
      if (csv->printLins[lin] == 1){
         strcpy(buffer, csv->tabela[lin].lin);
         substituiVirgulas(buffer);
         for (col = 0; col < csv->numCols; col++){
            if (csv->printCols[col] == 1){
               substr = separaString(buffer, col);
               if (substr != NULL) fprintf(aqvNovo, "%s", substr);
               if (col != csv->numCols - 1) fprintf(aqvNovo, "%c", ',');
            }
         }
         if (lin != csv->numLins - 1) fprintf(aqvNovo, "%c", '\n');
      }
   }

   fclose(aqvNovo);
   return (1);
}

/*------------------------------
      SUMÁRIO
 ------------------------------*/
int sumario(aqvCSV* csv)
{
   if (csv->arrVar == NULL) return (0);

   /* Descrição das variáveis */
   for (int i = 0; i < csv->numCols; i++){
      printf("%s ", csv->arrVar[i].nome);
      (csv->arrVar[i].tipo == NUMERIC) ? printf("[N]\n") : printf("[S]\n");
   }
   /* Número de variáveis encontradas */
   printf("\n%d variaveis encontradas\n", csv->numCols);

   return (1);
}

/*------------------------------
      MOSTRAR
 ------------------------------*/
int mostrar(aqvCSV* csv)
{
   int i;
   
   if (csv == NULL) return (0);
   
   /* Prepara os vetores de impressão para
    * a função mostrar() */
   for (i = 0; i < csv->numLins; i++)
      csv->printLins[i] = 1;
   for (i = 0; i < csv->numCols; i++)
      csv->printCols[i] = 1;

   imprimeArquivo(csv);

   return (1);
}

/*------------------------------
      FILTROS 
 ------------------------------*/
int compIgual(char* s1, char* s2)
{
   if (s1 == NULL && s2 == NULL) return (1);
   if (s1 == NULL && s2 != NULL) return (0);
   if (s1 != NULL && s2 == NULL) return (0); 
   if (strcmp(s1, s2) == 0) return (1);
   return (0);
}

int compMaior(char* s1, char* s2)
{
   if (s1 == NULL && s2 == NULL) return (0);
   if (s1 == NULL && s2 != NULL) return (0);
   if (s1 != NULL && s2 == NULL) return (1); 
   if (strcmp(s1, s2) > 0) return (1);
   return (0);
}

int compMaiorIgual(char* s1, char* s2)
{
   if (s1 == NULL && s2 == NULL) return (1);
   if (s1 == NULL && s2 != NULL) return (0);
   if (s1 != NULL && s2 == NULL) return (1); 
   if (strcmp(s1, s2) >= 0) return (1);
   return (0);
}

int compMenor(char* s1, char* s2)
{
   if (s1 == NULL && s2 == NULL) return (0);
   if (s1 == NULL && s2 != NULL) return (1);
   if (s1 != NULL && s2 == NULL) return (0); 
   if (strcmp(s1, s2) < 0) return (1);
   return (0);
}

int compMenorIgual(char* s1, char* s2)
{
   if (s1 == NULL && s2 == NULL) return (1);
   if (s1 == NULL && s2 != NULL) return (1);
   if (s1 != NULL && s2 == NULL) return (0); 
   if (strcmp(s1, s2) <= 0) return (1);
   return (0);
}

int compDiferente(char* s1, char* s2)
{
   if (s1 == NULL && s2 == NULL) return (0);
   if (s1 == NULL && s2 != NULL) return (1);
   if (s1 != NULL && s2 == NULL) return (1); 
   if (strcmp(s1, s2) != 0) return (1);
   return (0);
}

int filtros(aqvCSV* csv, int (*filt) (char*, char*), char* var, char* valor)
{
   int i, indCol;
   char buffer[MAX_BUF], *substr;
   
   if (csv == NULL || filt == NULL || var == NULL || valor == NULL) return (0);

   /* Acha o índice da variável */
   indCol = achaVariavel(csv, var);
   if (indCol == -1) return (0);
   
   /* Zera o vetor para uso */
   for (i = 1; i < csv->numLins; i++)
      csv->printLins[i] = 0;
   
   /* Função filtros imprime todas as colunas*/
   for (i = 0; i < csv->numCols; i++)
      csv->printCols[i] = 1;

   /* Passa pelo arquivo, verificando se é
    * ou não para imprimir a linha baseado no filtro. */
   csv->printLins[0] = 1;
   for (i = 1; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);
      
      substr = separaString(buffer, indCol);
      if (filt(substr, valor) == 1) csv->printLins[i] = 1;
   }
   
   /* Imprime a tabela conforme os vetores */
   imprimeArquivo(csv);
   
   return (1);
}

/*------------------------------
      DESCRIÇÃO DOS DADOS
 ------------------------------*/
int particionaString(char** v, int ini, int fim)
{
   char *x, *troca;
   int i = ini - 1;
   
   x = v[fim];
   for (int j = ini; j < fim; j++){
      if (strcmp(v[j], x) <= 0){
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

void ordenaString(char** v, int ini, int fim)
{
   int q;

   if (ini < fim){
      q = particionaString(v, ini, fim);
      ordenaString(v, ini, q - 1);
      ordenaString(v, q + 1, fim);
   }
}

void imprimeUnicos(char** valUnicos, int contadorUnicos)
{
   printf("[ ");
   for (int i = 0; i < contadorUnicos; i++){
      if (i != contadorUnicos - 1){
         if (atof(valUnicos[i])) printf("%s, ", valUnicos[i]);
         else printf("'%s', ", valUnicos[i]);
      }
      else{
         if (atof(valUnicos[i])) printf("%s ", valUnicos[i]);
         else printf("'%s' ", valUnicos[i]);
      }
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
   double variancia, desvioPadrao;

   char buffer[MAX_BUF], *substr;
   char **valores, **valUnicos;
  
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

   valores = (char**) malloc (contador * sizeof(char**));
   if (valores == NULL) return;

   /* Faz a leitura dos valores da variável em um
    * vetor de floats 'valores' */
   j = 0;
   for (i = 1; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL){
         valores[j] = strdup(substr);
         j++;
      }
   }

   /* Cálculo da Média */
   soma = 0;
   for (i = 0; i < contador; i++)
      soma += atof(valores[i]);
   media = soma / contador;
   
   /* Cálculo do Desvio */
   variancia = 0;
   for (i = 0; i < contador; i++)
      variancia += (atof(valores[i]) - media) * (atof(valores[i]) - media);
   desvioPadrao = sqrt(variancia / (contador - 1.0));
   
   /* Cálculo da Mediana */
   ordenaString(valores, 0, contador - 1);
   if (contador % 2 == 1) mediana = atof(valores[(contador - 1) / 2]);
   else mediana = (atof(valores[(contador / 2)]) + atof(valores[(contador / 2) - 1])) / 2;
   
   /* Cálculo do Valor Mínimo e Máximo */
   valMin = atof(valores[0]);
   valMax = atof(valores[contador - 1]);

   for (i = 0; i < contador; i++)
      free(valores[i]);
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

   ordenaString(valUnicos, 0, contadorUnicos - 1);

   qntAparece = (int*) malloc (contadorUnicos * sizeof(int));
   if (qntAparece == NULL) { free(valUnicos); return; }
   
   for (i = 0; i < contadorUnicos; i++)
      qntAparece[i] = 0;

   for (i = 0; i < csv->numLins; i++){
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
   imprimeUnicos(valUnicos, contadorUnicos);

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

   for (i = 0; i < csv->numLins; i++){
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
   imprimeUnicos(valUnicos, contadorUnicos);

   for (i = 0; i < contadorUnicos; i++)
      free(valUnicos[i]);
   free(valUnicos);
   free(qntAparece);
}

//------------------------------------
int descricaoDados(aqvCSV* csv, char* var)
{
   int indVar;
  
   if (csv == NULL) return (0);

   if (csv->numCols == 0 || csv->arrVar == NULL) return (0);
   indVar = achaVariavel(csv, var);
   if (indVar == -1) return (0);

   if (csv->arrVar[indVar].tipo == NUMERIC) descricaoNum(csv, indVar);
   else descricaoString(csv, indVar);

   return (1);
}
//------------------------------------

/*------------------------------
      ORDENAÇÃO 
 ------------------------------*/
int ordenaDados(aqvCSV* csv, char *var, int opt)
{
   printf("Linhas: %d", csv->numLins);
   printf("Colunas: %d", csv->numCols);
   printf("Variavel: %s\n", var);
   printf("Ordem: %d\n", opt);
    
   return (1);
}

/*------------------------------
      SELEÇÃO
 ------------------------------*/
int selecionaDados(aqvCSV* csv, char* var)
{
   int i, j, contaVar, achouVar;
   char buffer[MAX_BUF], *substr;
   
   if (csv == NULL || var == NULL) return (0);

   /* Prepara os vetores de impressão
    * para a função selecionaDados() */
   for (i = 0; i < csv->numLins; i++)
      csv->printLins[i] = 1;

   for (i = 0; i < csv->numCols; i++)
      csv->printCols[i] = 0;

   /* Contagem de variáveis em var 
    * Além disso, substitui espaços por nulo */
   for (i = 0; i < strlen(var); i++)
      printf("%c", var[i]);
   
   contaVar = 1;
   for (i = 0; i < strlen(var); i++)
      if (var[i] == ' ') contaVar++;

   printf("Qntd. variaveis: %d\n", contaVar);
   
   /* Verifica se todas as variáveis dadas estão no arquivo
    * Além disso, guarda os índices das colunas das variáveis
   achouVar = 0;
   for (i = 0; i < contaVar; i++){
      for (j = 0; var[j] != ' ' && j < strlen(var); j++)
         buffer[i] = var[i];
      buffer[i] = '\0';
      substr = separaString(var, i);
      for (j = 0; j < csv->numCols; i++){
         if (substr != NULL)
            if (strcmp(substr, csv->arrVar[j].nome) == 0){
               csv->printCols[j] = 1;
               achouVar++;
            }
      }
   }
   printf("\n");
   if (achouVar != contaVar) return (0);*/
   
   /* Imprime as informações */
   //imprimeArquivo(csv);

   return (1);
}

/*------------------------------
      DADOS FALTANTES
 ------------------------------*/
int dadosFaltantes(aqvCSV* csv, int escolha)
{
   int i, j;
   char buffer[MAX_BUF], *substr;
   
   if (csv == NULL) return (0);
    /* Prepara os vetores de impressão para 
    * imprimir as linhas nulas */
   csv->printLins[0] = 1;
   for (i = 1; i < csv->numLins; i++)
      csv->printLins[i] = 0;
   for (i = 0; i < csv->numCols; i++)
      csv->printCols[i] = 1;
   
   /* Marca as linhas nulas para impressão */
   for (i = 1; i < csv->numLins; i++){
      strcpy(buffer, csv->tabela[i].lin);
      substituiVirgulas(buffer);
      
      for (j = 0; j < csv->numCols; j++){
         substr = separaString(buffer, j);
         if (substr == NULL){
            csv->printLins[i] = 1;
            break;
         }
      }
   }
   if (escolha == 1){
      /* Imprime a tabela com linhas nulas */
      imprimeArquivo(csv);
   }
   else if (escolha == 2){
      return (1);
   }
   else if (escolha == 3){
      return (1);
   }
   else if (escolha == 4){
      return (1);
   }
   else return (0);

   return (1);
}

/*------------------------------
      SALVAR DADOS
 ------------------------------*/
int salvaDados(aqvCSV* csv, char* nomeAqv)
{
   FILE* aqvNovo;
   int i;

   if (csv == NULL || nomeAqv == NULL) return (0);

   aqvNovo = fopen(nomeAqv, "w");
   if (aqvNovo == NULL) return (0);
   
   for (i = 0; i < csv->numLins; i++)
      fprintf(aqvNovo, "%s", csv->tabela[i].lin);
   fclose(aqvNovo);

   fseek(csv->aqv, 0, SEEK_SET);
   return (1);
}

/*------------------------------
      TESTE DA BIBLIOTECA
 ------------------------------*/
/*
int main()
{
   FILE* arqT;
   int i;

   arqT = fopen("Teste3.csv", "r");
   aqvCSV* csv = criaAqvCSV(arqT);
   //Testa contaColunas()
   printf("Numero colunas = %d\n", csv->numCols);
   printf("Numero linhas = %d\n", csv->numLins);
   printf("\n");

   filtros(csv, compMenor, "Idade", "32.0");

   char* novoAqv = "aqvIncrivel.csv";
   gravaCSV(csv, novoAqv);

   return (0);
}*/
