/*------------------------------
    Biblioteca io.c  
 ------------------------------*/
/* Esta biblioteca é destinada a funções que realizam as operações
 * de cada opção do programa csvreader. */

/* Includes necessários */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

/* Defines necessários */
#define MAX_BUF 1024

/*------------------------------
      FUNÇÕES DE USO GERAL
 ------------------------------*/
/* Recebe uma string e o tamanho da string.
 * Substitui todos os caracteres ',' e '\n' pelo caractere nulo '\0'. */
void substituiVirgulas(char* str)
{
   int tam = strlen(str);

   for (int i = 0; i < tam; i++)
      if (str[i] == ',' || str[i] == '\n')
         str[i] = '\0';
}

/* Recebe uma linha de um arquivo .csv e o índice da coluna do arquivo. 
 * Retorna o endereço do primeiro caractere da string da coluna ou NULL. */
char* separaString(char* line, int indCol)
{
   int indice = 0;
   char *inicio;
   
   inicio = line;
   while (indice < indCol) {
      if (inicio[0] == '\0') indice++;
      inicio = &inicio[1];
   }
   if (inicio[0] == '\0') inicio = NULL;

   return (inicio);    
}

/* Recebe um documento .csv.
 * Retorna o número de colunas do documento, ou 0 caso haja erros. */
int contaColunas(FILE* aqv)
{
   int bufLen, numCols = 1;
   char buffer[MAX_BUF], *str;

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
   char buffer[MAX_BUF];

   while (!feof(aqv)){ fgets(buffer, MAX_BUF, aqv); numLin++; }
   fseek(aqv, 0, SEEK_SET);
   return (numLin);
}

/* Recebe um arquivo .csv.
 * Aloca um vetor de variáveis do arquivo e retorna o ponteiro para tal, ou NULL em uma falha. */
variavel* geraVetorVariaveis(FILE* aqv)
{
   char buffer[MAX_BUF], *substr;
   variavel *arrVar;
   int i, j, numCols;

   numCols = contaColunas(aqv);
   arrVar = (variavel*) malloc (numCols * sizeof(variavel));
   for (i = 0; i < numCols; i++) arrVar[i].tipo = STRING;

   /* Leitura do cabeçalho */
   fgets(buffer, MAX_BUF, aqv);
   substituiVirgulas(buffer);
   for (i = 0; i < numCols; i++){
      substr = separaString(buffer, i);
      strcpy(arrVar[i].nome, substr);
      arrVar[i].maxLen = strlen(substr);
   }
   /* Leitura das linhas restantes */
   while (!feof(aqv)){
      fgets(buffer, MAX_BUF, aqv);
      substituiVirgulas(buffer);
      for (int j = 0; j < numCols; j++){
         substr = separaString(buffer, j);
         if (atof(substr)) arrVar[j].tipo = NUMERIC;
         if (strlen(substr) > arrVar[j].maxLen)
            arrVar[j].maxLen = strlen(substr);
      }
   }
   
   fseek(aqv, 0, SEEK_SET);
   return (arrVar);
}

/* Recebe um vetor de variáveis de um arquivo .csv.
 * Libera a memória alocada do vetor. */
void destroiVetorVariaveis(variavel* arrVar)
{
   free(arrVar);
}

/* Recebe uma string com o nome de uma variável, um vetor de variáveis de um arquivo
 * .csv e o tamanho do vetor.
 * Retorna o índice correspondente à variável, ou -1 se não houver variável no vetor. */
int procuraVariavel(char* var, variavel* arrVar, int max)
{
   int i;
   
   for (i = 0; i < max && strcmp(arrVar[i].nome, var); i++) ;
   if (i == max) return (-1);
   else return (i);
}

/* Função para testes. 
 * Imprime um vetor de variáveis de um arquivo .csv. */
void imprimeVariaveis(variavel* arrVar, int max)
{
   for (int i = 0; i < max; i++){
      printf("Variavel %s de tipo ", arrVar[i].nome);
      (arrVar[i].tipo == NUMERIC) ? printf("[N]\n") : printf("[S]\n");
   }
}

/*------------------------------
      SUMÁRIO
 ------------------------------*/
void sumario(FILE* aqv)
{
   int numCols;
   variavel* arrVar;

   arrVar = geraVetorVariaveis(aqv);
   if (arrVar == NULL) return;
   numCols = contaColunas(aqv);
   if (numCols == 0) return;

   /* Descrição das variáveis */
   for (int i = 0; i < numCols; i++){
      printf("%s ", arrVar[i].nome);
      (arrVar[i].tipo == NUMERIC) ? printf("[N]\n") : printf("[S]\n");
   }
   /* Número de variáveis encontradas */
   printf("%d variaveis encontradas\n", numCols);

   destroiVetorVariaveis(arrVar);  
}

/*------------------------------
      MOSTRAR
 ------------------------------*/
void mostrar(FILE* aqv)
{
   int i, linha = 0, numCols = contaColunas(aqv);
   char buffer[MAX_BUF], *substr;
   variavel* arrVar;

   arrVar = geraVetorVariaveis(aqv);
   
   /* Impressão cabeçalho */
   fgets(buffer, MAX_BUF, aqv);
   substituiVirgulas(buffer);
   printf("  ");
   for (i = 0; i < numCols; i++)
      printf("%*s ", arrVar[i].maxLen, separaString(buffer, i));
   printf("\n");

   /* Impressão conteúdo */
   while (!feof(aqv)){
      printf("%d ", linha);
      fgets(buffer, MAX_BUF, aqv);
      substituiVirgulas(buffer);
      for (i = 0; i < numCols; i++){
         substr = separaString(buffer, i);
         (substr == NULL) ? printf("NaN ") : printf("%*s ", arrVar[i].maxLen, substr);
      }
      printf("\n");
      linha++;
   }
   printf("\n[%d rows x %d columns]\n", linha, numCols);
   printf("\n");

   destroiVetorVariaveis(arrVar);
   fseek(aqv, 0, SEEK_SET);
}

/*------------------------------
      FILTROS 
 ------------------------------*/
void filtros(FILE* aqv, void (*filt) (char*))
{
   char buffer[MAX_BUF];
   filt(buffer);
}

/*------------------------------
      DESCRIÇÃO DOS DADOS
 ------------------------------*/
void descricaoNum() {}
void descricaoString() {}
void descricaoDados(FILE* aqv)
{
   int indVar, numCols;
   char* var;
   variavel* arrVar;
   
   numCols = contaColunas(aqv);
   if (numCols == 0) return;
   arrVar = geraVetorVariaveis(aqv);
   if (arrVar == NULL) return;

   scanf("%s", var);
   indVar = procuraVariavel(var, arrVar, numCols);
   if (indVar == -1){ printf("Variavel nao encontrada.\n"); destroiVetorVariaveis(arrVar); return; }

   if (arrVar[indVar].tipo == NUMERIC) descricaoNum();
   else descricaoString();

   destroiVetorVariaveis(arrVar);
   fseek(aqv, 0, SEEK_SET);
}

/*------------------------------
      ORDENAÇÃO 
 ------------------------------*/
void ordenacaoAqv(FILE* aqv)
{
   
}

/*------------------------------
      SELEÇÃO
 ------------------------------*/
void selecaoAqv(FILE* aqv)
{
   
}

/*------------------------------
      DADOS FALTANTES
 ------------------------------*/
void dadosFaltantes()
{
   
}

/*------------------------------
      SALVAR DADOS
 ------------------------------*/
void salvaDados()
{
   
}

/*------------------------------
      TESTE DA BIBLIOTECA
 ------------------------------*/
/*
int main()
{
   FILE* arqT;

   arqT = fopen("aqvTeste.csv", "r");
   //Testa contaColunas()
   int numCols = contaColunas(arqT);
   printf("Numero colunas = %d\n", numCols);
   int numLinhas = contaLinhas(arqT);
   printf("Numero linhas = %d\n", numLinhas);
   printf("\n");

   // Teste do vetor
   variavel* array;
   array = geraVetorVariaveis(arqT);
   if (!array) printf("NULO!!");
   imprimeVariaveis(array, numCols);
   destroiVetorVariaveis(array);

   return (0);
}*/
