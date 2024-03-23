#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  char *rotulo;
  char *tipo;
  char *categorias;
} atributo;

void exibe_atributos(atributo *infos, int tamanho){
    if (infos == 0){
        printf("O arquivo ARFF fornecido eh invalido!\n");
        exit(0);
    }

    printf("==== ATRIBUTOS DO ARQUIVO ====\n");
    for(int i = 0; i < tamanho; i++){
        printf("-> Atributo #%d\n", i+1);
        printf("Rotulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
        if (i < tamanho-1) printf("------------------------------\n");
    }
    printf("===============================\n");
}

/* Conta quantos atributos o arquivo possui.
   Retorna a quantidade de atributos. */
int conta_atributos(FILE *arff){
   // VOCE DEVE IMPLEMENTAR ESTA FUNCAO COMO PARTE DO A1!
   char line[1025], *token;
   int num_attr = 0;

   fgets(line, 1025, arff);
   token = strtok(line, " ");
   while (strcmp(token, "@data")) {
      if (!strcmp(token, "@attribute"))
         num_attr++;
      fgets(line, 1025, arff);
      token = strtok(line, " ");
   }

   rewind(arff); /* Traz o ponteiro de leitura para o início */

   return (num_attr);
}

atributo* processa_atributos(FILE *arff){
   // VOCE DEVE IMPLEMENTAR ESTA FUNCAO COMO PARTE DO A1!
   int i, tam_infos;
   char line[1025];
   atributo *infos;
   
   tam_infos = conta_atributos(arff);
   for (i = 0; i < tam_infos; i++) {
      fgets(line, 1026, arff);

      /* Remove o caractere de nova linha da string */
      line[strcspn(line, "\n")] = 0;

      strtok(line, " "); /* Lê a string "@attribute" */

      infos[i].rotulo = strtok(NULL, " "); /* Lê a string que contém o rótulo */

      infos[i].tipo = strtok(NULL, " "); /* Lê a string que contém o tipo */

      infos[i].categorias = NULL;
      /* Caso a string do tipo seja categórica, passa o tipo para categoric */
      if (strcmp(infos[i].tipo, "numeric") && strcmp(infos[i].tipo, "string")) {
         strcpy(infos[i].categorias, infos[i].tipo);
         strcpy(infos[i].tipo, "categoric");
      }
   }

   rewind(arff); /* Traz o ponteiro de leitura para o início */

   return infos;
}

int main(int argc, char **argv)
{
   int opt;
   char exibicao = 0;
   char *entrada = 0;

   while ((opt = getopt(argc, argv, "pi:")) != -1) {
      switch (opt) {
      case 'i':
         entrada = strdup(optarg);
         break;
      case 'p':
         exibicao = 1;
         break;
      default:
         fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
         exit(1);
      }
   }

   if ( ! entrada ) {
      fprintf(stderr, "Forma de uso: ./arff -i <arq_in> [-p]\n");
      exit(2);
   }

   // VOCE DEVE IMPLEMENTAR AS ROTINAS NECESSARIAS E A CHAMADA DE FUNCOES PARA PROCESSAR OS ATRIBUTOS ARFF AQUI
   FILE *file = fopen(entrada, "r");

   if (!file) exit(3);

   atributo *infos;
   infos = processa_atributos(file);
   if (!infos) exit(4);

   int tam_infos;
   tam_infos = conta_atributos(file);
   printf("Atributos: %d\n", tam_infos);

   if (exibicao){
      // VOCE DEVE CHAMAR A FUNCAO DE EXIBICAO AQUI (USE A FUNCAO exibe_atributos)
      exibe_atributos(infos, tam_infos);
   }

   fclose(file);

   return (0);
}
