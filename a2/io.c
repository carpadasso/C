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
   int bufLen;
   int numCols = 1;
   char buffer[MAX_BUF], *str;

   str = fgets(buffer, MAX_BUF, aqv);
   if (str != buffer) return 0;

   bufLen = strlen(buffer);
   for (int i = 0; i < bufLen; i++)
      if (buffer[i] == ',') numCols++;
   
   fseek(aqv, 0, SEEK_SET);
   return (numCols);
}

/* Recebe uma linha de um arquivo .csv.
 * Retorna o número de colunas da linha dada. */
int contaColunasLin(char* linha)
{
   char* pnt;
   unsigned int numCols = 1;
   
   pnt = linha;
   while (pnt[0] != '\0'){
      if (pnt[0] == ',') numCols++;
      pnt = &pnt[1];
   }

   return (numCols);
}

/* Recebe um arquivo .csv.
 * Retorna o número de linhas contidas no arquivo. */
unsigned int contaLinhas(FILE* aqv)
{
   unsigned int numLin = 0;
   char buffer[MAX_BUF];

   while (!feof(aqv)){ numLin++; fgets(buffer, MAX_BUF, aqv); }
   fseek(aqv, 0, SEEK_SET);
   return (numLin - 1);
}

/* Recebe um arquivo .csv.
 * Aloca um vetor de variáveis do arquivo e retorna o ponteiro para tal, ou NULL em uma falha. */
variavel* geraVariaveis(FILE* aqv)
{
   char buffer[MAX_BUF], *substr;
   variavel *arrVar;
   int i, numCols;

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
         if (substr != NULL && atof(substr)) arrVar[j].tipo = NUMERIC;
         if (substr != NULL && strlen(substr) > arrVar[j].maxLen)
            arrVar[j].maxLen = strlen(substr);
      }
   }
   
   fseek(aqv, 0, SEEK_SET);
   return (arrVar);
}

/* Recebe um vetor de variáveis de um arquivo .csv.
 * Libera a memória alocada do vetor. */
void destroiVariaveis(variavel* arrVar)
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
      FUNÇÕES DE VERIFICAÇÃO
 ------------------------------*/
/* Recebe um arquivo .csv.
 * Verifica se todas as linhas possuem o mesmo número de colunas.
 * Após, verifica se todos os dados da coluna são do mesmo tipo, coluna a coluna.
 * Retorna 0 caso haja um erro no arquivo, e 1 caso contrário. */
int verificaArquivo(FILE* aqv){
   int i, len, cols, numCols;
   char buffer[MAX_BUF], *substr;
   variavel* arrVar;
   
   numCols = contaColunas(aqv);

   /* Verificação do número de colunas */
   fgets(buffer, MAX_BUF, aqv);
   substituiVirgulas(buffer);
   len = contaColunas(aqv);

   while (!feof(aqv)){
      fgets(buffer, MAX_BUF, aqv);
      cols = contaColunasLin(buffer);

      if (cols != len) return (0);
   }
   fseek(aqv, 0, SEEK_SET);

   /* Verificação do tipo de variáveis */
   arrVar = geraVariaveis(aqv);
   fgets(buffer, MAX_BUF, aqv);
   while (!feof(aqv)){
      fgets(buffer, MAX_BUF, aqv);
      substituiVirgulas(buffer);
      for (i = 0; i < numCols; i++){
         substr = separaString(buffer, i);
         if (substr != NULL){
            if (atof(substr) && arrVar[i].tipo == STRING) return (0);
            if (!atof(substr) && arrVar[i].tipo == NUMERIC) return (0);
         }
      }
   }
   fseek(aqv, 0, SEEK_SET);

   destroiVariaveis(arrVar);

   return (1);
}

/*------------------------------
      SUMÁRIO
 ------------------------------*/
void sumario(FILE* aqv)
{
   int numCols;
   variavel* arrVar;

   arrVar = geraVariaveis(aqv);
   if (arrVar == NULL) return;
   numCols = contaColunas(aqv);
   if (numCols == 0) return;

   /* Descrição das variáveis */
   for (int i = 0; i < numCols; i++){
      printf("%s ", arrVar[i].nome);
      (arrVar[i].tipo == NUMERIC) ? printf("[N]\n") : printf("[S]\n");
   }
   /* Número de variáveis encontradas */
   printf("\n%d variaveis encontradas\n", numCols);

   destroiVariaveis(arrVar);  
}

/*------------------------------
      MOSTRAR
 ------------------------------*/
int digitos(unsigned int n)
{
   int numDigitos;

   numDigitos = 0;
   do {
      n = n / 10;
      numDigitos++;
   } while (n != 0);

   return (numDigitos);
}

void mostrar(FILE* aqv)
{
   int j, k, numCols, tamMaximo;
   unsigned int i, numLins;
   char buffer[MAX_BUF], *substr;
   variavel* arrVar;

   numCols = contaColunas(aqv);
   numLins = contaLinhas(aqv);
   arrVar = geraVariaveis(aqv);
   
   /* Impressão cabeçalho */
   fgets(buffer, MAX_BUF, aqv);
   substituiVirgulas(buffer);
   if (numLins > 11){
      tamMaximo = 3;
      if (digitos(numLins) > tamMaximo) tamMaximo = digitos(numLins);
      printf("%*s ", tamMaximo, " ");
   }
   else printf("  ");
   for (i = 0; i < numCols; i++)
      printf("%*s ", arrVar[i].maxLen, separaString(buffer, i));
   printf("\n");

   /* Impressão conteúdo */
   if (numLins <= 11) { 
      for (i = 0; i < numLins - 1; i++){
         printf("%d ", i);
         fgets(buffer, MAX_BUF, aqv);
         substituiVirgulas(buffer);
         for (j = 0; j < numCols; j++){
            substr = separaString(buffer, j);
            tamMaximo = arrVar[j].maxLen;
            (substr == NULL) ? printf("%*s ", tamMaximo, "NaN") : printf("%*s ", tamMaximo, substr);
         }
         printf("\n");
      }
   }
   else {
      /* Impressão das cinco primeira linhas */
      for (i = 0; i < 5; i++){
         tamMaximo = 3;                                                    /* A string "..." possui três dígitos */
         if (digitos(numLins) > tamMaximo) tamMaximo = digitos(numLins);   /* Se os números tem mais de 3 dígitos, imprime com o num. de dígitos */
         printf("%-*d ", tamMaximo, i);
         fgets(buffer, MAX_BUF, aqv);
         substituiVirgulas(buffer);
         for (j = 0; j < numCols; j++){
            substr = separaString(buffer, j);
            tamMaximo = 3;
            if(arrVar[j].maxLen > 3) tamMaximo = arrVar[j].maxLen;
            (substr == NULL) ? printf("%*s ", tamMaximo, "NaN") : printf("%*s ", tamMaximo, substr);
         }
         printf("\n");
      }
      /* Impressão da linha com "..." */
      tamMaximo = 3;                                                    /* A string "..." possui três dígitos */
      if (digitos(numLins) > tamMaximo) tamMaximo = digitos(numLins);   /* Se os números tem mais de 3 dígitos, imprime com o num. de dígitos */
      printf("%-*s ", tamMaximo, "...");
      for (j = 0; j < numCols; j++){
         tamMaximo = 3;
         if (arrVar[j].maxLen > 3) tamMaximo = arrVar[j].maxLen;
         printf("%*s ", tamMaximo, "...");
      }
      printf("\n");

      while (i < numLins - 6){ fgets(buffer, MAX_BUF, aqv); i++; }   /* Pula até a quinta última linha */

      /* Impressão das últimas cinco linhas */
      for (j = i; j < numLins - 1; j++){
         tamMaximo = 3;
         if (digitos(numLins) > tamMaximo) tamMaximo = digitos(numLins);
         printf("%-*d ", tamMaximo, j);
         fgets(buffer, MAX_BUF, aqv);
         substituiVirgulas(buffer);
         for (k = 0; k < numCols; k++){
            substr = separaString(buffer, k);
            tamMaximo = 3;
            if (arrVar[k].maxLen > 3) tamMaximo = arrVar[k].maxLen;
            (substr == NULL) ? printf("%*s ", tamMaximo, "NaN") : printf("%*s ", tamMaximo, substr);
         }
         printf("\n");
      }
   }

   printf("\n[%d rows x %d columns]\n", numLins - 1, numCols);

   destroiVariaveis(arrVar);
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
int descricaoDados(FILE* aqv, char* var)
{
   int indVar, numCols;
   variavel* arrVar;
   
   numCols = contaColunas(aqv);
   if (numCols == 0) return (0);
   arrVar = geraVariaveis(aqv);
   if (arrVar == NULL) return (0);

   indVar = procuraVariavel(var, arrVar, numCols);
   if (indVar == -1){ destroiVariaveis(arrVar); return 0; }

   if (arrVar[indVar].tipo == NUMERIC) descricaoNum();
   else descricaoString();

   destroiVariaveis(arrVar);
   fseek(aqv, 0, SEEK_SET);

   return (1);
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

   arqT = fopen("Teste1.csv", "r");
   //Testa contaColunas()
   int numCols = contaColunas(arqT);
   printf("Numero colunas = %d\n", numCols);
   int numLinhas = contaLinhas(arqT);
   printf("Numero linhas = %d\n", numLinhas);
   printf("\n");

   // Teste do vetor
   variavel* array;
   array = geraVariaveis(arqT);
   if (!array) printf("NULO!!");
   imprimeVariaveis(array, numCols);
   destroiVariaveis(array);

   return (0);
}*/
