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
        printf("R�tulo: %s\n", infos[i].rotulo);
        printf("Tipo: %s\n", infos[i].tipo);
        if (infos[i].categorias) printf("Categorias: %s\n", infos[i].categorias);
        if (i < tamanho-1) printf("------------------------------\n");
    }
    printf("===============================\n");
}

int conta_atributos(FILE *arff){
    // VOCE DEVE IMPLEMENTAR ESTA FUNCAO COMO PARTE DO A1!
}

atributo* processa_atributos(FILE *arff){
    // VOCE DEVE IMPLEMENTAR ESTA FUNCAO COMO PARTE DO A1!
}

int main(int argc, char **argv){
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

  if (exibicao){
    // VOCE DEVE CHAMAR A FUNCAO DE EXIBICAO AQUI (USE A FUNCAO exibe_atributos)
  }

  return 0 ;
}
