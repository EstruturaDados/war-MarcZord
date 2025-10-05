#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que representa cada território do jogo
typedef struct {
    char nome[30];  // Nome do território
    char cor[10];   // Cor do exército que controla o território
    int tropas;     // Número de tropas no território
} Territorio;

// Função para limpar o buffer do teclado, evitando entradas indesejadas
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função que simula a rolagem de dados de ataque e defesa
void atacar(int *atacante, int *defensor) {
    *atacante = rand() % 6 + 1;  // Dado do atacante (1 a 6)
    *defensor = rand() % 6 + 1;  // Dado do defensor (1 a 6)
}

// Função para cadastrar territórios dinamicamente
void cadastrarTerritorios(Territorio *territorios, int quantidade) {
    printf("=========================================================\n");
    printf("Vamos cadastrar os territorios iniciais do nosso mundo.\n");
    printf("=========================================================\n\n");

    for (int i = 0; i < quantidade; i++) {
        printf("\n=== Cadastrando Territorio %d ===\n", i + 1);
        printf("Nome do territorio: ");
        scanf("%s", territorios[i].nome);
        printf("Cor do territorio: ");
        scanf("%s", territorios[i].cor);
        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    printf("\nCadastro inicial concluido com sucesso!\n\n");
}

// Função para exibir o mapa atual do mundo
void exibirMapa(Territorio *territorios, int quantidade) {
    printf("=========================================================\n");
    printf("               MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("=========================================================\n\n");

    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1,
               territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("\n");
}

// Função que resolve o ataque entre dois territórios usando dados aleatórios
void atacarTerritorios(Territorio *atacante, Territorio *defensor) {
    int dadoAtaque, dadoDefesa;
    atacar(&dadoAtaque, &dadoDefesa);

    // Exibe resultado da rolagem
    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtaque);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefesa);

    // Determina vencedor e atualiza tropas
    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu %d tropas.\n\n", defensor->tropas / 2);
        atacante->tropas += defensor->tropas / 2;
        defensor->tropas -= defensor->tropas / 2;
    } else {
        printf("VITORIA DO DEFENSOR! O ataque perdeu 1 tropa.\n\n");
        defensor->tropas += 1;
        atacante->tropas -= 1;
    }

    // Verifica se algum território foi conquistado
    if (atacante->tropas <= 0) {
        printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s!\n\n",
               defensor->nome, atacante->cor);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas += 1;
    } else if (defensor->tropas <= 0) {
        printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s!\n\n",
               atacante->nome, defensor->cor);
        strcpy(atacante->cor, defensor->cor);
        atacante->tropas += 1;
    }
}

// Função para liberar memória alocada dinamicamente
void liberarMemoria(Territorio *territorios) {
    free(territorios);
}

int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    int numeroTerritorio;

    // Solicita quantidade de territórios
    printf("Quantos territorios terao? ");
    scanf("%d", &numeroTerritorio);
    limparBufferEntrada();

    // Aloca dinamicamente memória para os territórios
    Territorio *territorio = (Territorio *)calloc(numeroTerritorio, sizeof(Territorio));
    if (territorio == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    // Cadastro inicial e exibição do mapa
    cadastrarTerritorios(territorio, numeroTerritorio);
    exibirMapa(territorio, numeroTerritorio);

    int escolhaAtacante = 1, escolhaDefensor;

    // Loop principal de ataque
    while (1) {
        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", numeroTerritorio);
        if (scanf("%d", &escolhaAtacante) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }
        limparBufferEntrada();

        if (escolhaAtacante == 0) {
            printf("Jogo encerrado!\n");
            break;
        }
        if (escolhaAtacante < 1 || escolhaAtacante > numeroTerritorio) {
            printf("Territorio atacante invalido.\n");
            continue;
        }

        printf("Escolha o territorio defensor (1 a %d): ", numeroTerritorio);
        if (scanf("%d", &escolhaDefensor) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }
        limparBufferEntrada();

        if (escolhaDefensor < 1 || escolhaDefensor > numeroTerritorio) {
            printf("Territorio defensor invalido.\n");
            continue;
        }
        if (escolhaDefensor == escolhaAtacante) {
            printf("Nao e possivel atacar o mesmo territorio.\n");
            continue;
        }

        // Ponteiros para os territórios escolhidos
        Territorio *atk = &territorio[escolhaAtacante - 1];
        Territorio *def = &territorio[escolhaDefensor - 1];

        // Validação: não pode atacar território da mesma cor
        if (strcmp(atk->cor, def->cor) == 0) {
            printf("Nao e permitido atacar um territorio do mesmo exercito (mesma cor: %s).\n", atk->cor);
            continue;
        }

        // Validação: atacante precisa ter pelo menos 2 tropas
        if (atk->tropas < 2) {
            printf("Territorio atacante precisa ter pelo menos 2 tropas para atacar (atual: %d).\n", atk->tropas);
            continue;
        }

        // Executa ataque
        atacarTerritorios(atk, def);

        // Exibe mapa atualizado
        exibirMapa(territorio, numeroTerritorio);

        printf("Pressione ENTER para continuar para o proximo turno...");
        getchar();
    }

    // Libera memória alocada dinamicamente
    liberarMemoria(territorio);
    return 0;
}
