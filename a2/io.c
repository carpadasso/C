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
#define SUM 0  /* Sumário */
#define MST 1  /* Mostrar */
#define FLT 2  /* Filtros */
#define DSC 3  /* Descrição */
#define ORD 4  /* Ordenação */
#define SEL 5  /* Seleção */
#define FAL 6  /* Dados Faltantes */
#define SAL 7  /* Salva Dados */

/* Tipos de dados */
typedef struct {
   char* valor;
   int vezes;
} dado;

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

/* Recebe uma linha de um arquivo .csv.
 * Retorna o número de colunas da linha dada. */
int colunasNaLinha(char* linha)
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

/* Recebe o índice original de uma linha. 
 * Imprime a linha conforme a formatação correta. */
int imprimeLinha(aqvCSV* csv, int ind, int tamMax)
{
   int i, j;
   char buffer[MAX_BUF], *substr, *printStr;
   
   strcpy(buffer, csv->tabela[ind].lin); /* Linha da tabela -> Buffer */
   substituiVirgulas(buffer);

   for (i = 0; i < csv->numCols; i++){
      substr = separaString(buffer, i);
      if (substr == NULL) printf("%*s ", tamMax, "NaN");
      else printf("%*s ", tamMax, substr);
   }

   return (1);
}

/*------------------------------
      FUNÇÕES DE VERIFICAÇÃO
 ------------------------------*/
/* Recebe um arquivo .csv.
 * Verifica se todas as linhas possuem o mesmo número de colunas.
 * Após, verifica se todos os dados da coluna são do mesmo tipo, coluna a coluna.
 * Retorna 0 caso haja um erro no arquivo, e 1 caso contrário. */
int verificaArquivo(aqvCSV* csv){
   int i, len, cols, numCols;
   char buffer[MAX_BUF], *substr;
   variavel* arrVar;
   
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
   
   c = fgetc(aqv);
   while (c != EOF){
      if (c == '\n') numLin++;
      c = fgetc(aqv);
   }
   numLin++;

   fseek(aqv, 0, SEEK_SET);
   return (numLin - 1);
}

linha* geraTabela(FILE* aqv)
{
   linha *tabela;
   int indice, numLins;
   char buffer[MAX_BUF], *substr;

   numLins = contaLinhas(aqv);
   tabela = (linha*) malloc ((numLins + 1) * sizeof(linha));
   if (tabela == NULL) return NULL;

   indice = 0;
   while (!feof(aqv)){
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

aqvCSV* criaAqvCSV(FILE* aqv){
   aqvCSV* csv;
   
   if (aqv == NULL) return NULL;
   /* Alocação de memória */
   csv = (aqvCSV*) malloc (sizeof(aqvCSV));
   if(csv == NULL) return NULL;
   /* Preenche atributos */
   csv->aqv = aqv;
   csv->tabela = geraTabela(aqv);
   if (csv->tabela == NULL){ destroiAqvCSV(csv); return NULL; }
   csv->numCols = contaColunas(aqv);
   csv->numLins = contaLinhas(aqv);
   csv->arrVar = geraVariaveis(aqv);
   if (csv->arrVar == NULL){ destroiAqvCSV(csv); return NULL; }

   return (csv);
}

void destroiAqvCSV(aqvCSV* csv)
{
   if (csv->tabela) free(csv->tabela);
   if (csv->arrVar) free(csv->arrVar);
   free(csv);
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
/* Recebe um número inteiro.
 * Retorna um*/
int digitos(int n)
{
   int numDigitos = 0;

   do { n = n / 10; numDigitos++; } while (n != 0);
   return (numDigitos);
}

int mostrar(aqvCSV* csv)
{
   int i, j, k, tamMaximo;
   char buffer[MAX_BUF], *substr;

   /* Impressão cabeçalho */
   fgets(buffer, MAX_BUF, csv->aqv);
   substituiVirgulas(buffer);
   if (csv->numLins > 11){
      tamMaximo = 3;
      if (digitos(csv->numLins) > tamMaximo) tamMaximo = digitos(csv->numLins);
      printf("%*s ", tamMaximo, " ");
   }
   else printf("  ");
   for (i = 0; i < csv->numCols; i++)
      printf("%*s ", csv->arrVar[i].maxLen, separaString(buffer, i));
   printf("\n");

   /* Impressão conteúdo */
   if (csv->numLins <= 11) {  /* Impressão sem cortes de linhas */
      for (i = 0; i < csv->numLins - 1; i++){
         printf("%d ", i);
         fgets(buffer, MAX_BUF, csv->aqv);
         substituiVirgulas(buffer);
         for (j = 0; j < csv->numCols; j++){
            substr = separaString(buffer, j);
            tamMaximo = csv->arrVar[j].maxLen;
            (substr == NULL) ? printf("%*s ", tamMaximo, "NaN") : printf("%*s ", tamMaximo, substr);
         }
         printf("\n");
      }
   }
   else {
      /* Impressão com cortes de linhas*/
      /* Impressão das cinco primeira linhas */
      for (i = 0; i < 5; i++){
         tamMaximo = 3;                                                             /* A string "..." possui três dígitos */
         if (digitos(csv->numLins) > tamMaximo) tamMaximo = digitos(csv->numLins);  /* Se os números tem mais de 3 dígitos, imprime com o num. de dígitos */
         printf("%-*d ", tamMaximo, i);
         fgets(buffer, MAX_BUF, csv->aqv);
         substituiVirgulas(buffer);
         for (j = 0; j < csv->numCols; j++){
            substr = separaString(buffer, j);
            tamMaximo = 3;
            if(csv->arrVar[j].maxLen > 3) tamMaximo = csv->arrVar[j].maxLen;
            (substr == NULL) ? printf("%*s ", tamMaximo, "NaN") : printf("%*s ", tamMaximo, substr);
         }
         printf("\n");
      }
      /* Impressão da linha com "..." */
      tamMaximo = 3;                                                    /* A string "..." possui três dígitos */
      if (digitos(csv->numLins) > tamMaximo) tamMaximo = digitos(csv->numLins);   /* Se os números tem mais de 3 dígitos, imprime com o num. de dígitos */
      printf("%-*s ", tamMaximo, "...");
      for (j = 0; j < csv->numCols; j++){
         tamMaximo = 3;
         if (csv->arrVar[j].maxLen > 3) tamMaximo = csv->arrVar[j].maxLen;
         printf("%*s ", tamMaximo, "...");
      }
      printf("\n");

      while (i < csv->numLins - 6){ fgets(buffer, MAX_BUF, csv->aqv); i++; }   /* Pula até a quinta última linha */

      /* Impressão das últimas cinco linhas */
      for (j = i; j < csv->numLins - 1; j++){
         tamMaximo = 3;
         if (digitos(csv->numLins) > tamMaximo) tamMaximo = digitos(csv->numLins);
         printf("%-*d ", tamMaximo, j);
         fgets(buffer, MAX_BUF, csv->aqv);
         substituiVirgulas(buffer);
         for (k = 0; k < csv->numCols; k++){
            substr = separaString(buffer, k);
            tamMaximo = 3;
            if (csv->arrVar[k].maxLen > 3) tamMaximo = csv->arrVar[k].maxLen;
            (substr == NULL) ? printf("%*s ", tamMaximo, "NaN") : printf("%*s ", tamMaximo, substr);
         }
         printf("\n");
      }
   }

   printf("\n[%d rows x %d columns]\n", csv->numLins - 1, csv->numCols);

   fseek(csv->aqv, 0, SEEK_SET);
   return (1);
}

/*------------------------------
      FILTROS 
 ------------------------------*/

int compIgual(char* var, int valor){
   return (1);
}

int compMaior(char* var, int valor){
   return (1);
}

int compMaiorIgual(char* var, int valor){
   return (1);
}

int compMenor(char* var, int valor){
   return (1);
}

int compMenorIgual(char* var, int valor){
   return (1);
}

int compDiferente(char* var, int valor){
   return (1);
}

int filtros(aqvCSV* csv, int (*filt) (char*, int), int valor)
{
   return (1);
}

/*------------------------------
      DESCRIÇÃO DOS DADOS
 ------------------------------*/
/* Recebe o nome de uma variável, o vetor de variáveis e o tamanho do vetor. 
 * Retorna o índice da variável no vetor, ou -1 caso não ache a variável. */
int procuraVariavel(char* var, variavel* arrVar, int max)
{
   int i;
   
   for (i = 0; i < max && strcmp(arrVar[i].nome, var); i++) ;
   if (i == max) return (-1);
   else return (i);
}

/* Recebe uma struct aqvCSV e o índice da coluna de uma variável numérica.
 * Faz a descrição numérica da variável. */
void descricaoNum(aqvCSV* csv, int indVar){
   dado* dados;
   int contador, qntdModa;
   float valor, media, mediana, desvPadrao;
   char buffer[MAX_BUF], *substr, *moda, *valMin, *valMax, **valUnicos;
   
   fgets(buffer, MAX_BUF, csv->aqv); /* Pula a primeira linha */
   
   contador = 0;
   media = 0;
   valUnicos = (char**) malloc (csv->numLins * sizeof(char*));
   dados = (dado*) malloc (csv->numLins * sizeof(dado));
   while (!feof(csv->aqv)){
      fgets(buffer, MAX_BUF, csv->aqv);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL){
         strcpy(dados[contador].valor, substr);
         contador++;
      }
   }
   fseek(csv->aqv, 0, SEEK_SET);
   
   printf("Contador: %d\n", contador);
   printf("Media: %.1f\n", media);
   printf("Desvio: %.1f\n", desvPadrao);
   printf("Mediana: %.1f\n", mediana);
   printf("Moda: %s %d vezes\n", moda, qntdModa);
   printf("Min.: %s\n", valMin);
   printf("Max.: %s\n", valMax);
   printf("Valores unicos: [");
   //imprime(valUnicos);
   printf("]\n");

   free(valUnicos);
   free(dados);
}

/* Recebe uma struct aqvCSV e o índice da coluna de uma variável string.
 * Faz a descrição de string da variável. */
void descricaoString(aqvCSV* csv, int indVar){
   dado* dados;
   int i, contador, qntdModa;
   char buffer[MAX_BUF], *substr, *moda, **valUnicos;

   fgets(buffer, MAX_BUF, csv->aqv); /* Pula a primeira linha */
   
   contador = 0;
   dados = (dado*) malloc (csv->numLins * sizeof(dado));
   valUnicos = (char**) malloc (csv->numLins * sizeof(char*));
   while (!feof(csv->aqv)){
      fgets(buffer, MAX_BUF, csv->aqv);
      substituiVirgulas(buffer);

      substr = separaString(buffer, indVar);
      if (substr != NULL){
         strcpy(dados[contador].valor, substr);
         contador++;
      }
   }
   for (i = 0; i < contador; i++){
      
   }

   printf("Contador: %d\n", contador);
   printf("Moda: %s %d vezes\n", moda, qntdModa);
   printf("Valores unicos: [");
   //imprime(ValUnicos);
   printf("]\n");

   free(dados);
   free(valUnicos);
}

int descricaoDados(aqvCSV* csv, char* var)
{
   int indVar, numCols;
   variavel* arrVar;
   
   if (csv->numCols == 0 && csv->arrVar == NULL) return (0);
   indVar = procuraVariavel(var, arrVar, numCols);
   if (indVar == -1) return (0);

   if (csv->arrVar[indVar].tipo == NUMERIC) descricaoNum(csv, indVar);
   else descricaoString(csv, indVar);

   fseek(csv->aqv, 0, SEEK_SET);

   return (1);
}

/*------------------------------
      ORDENAÇÃO 
 ------------------------------*/
int ordenaDados(aqvCSV* csv, char *var, int opt)
{
   int *indices;

   indices = (int*) malloc (csv->numLins * sizeof(int));
   if (indices == NULL) return (0);
   
   
   return (1);
}

/*------------------------------
      SELEÇÃO
 ------------------------------*/
int selecionaDados(aqvCSV* csv, char* var)
{
   return (1);
}

/*------------------------------
      DADOS FALTANTES
 ------------------------------*/
int dadosFaltantes(aqvCSV* csv, int escolha)
{
   return (1);
}

/*------------------------------
      SALVAR DADOS
 ------------------------------*/
int salvaDados(aqvCSV* csv, char* nomeAqv)
{
   FILE* aqvNovo;
   char buffer[MAX_BUF];

   aqvNovo = fopen(nomeAqv, "w+");
   if (aqvNovo == NULL) return (0);

   while (!feof(csv->aqv)){
      fgets(buffer, MAX_BUF, csv->aqv);
      fputs(buffer, aqvNovo);
   }
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
