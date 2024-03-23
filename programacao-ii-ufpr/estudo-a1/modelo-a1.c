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

int conta_atributos(FILE *arff){
   // VOCE DEVE IMPLEMENTAR ESTA FUNCAO COMO PARTE DO A1!
   char line[1025], *token;
   int num_attr = 0;

   fgets(line, 1026, arff);
   token = strtok(line, " ");
   while (strcmp(token, "@data")) {
      num_attr++;
      fgets(line, 1026, arff);
      token = strtok(line, " ");
   }

   return (num_attr);
}

atributo* processa_atributos(FILE *arff){
   // VOCE DEVE IMPLEMENTAR ESTA FUNCAO COMO PARTE DO A1!
   int tam_infos;
   atributo *infos;
   
   if (invalidoArff(arff))
      return NULL;

   tam_infos = conta_atributos(arff);
   
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
   atributo *infos;

   if (!file) return (1);

   /*
   atributo *infos;
   infos = processa_atributos(file);
   if (!infos) return (1);
   */
  
   int tam_infos;
   tam_infos = conta_atributos(file);
   printf("Numero de atributos: %d\n", tam_infos);

   if (exibicao){
   // VOCE DEVE CHAMAR A FUNCAO DE EXIBICAO AQUI (USE A FUNCAO exibe_atributos)
      exibe_atributos(infos, tam_infos);
   }

  return (0);
}
