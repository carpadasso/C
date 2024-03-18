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
   //        ÁREA DEDICADA A FUNÇÕES
   //---------------------------------------
   // Tema 02: Escrita em Arquivos
   /* As operações de escrita permitem adicionar dados em arquivos. Essa
   adição se dá byte a byte a partir do endereço apontado pelo ponteiro
   de escrita de um arquivo aberto. Existem duas categorias principais
   de operações de escrita em arquivos: Simples e Formatada. */

   // Escrita Simples em Arquivos
   /* Função int fputc(int c, FILE* s):
      - Permite escrever um único byte em um arquivo especificado.
      - Parâmetro c: Representa o byte a ser gravado.
      - Parâmetro s: Stream que indica em qual arquivo o byte deve ser
      gravado.
      - Em caso de sucesso, a função retorna o próprio byte escrito.
      -> Opção via macro: int putc(int c, FILE* s)
      -> Opção para o stream stdout: int putchar(int c) */
   fputc('k', f);

   /* Função int fputs(const char *str, FILE* s)
      - Uma operação bastante conveniente é a escrita de strings, em vez
      de escrever cada um de seus caracteres individualmente. A função
      fputs() disponibiliza essa funcionalidade.
      - Parâmetro str: Uma string a ser escrita.
      - Parâmetro s: Stream que indica o arquivo onde a string será escrita.
      - Retorna um valor positivo em caso de sucesso na escrita.
      -> Opção para o stream stdout: int puts(const char *str) */
   fputs("cachorro quente eh muito bom\n", f);

   // Escrita Formatada em Arquivos
   /* Função int fprintf(FILE* s, const char* f, ...)
      - Permite a escrita de arquivos seguindo um padrão de formatação (o
      mesmo do printf). A função possui N parâmetros.
      - Parâmetro s: Stream que indica o arquivo que os dados devem ser
      gravados.
      - Parâmetro f: String determinando o formato de gravação (como no
      printf).
      - Parâmetros adicionais: Dados a serem inseridos nas posições previstas
      do formato.
      - Em caso de sucesso, a função retorna o número de bytes escritos no
      formato.
      -> Opção para string: int sprintf(char* str, const char* f, ...)
      -> Opção para o stream stdout: int printf(const char* f, ...) */
   char *cor = "roxo";
   fprintf(f, "o ceu eh %s\n", cor);

   // Tema 03: Leitura de Arquivos
   /* Se a intenção de escrever em um arquivo é inserir dados nele, de forma
   intuitiva podemos entender que ler de um arquivo é recuperar dados do
   mesmo, utilizando um arquivo previamente aberto. Além disso, as operações
   de leitura de arquivos são categorizadas em Leituras Simples e Formatadas. */

   // Leituras Simples
   /* Função int fgetc(FILE* s)
      - Permite ler um único byte em um arquivo especificado.
      - Parâmetro s: Stream que indica o arquivo que o byte deve ser lido.
      - Em caso de sucesso, a função retorna o byte lido.
      -> Opção via macro: int gect(FILE* s)
      -> Opção para o stream stdin: int getchar() */
   char c;
   c = fgetc(f);

   




   //----------------------------------------
   /* Função int fclose(FILE* stream):
      - Utilizada para fechar arquivos abertos.
      - A função retorna zero (0) caso o fechamento seja bem sucedido.
      - O fechamento do arquivo encerra o processo de manipulação de
      seus dados, libera os recursos computacionais alocados para tais
      processos e esvazia os buffers de saída. */
   fclose(f);
}