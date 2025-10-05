#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;

} Territorio;

int main(){

    Territorio territorio[5];

   printf("=========================================================\n");
   printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n");
   printf("=========================================================\n\n");

   for (int i = 0; i < 5; i++)
   {

    printf("\n=== Cadastrando Territorio %d ===", i + 1);

    printf("\n\nNome do territorio: ");
    scanf("%s", territorio[i].nome);
    printf("\nCor do territorio: ");
    scanf("%s", territorio[i].cor);
    printf("\nNumeros de tropas: ");
    scanf("%d", &territorio[i].tropas);

   }

   printf("\n\nCadastro inicial concluido com sucesso!\n\n");

   printf("=========================================================\n");
   printf("               MAPA DO MUNDO - ESTADO ATUAL  ");
   printf("\n=========================================================\n\n");

   for (int i = 0; i < 5; i++)
   {
        printf("Territorio %d:\n\n", i + 1);
        printf("  - Nome: %s\n", territorio[i].nome);
        printf("  - Dominado por: %s\n", territorio[i].cor);
        printf("  - Tropas: %d\n\n", territorio[i].tropas);
   }
   
}
