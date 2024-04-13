#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

#define MAX_BUF 1024

int main(int argc, char** argv)
{
   aqvCSV* csv;
   FILE *aqv;

   int opt, escolha, op;
   char c, *filename, nomeAqv[MAX_BUF], var[MAX_BUF], valor[MAX_BUF], comparador[MAX_BUF];
   
   int (*filt) (char *s1, char* s2);

   if (argc != 2) {
      fprintf(stderr, "Uso do programa: ./csvreader <arquivo.csv>\n");
      exit(1);
   }

   filename = strdup(argv[1]);
   aqv = fopen(filename, "r");
   if (aqv == NULL) {
      fprintf(stderr, "Erro ao abrir o arquivo.\n");
      exit(2);
   }
   free(filename);

   csv = criaAqvCSV(aqv);
   if (csv == NULL){
      fprintf(stderr, "Impossível gerar dados do arquivo.\n");
      exit(4);
   }

   if (!verificaArquivo(csv)){
      fclose(aqv);
      fprintf(stderr, "Arquivo com erros de formatação!\n");
      exit(3);
   }
   
   printf("Carregando...\n\n");

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
            /* Opção 1 - Sumário */
            if (!sumario(csv))
               printf("Erro ao exibir o sumario.\n");
            break;
         case 2:
            /* Opção 2 - Mostrar */
            if (!mostrar(csv))
               printf("Erro ao mostrar os dados do arquivo.\n");
            break;
         case 3:
            /* Opção 3 - Filtros*/
            printf("Entre com a variavel: ");
            scanf("%s", var);

            printf("Escolha um filtro ( == > >= < <= != ): ");
            scanf("%s", comparador);
            
            if (!strcmp(comparador, "==")) filt = compIgual;
            else if (!strcmp(comparador, ">")) filt = compMaior;
            else if (!strcmp(comparador, ">=")) filt = compMaiorIgual;
            else if (!strcmp(comparador, "<")) filt = compMenor;
            else if (!strcmp(comparador, "<=")) filt = compMenorIgual;
            else if (!strcmp(comparador, "!=")) filt = compDiferente;
            else {
               printf("Filtro invalido.\n");
               break;
            }

            printf("Digite um valor: ");
            scanf("%s", valor);
            
            if (!filtros(csv, filt, var, valor)){
               printf("Nao foi possivel aplicar o filtro desejado.\n");
               break;
            }

            break;
         case 4:
            /* Opção 4 - Descrição de Dados */
            printf("Entre com a variavel: ");
            scanf("%s", var);

            if (!descricaoDados(csv, var))
               printf("Erro ao realizar a descricao da variavel.\n");

            break;
         case 5:
            /* Opção 5 - Ordenação */
            printf("Entre com a variavel: ");
            scanf("%s", var);
            
            fflush(stdin);

            printf("Selecione uma opcao [A]scendente ou [D]escendente: ");
            scanf("%c", &c);
            if (c != 'A' && c != 'D'){
               printf("Opcao invalida.\n");
               break;
            }

            if (c == 'A') op = 0;
            else if (c == 'D') op = 1;

            if (!ordenaDados(csv, var, op)){
               printf("Erro ao executar a ordenacao no arquivo.\n");
               break;
            }
            
            fflush(stdin);
            printf("Deseja gravar um arquivo com os dados ordenados? [S|N] ");
            scanf("%c", &c);
            if (c != 'N' && c != 'S'){
               printf("Opcao invalida.\n");
               break;
            }
            
            if (c == 'S'){
               printf("Entre com o nome do arquivo: ");
               scanf("%[^\n]", nomeAqv);
               getchar();

               if (!salvaDados(csv, nomeAqv)){
                  printf("Erro ao gravar o arquivo.\n");
                  break;
               }
               
               printf("Deseja descartar os dados originais? [S|N] ");
               do {
                  c = getchar();
                  if (c != 'N' && c != 'S') printf("Opcao errada, tente novamente: ");
               } while (c != 'N' && c != 'S');
               if (c == 'S') freopen(nomeAqv, "r+", csv->aqv); 
            }

            break;
         case 6:
            /* Opção 6 - Seleção */
            printf("Entre com a(s) variavel(is) que deseja selecionar (separadas por espaco): ");
            scanf("%[^\n]", var);
            getchar();

            if (!selecionaDados(csv, var))
               printf("Nao foi possivel selecionar os dados desejados.\n");
            
            printf("Deseja gravar um arquivo com os dados ordenados? [S|N] ");
            do {
               c = getchar();
               if (c != 'N' && c != 'S') printf("Opcao errada, tente novamente: ");
            } while (c != 'N' && c != 'S');

            if (c == 'S'){
               printf("Entre com o nome do arquivo: ");
               scanf("%[^\n]", nomeAqv);
               getchar();

               if (!salvaDados(csv, nomeAqv)){
                  printf("Erro ao gravar o arquivo.\n");
                  break;
               }
               else printf("Arquivo gravado com sucesso.\n");
            }

            break;
         case 7:
            /* Opção 7 - Dados Faltantes */
            printf("1) Listar registros com NaN\n"
                   "2) Substituir pela media\n"
                   "3) Substituir pelo proximo valor valido\n"
                   "4) Remover registros com NaN\n"
                   "5) Voltar ao menu principal\n");
            printf("Escolha o que fazer com os dados faltantes: ");
            scanf("%d", &escolha);

            if (1 <= escolha && escolha <= 4){
               if (!dadosFaltantes(csv, escolha)){
                  printf("Erro ao executar a operacao com os dados faltantes.\n");
               }
            }
            else if (escolha == 5) break;
            else {
               printf("Escolha invalida.\n");
            }
            
            if (escolha == 1){
               printf("Deseja gravar um arquivo com os dados ordenados? [S|N] ");
               do {
                  c = getchar();
                  if (c != 'N' && c != 'S') printf("Opcao errada, tente novamente: ");
               } while (c != 'N' && c != 'S');

               if (c == 'S'){
                  printf("Entre com o nome do arquivo: ");
                  scanf("%[^\n]", nomeAqv);
                  getchar();

                  if (!salvaDados(csv, nomeAqv)){
                     printf("Erro ao gravar o arquivo.\n");
                     break;
                  }
                  
                  printf("Deseja descartar os dados originais? [S|N] ");
                  do {
                     c = getchar();
                     if (c != 'N' && c != 'S') printf("Opcao errada, tente novamente: ");
                  } while (c != 'N' && c != 'S');
                  if (c == 'S') freopen(nomeAqv, "r+", csv->aqv); 
               }
            }

            break;
         case 8:
            printf("Entre com o nome do arquivo: ");
            scanf("%s", nomeAqv);
            getchar();

            if (!salvaDados(csv, nomeAqv))
               printf("Erro ao salvar o arquivo.\n");
            else
               printf("Arquivo gravado com sucesso.\n");

            break;
         case 9:
            fclose(csv->aqv);
            destroiAqvCSV(csv);

            return (0);
         default:
            printf("Opcao invalida! Tente novamente.\n");
      }
      printf("\nPressione ENTER para continuar\n");
      setbuf(stdin, NULL);
      while (getchar() != '\n') ;
   }
}
