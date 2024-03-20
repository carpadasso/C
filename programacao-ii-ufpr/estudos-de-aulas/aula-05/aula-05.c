// Aula 05: Arquivos Binários
#include <stdio.h>

int main()
{
   FILE* file;

   //-------------------------------------------------------
   // Tema 01: O que são arquivos binários
   /* Em suma, podemos compreender um arquivo como binário quando o mesmo
   contempla dados não textuais (isto é, que não seguem nenhuma tablea de
   codificação de símbolos, como ASCII ou UTF).
   
   Quando nós falamos em arquivos contendo imagens, músicas, texto formatado
   e binários (entre outros), estamos nos refereindo a arquivos binários!
   
   Tecnicamente, podemos entender um arquivo binário como uma sequência de 
   blocos de dados de mesmo tamanho.
   
   Por óbvio, o tamanho dos blocos de dados depende do tipo de informação
   persistida em um arquivo binário (define normalmente o "passo" de leitura). */

   //-------------------------------------------------------
   // Tema 02: Manipulação de Arquivos Binários
   /* Sabendo das características específicas dos arquivos binários, podemos
   inferir que vão existir funções específicas para realizar a leitura e escrita
   dos mesmos:
      - size_t fread(void* data, size_t size, size_t count, FILE* stream)
      - size_t fwrite(const void* data, size_t size, size_t count, FILE* stream)
   Essas funções operam através da ideia de blocos de dados, sendo que um deter-
   minado bloco é definido pelo número de bytes que o compõem. */

   /* Função fread()
   Destinada à leitura de blocos de dados de um arquivo binário.
   Parâmetros:
      - data: buffer que armazenará os blocos lidos de um arquivo.
      - size: tamanho do bloco de dados, em bytes.
      - count: quantidade de blocos de dados a serem lidos.
      - stream: arquivo de origem dos blocos a serem lidos.
   Retorna a quantidade de blocos lidos com sucesso no buffer. */
   file = fopen("citizens.bin", "rb");
   int struct_bloco1;
   fread(&struct_bloco1, sizeof(struct_bloco1), 1, file);
   fclose(file);

   /* Função fwrite()
   Destina-se à escrita de blocos de dados de um arquivo binário.
   Parâmetros:
      - data: buffer que armazena os blocos a serem escritos no arquivo.
      - size: tamanho do bloco de dados, em bytes.
      - count: quantidade de blocos de dados a serem escritos.
      - stream: arquivo de destino dos blocos a serem escritos.
   Retorna a quantidade de blocos escritos com sucesso no arquivo. */
   file = fopen("exemplo.bin", "wb");
   int struct_bloco2 = 1;
   fwrite(&struct_bloco2, sizeof(int), 1, file);
   fclose(file);

   //--------------------------------------------------------
   // Tema 03: Reposicionamento de ponteiros de um arquivo
   /* É importante ressaltar que, para todo arquivo aberto, o Sistema Ope-
   racional mantém um ponteiro guardando a posição na qual será realizada
   a próxima operação de arquivo. 
   
   Por default, as operações de arquivo sempre avançam o seu respectivo pon-
   teiro, realizando um acesso sequencial até o final do arquivo. 
   
   Porém, eventualmente e dependendo do tipo de arquivo acessado, é necessário
   realizar acesso direto a algumas posições de um arquivo. */

   /* Para realizar acesso a posições aleatórias de um arquivos, existe um
   conjunto de funções específicas disponíveis:
      - int fseek(FILE* stream, long int offset, int whence)
      - void rewind(FILE* stream)
      - long int ftell(FILE* stream)
   Vale ressaltar que, no caso das funções de posicionamento, todas as po-
   sições se dão em termos dos bytes de um arquivo, não de blocos. */

   /* Função fseek()
   É a função de reposicionamento do ponteiro de um arquivo.
   Parâmetros:
      - stream: arquivo em que se deseja mover o ponteiro.
      - offset: tamanho do deslocamento, em número de bytes.
      - whence: parâmetro de para aplicação do deslocamento (SEEK_SET,
      SEEK_END, SEEK_CUR) */

}