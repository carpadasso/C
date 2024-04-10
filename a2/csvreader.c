#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

int main(int argc, char** argv)
{
   FILE *aqv;
   int opt;
   char *nome_arquivo;

   if (argc != 2) {
      fprintf(stderr, "Uso do programa: ./csvreader <arquivo.csv>\n");
      exit(1);
   }

   nome_arquivo = strdup(argv[1]);
   aqv = fopen(nome_arquivo, "r");
   if (!aqv) {
      fprintf(stderr, "Erro ao abrir o arquivo.\n");
      exit(2);
   }

   while (1) {
      printf("1) Sumario do Arquivo\n2) Mostrar\n3) Filtros\n");
      printf("4) Descricao dos Dados\n5) Ordenacao\n6) Selecao\n");
      printf("7) Dados Faltantes\n8) Salvar Dados\n9) Fim\n");
      printf("Escolha a opcao: ");
      scanf("%d", &opt);
      printf("\n");
      switch (opt) {
         case 1:
            sumario(aqv);
            break;
         case 2:
            mostrar(aqv);
            break;
         case 3:
            break;
         case 4:
            break;
         case 5:
            break;
         case 6:
            break;
         case 7:
            break;
         case 8:
            break;
         case 9:
            fclose(aqv);
            return (0);
         default:
            printf("Opcao invalida! Tente novamente.\n");
      }
      printf("Pressione ENTER para continuar\n");
      fflush(stdin);
      while (getchar() != '\n') ;
   }
}
