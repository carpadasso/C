// Aula 03: Codificação de Caracteres e Strings
#include <stdio.h>
#include <string.h>

int main()
{
   // --------------------------------------------------
   // Tema 01: Strings Constantes
   /*
   // Declarações de strings constantes
   char *possibilidade1 = "PROG2";
   char possibilidade2[] = "PROG2";
   char possibilidade3[] = { 'P', 'R', 'O', 'G', '2', '\0' };

   // Para escrever uma string na tela via printf:
   // - Utilizamos o indicador de tipo "%s"
   // - Passamos o ponteiro para a string desejada como argumento
   // Ocorrerá a escrita de todos os caracteres até que o caractere nulo
   // (\0) seja encontrado.
   printf("%s -- %d\n", possibilidade1, possibilidade1);
   printf("%s -- %d\n", possibilidade2, possibilidade2);
   printf("%s -- %d\n", possibilidade3, possibilidade3);
   */

   // --------------------------------------------------
   // Tema 02: Formas alternativas de escrita de Strings
   /*
   char *string = "PROG2";

   printf("Alternativa com puts: \n");
   puts(string);

   printf("Alternativa iterativa: \n");
   for (int i = 0; string[i]; i++)
      putchar(string[i]);
   */

   // --------------------------------------------------
   // Tema 03: Leitura de Strings
   /*
   char string[101];

   printf("Digite uma string: ");
   // Alternativa 01: Utilizando scanf("%s")
   // A leitura é realizada até que um espaço, tabulação ou fim de
   // linha seja encontrado.
   scanf("%s", string);

   // Alternativa 02: Utilizando scanf("%Ns")
   // A leitura é realizada até vinte (20) caracteres, descartando
   // o excedente.
   scanf("%20s", string);

   // Alternativa 03: scanf("%[A-Za-z0-9]")
   // Apenas caracteres alfanuméricos são considerados.
   scanf("%[A-Za-z0-9]", string);

   // Alternativa 04: scanf("%[^\n]")
   // Todos os caracteres que não são quebra de linha são lidos; A
   // leitura para na quebra de linha (que não é lida).
   scanf("%[^\n]", string);
   // A função getchar() faz a limpeza do buffer, removendo a quebra
   // de linha do mesmo.
   getchar();

   printf("A string digitada foi: %s", string);
   */

   // --------------------------------------------------
   // Tema 04: Manipulação de Strings
   char string[101];

   printf("Digite uma string: ");
   scanf("%s", string);

   // Função strlen(): Recebe o ponteiro para uma string como argumento;
   // Retorna a quantidade de caracteres na mesma (sem contar o '\0').
   printf("A string digitada tem %d caracteres!", strlen(string));

   // Função strcpy(): Recebe, em ordem, o ponteiro para onde a string
   // base deve ser copiada (um vetor de char com tamanho suficiente) e
   // o ponteiro para a string base; retorna o ponteiro para a string
   // copiada.
   char copia[101];

   strcpy(copia, string);
   printf("A string digitada foi: %s", copia);

   // Função strncpy(): Mesmo funcionamento da strcpy(), porém recebe um
   // terceiro argumento inteiro indicando quantos caracteres devem ser
   // copiados, no máximo (excluindo o '\0').
   char copia[101];

   strncpy(copia, string, 4);
   printf("A string digitada foi: %s", copia);

   // Função strcat(): Recebe dois ponteiros para strings, sendo que a
   // segunda string deve ser concatenada na primeira (a primeira string
   // deve ter espaço previamente alocado), ou seja, acopla a segunda
   // string no final da primeira.
   // Retorna o ponteiro para a string copiada.
   char string01[101], string02[101];

   printf("Digite uma string: ");
   scanf("%s", string01);
   printf("Digite outra string: ");
   scanf("%s", string02);
   if ((strlen(string01) <= 50) && strlen(string02) <= 50){
      strcat(string01, string02);
      printf("A concatenacao das strings e: %s", string01);
   }

   // Função strncat(): Mesmo funcionamento da strcat(), porém recebe
   // um terceiro argumento inteiro indicando quantos caracteres devem
   // ser concatenados, no máximo (excluindo o '\0').
   char string01[101], string02[101];

   printf("Digite uma string: ");
   scanf("%s", string01);
   printf("Digite outra string: ");
   scanf("%s", string02);
   if (strlen(string01) <= 50){
      strncat(string01, string02, 50);
      printf("A concatenacao das strings e: %s", string01);
   }

   // Função strchr(): Recebe um ponteiro para string e um caractere
   // qualquer.
   // Retorna o ponteiro para a primeira ocorrência do caractere na string
   // fornecida. Se não houver ocorrências, retorna NULL (0).
   char *busca;

   busca = strchr(string, 'a');
   if (busca != 0)
      printf("O primeiro 'a' encontrado esta no indice %d", busca-string);
   else
      printf("Nao existe a letra 'a' na string!");
   
   // Função strrchr(): Recebe um ponteiro para string e um caractere
   // qualquer.
   // Retorna o ponteiro para a última ocorrência do caractere na string
   // fornecida. Se não houver ocorrências, retorna NULL (0).
   char *busca;

   busca = strrchr(string, 'a');
   if (busca != 0)
      printf("O ultimo 'a' encontrado esta no indice %d", busca-string);
   else
      printf("Nao existe a letra 'a' na string!");
   
   // Função strstr(): Recebe um ponteiro para string e um para uma
   // substring.
   // Retorna o ponteiro para a primeira ocorrência da substring na
   // string fornecida. Se não houver ocorrências, retorna NULL (0).
   char *busca;

   busca = strstr(string, 'aba');
   if (busca != 0)
      printf("A substring esta a partir do indice %d", busca-string);
   else
      printf("Nao existe a substring 'aba' na string!");

   // Função strdup(): Recebe um ponteiro para string como argumento e
   // duplica a mesma, alocando a memória necessária.
   // Retorna o ponteiro para a string duplicada.
   char string01[101], *string02;

   printf("Digite uma string: ");
   scanf("%s", string01);
   string02 = strdup(string01);
   printf("A string digitada foi: %s", string02);

   // Função strtok(): Recebe um ponteiro para string base e um para
   // uma substring separadora.
   // Retorna o ponteiro para a primeira ocorrência de um caractere
   // na string base antes/após a substring fornecida. Se não houver
   // ocorrências, retorna NULL (0).
   char separador[] = " - ", *token;

   printf("Digite uma string: ");
   scanf("%[^\n]", string);
   getchar();

   token = strtok(string, separador);
   do {
      printf("O token encontrado foi: %s\n", token);
   } while (token);

   return (0);
}