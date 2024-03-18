#include <stdio.h>
#include <string.h>

int main()
{
   /* Ponteiro do tipo FILE: um ponteiro do tipo FILE aponta para uma
   estrutura de controle de arquivo que contém informações importantes:
      - Nome do arquivo;
      - Modo de abertura (leitura, gravação, etc.);
      - Posição atual do ponteiro de leitura;
      - Posição atual do ponteiro de escrita. */
   FILE *f;

   //-------------------------------------
   // Tema 01: Abrindo e fechando arquivos

   /* Função FILE* fopen(const char *filename, const char *mode):
      - Utilizada para abrir arquivos.
      - Parâmetro filename: Uma string que contém o nome do arquivo
      a ser aberto, incluindo o caminho até o mesmo.
      - Parâmetro mode: Uma string que indica o modo de abertura. */

   /* Modos de abertura:
      - Leitura (Read): Modo utilizado para abrir um arquivo exclusivamente
      para leitura, onde nenhuma operação de escrita é permitida.
      Quando o arquivo não existe, a função fopen() retorna NULL.
      O argumento correspondente ao parâmetro 'mode' é "r".
      - Escrita (Write): Modo utilizado para abrir um arquivo exclusivamente
      para escrita, onde nenhuma operação de leitura é permitida.
      Quando o arquivo não existe, a função fopen() cria o arquivo e retorna
      o seu ponteiro. Caso o arquivo exista, este tem seu conteúdo resetado.
      O argumento correspondente ao parâmetro 'mode' é "w".
      - Concatenação (Append): Modo utilizado para abrir um arquivo para
      leitura e concatenação de dados (isto é, escrita no final do arquivo).
      Quando o arquivo não existe, a função fopen() cria o arquivo e retorna
      o seu ponteiro. Caso o arquivo exista, o seu conteúdo é preservado.
      O argumento correspondente ao parâmetro 'mode' é "a". */
   
   /* Modos de abertura derivados: Existem modos derivados dos anteriores
   que envolvem a inclusão do símbolo "+" após o modo de abertura. Nesses
   casos, independente do modo, o arquivo é aberto para leitura e escrita,
   e caso não exista, um novo arquivo é criado.
      - "r+": O conteúdo é preservado; Ponteiros são alocados para o início
      do arquivo.
      - "w+": O conteúdo é apagado; Ponteiros são alocados para o início 
      do arquivo.
      - "a+": O conteúdo existente é preservado; O ponteiro de leitura é
      alocado no início do arquivo, e o ponteiro de escrita é alocado no
      final do arquivo. */
   f = fopen("meuArquivo.txt", "a");

   //---------------------------------------
   // ÁREA DEDICADA A FUNÇÕES
   //---------------------------------------
   // Tema 02: Escrita em Arquivos

   









   //----------------------------------------
   /* Função int fclose(FILE* stream):
      - Utilizada para fechar arquivos abertos.
      - A função retorna zero (0) caso o fechamento seja bem sucedido.
      - O fechamento do arquivo encerra o processo de manipulação de
      seus dados, libera os recursos computacionais alocados para tais
      processos e esvazia os buffers de saída. */
   fclose(f);
}