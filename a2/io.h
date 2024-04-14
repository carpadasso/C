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
 * - tipoVariável: Tipo da variável (Numérica e String) */
typedef struct {
   char nome[MAX_BUF];
   enum tipoVariavel tipo;
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
 * - tabela: Vetor de linhas que guarda as linhas do arquivo
 * - numCols: Número de colunas do arquivo
 * - numLins: Número de linhas do arquivo
 * - printLins: Vetor que armazena os índices de linhas usáveis
 * - printCols: Vetor que armazena os índices de colunas usáveis
 * - arrVar: Vetor de variáveis */
typedef struct {
   FILE* aqv;
   linha* tabela;
   int numCols;
   int numLins;
   int* printLins;
   int* printCols;
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

/* Grava as informações do arquivo em nomeAqv */
int gravaCSV(aqvCSV* csv, char* nomeAqv);
/* --------------- */

/* --------------- */
/* Manipulação de arquivo CSV */

/* Recebe uma struct aqvCSV.
 * Retorna 0 caso haja um problema de formatação do arquivo, ou 1 caso contrário. */
int verificaArquivo(aqvCSV* csv);

/* Imprime o nome das variáveis e o tipo de cada variável. */
int sumario(aqvCSV* csv);

/* Imprime a tabela completa do arquivo. */
int mostrar(aqvCSV* csv);

/* Recebe uma variável, um filtro e um valor.
   Imprime a tabela filtrada. */
int filtros(aqvCSV* csv, int (*filt) (char* s1, char* s2), char* var, char* valor);
int compIgual(char* s1, char* s2);
int compMaior(char* s1, char* s2);
int compMaiorIgual(char* s1, char* s2);
int compMenor(char* s1, char* s2);
int compMenorIgual(char* s1, char* s2);
int compDiferente(char* s1, char* s2);

/* Imprime diversas descrições sobre o arquivo. 
   Variável Numérica: Contagem, Média, Mediana, Moda, Desvio Padrão
   Valor Mínimo, Valor Máximo, e Valores Únicos.
   Variável String: Contagem, Moda e Valores Únicos. */
int descricaoDados(aqvCSV* csv, char* var);

/* Imprime a tabela do arquivo de forma ordenada,
   de forma ascendente ou descendente. */
int ordenaDados(aqvCSV* csv, char* var, int op);

/* Imprime apenas as variáveis selecionadas. */
int selecionaDados(aqvCSV* csv, char* var);

/* Resolve a respeito dos dados faltantes no arquivo. */
int dadosFaltantes(aqvCSV* csv, int escolha);

/* Salva em um novo arquivo os dados atuais do arquivo. */
int salvaDados(aqvCSV* csv, char* nomeAqv);
/* --------------- */

#endif  
