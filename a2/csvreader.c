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

   if (!verificaArquivo(aqv)){
      fclose(aqv);
      fprintf(stderr, "Arquivo com erros de formatação!\n");
      exit(3);
   }
   free(nome_arquivo);

   while (1) {
      printf("1) Sumario do Arquivo\n"
             "2) Mostrar\n"
             "3) Filtros\n"
             "4) Descricao dos Dados\n"
             "5) Ordenacao\n6) Selecao\n"
             "7) Dados Faltantes\n"
             "8) Salvar Dados\n"
             "9) Fim\n");
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
      printf("\nPressione ENTER para continuar\n");
      setbuf(stdin, NULL);
      while (getchar() != '\n') ;
   }
}
