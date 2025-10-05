#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que representa cada território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para limpar buffer do teclado
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função que simula a rolagem de dados de ataque
void atacar(int *atacante, int *defensor) {
    *atacante = rand() % 6 + 1;
    *defensor = rand() % 6 + 1;
}

// Função para cadastrar territórios
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

// Função para exibir mapa do mundo
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

// Função que resolve o ataque entre dois territórios
void atacarTerritorios(Territorio *atacante, Territorio *defensor) {
    int dadoAtaque, dadoDefesa;
    atacar(&dadoAtaque, &dadoDefesa);

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtaque);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu %d tropas.\n\n", defensor->tropas / 2);
        atacante->tropas += defensor->tropas / 2;
        defensor->tropas -= defensor->tropas / 2;
    } else {
        printf("VITORIA DO DEFENSOR! O ataque perdeu 1 tropa.\n\n");
        defensor->tropas += 1;
        atacante->tropas -= 1;
    }

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

// Função para atribuir uma missão a um jogador
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Função para verificar se a missão foi cumprida
// Aqui fazemos uma verificação simples: se existe algum território do jogador com menos de 1 tropa
// Em implementação real, essa lógica deve ser expandida conforme a missão
int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador) {
    // Exemplo simples: se algum território inimigo está completamente eliminado
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) != 0 && mapa[i].tropas == 0) {
            return 1; // missão cumprida
        }
    }
    return 0; // missão não cumprida
}

// Função para liberar memória alocada
void liberarMemoria(Territorio *territorios, char *missao) {
    free(territorios);
    free(missao);
}

int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    int numeroTerritorio;
    printf("Quantos territorios terao? ");
    scanf("%d", &numeroTerritorio);
    limparBufferEntrada();

    // Aloca dinamicamente os territórios
    Territorio *territorio = (Territorio *)calloc(numeroTerritorio, sizeof(Territorio));
    if (territorio == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    // Cadastro inicial e exibição
    cadastrarTerritorios(territorio, numeroTerritorio);
    exibirMapa(territorio, numeroTerritorio);

    // Criação das missões
    char *missoes[5] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Ter todos os territorios de uma cor",
        "Manter um territorio com pelo menos 5 tropas",
        "Controlar pelo menos 4 territorios"
    };

    // Aloca memória para a missão do jogador
    char *missaoJogador = (char *)malloc(100 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memoria para a missão!\n");
        free(territorio);
        return 1;
    }

    // Atribui missão ao jogador
    atribuirMissao(missaoJogador, missoes, 5);
    printf("\nSua missão é: %s\n", missaoJogador);

    int escolhaAtacante = 1, escolhaDefensor;

    while (1) {
        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", numeroTerritorio);
        if (scanf("%d", &escolhaAtacante) != 1) break;
        limparBufferEntrada();

        if (escolhaAtacante == 0) {
            printf("Jogo encerrado!\n");
            break;
        }
        if (escolhaAtacante < 1 || escolhaAtacante > numeroTerritorio) continue;

        printf("Escolha o territorio defensor (1 a %d): ", numeroTerritorio);
        if (scanf("%d", &escolhaDefensor) != 1) break;
        limparBufferEntrada();

        if (escolhaDefensor < 1 || escolhaDefensor > numeroTerritorio) continue;
        if (escolhaDefensor == escolhaAtacante) continue;

        Territorio *atk = &territorio[escolhaAtacante - 1];
        Territorio *def = &territorio[escolhaDefensor - 1];

        // Validação: não pode atacar território da mesma cor
        if (strcmp(atk->cor, def->cor) == 0) {
            printf("Nao e permitido atacar um territorio do mesmo exercito (mesma cor: %s).\n", atk->cor);
            continue;
        }

        if (atk->tropas < 2) {
            printf("Territorio atacante precisa ter pelo menos 2 tropas.\n");
            continue;
        }

        // Executa ataque
        atacarTerritorios(atk, def);
        exibirMapa(territorio, numeroTerritorio);

        // Verifica missão do jogador
        if (verificarMissao(missaoJogador, territorio, numeroTerritorio, atk->cor)) {
            printf("\nParabens! Voce cumpriu sua missao: %s\n", missaoJogador);
            break;
        }

        printf("Pressione ENTER para continuar para o proximo turno...");
        getchar();
    }

    // Libera memória
    liberarMemoria(territorio, missaoJogador);

    return 0;
}
