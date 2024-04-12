#ifndef __IO__
#define __IO__

#define MAX_BUF 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------
 *     Definição de tipos 
 ----------------------------*/
/* Tipo numérico para os tipos de variáveis */
enum tipoVariavel { NUMERIC, STRING };

/* Struct variável:
 * - nome: Nome da variável 
 * - tipoVariável: Tipo da variável (Numérica e String)
 * - maxLen: Número máximo de caracteres a serem impressos
 *   (utilizada na impressão de colunas do arquivo)*/
typedef struct {
   char nome[MAX_BUF];
   enum tipoVariavel tipo;
   int maxLen;
} variavel;

/* Struct linha:
 * - lin: Linha de um arquivo .csv 
 * - indOrig: Índice original da linha no arquivo */
typedef struct {
   char lin[MAX_BUF];
   int indOrig;
} linha;

/* Struct aqvCSV:
 * - aqv: Arquivo .csv
 * - numCols: Número de colunas do arquivo
 * - numLins: Número de linhas do arquivo
 * - arrVar: Vetor de variáveis */
typedef struct {
   FILE* aqv;
   linha* tabela;
   int numCols;
   int numLins;
   variavel* arrVar;
} aqvCSV;

/* ------------------------------
 *     Funções 
 ----------------------------*/

/* --------------- */
/* Manipulação de struct CSV */

/* Aloca dinamicamente uma struct do tipo aqvCSV */
aqvCSV* criaAqvCSV(FILE* aqv);

/* Libera a memória alocada por uma struct do tipo aqvCSV */
void destroiAqvCSV(aqvCSV* csv);
/* --------------- */

/* --------------- */
/* Manipulação de arquivo CSV */

/* Recebe uma struct aqvCSV.
 * Retorna 0 caso haja um problema de formatação do arquivo, ou 1 caso contrário. */
int verificaArquivo(aqvCSV* csv);

int sumario(aqvCSV* csv);

int mostrar(aqvCSV* csv);

int filtros(aqvCSV* csv, int (*filt) (char* var, int valor), int valor);
int compIgual(char* var, int valor);
int compMaior(char* var, int valor);
int compMaiorIgual(char* var, int valor);
int compMenor(char* var, int valor);
int compMenorIgual(char* var, int valor);
int compDiferente(char* var, int valor);

int descricaoDados(aqvCSV* csv, char* var);

int ordenaDados(aqvCSV* csv, char* var, int op);

int selecionaDados(aqvCSV* csv, char* var);

int dadosFaltantes(aqvCSV* csv, int escolha);

int salvaDados(aqvCSV* csv, char* nomeAqv);
/* --------------- */

#endif  
