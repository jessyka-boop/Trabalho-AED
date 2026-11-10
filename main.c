#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h> // Para diagnosticar erros de arquivo

// --- ESTRUTURA DE DADOS ---
typedef struct {
    char nome[30];
    char inicial;
    int pontuacao;
} Casa;

// --- PROTÓTIPOS ---
void inicializarCasas(Casa *casas);
void coletarInfoGeral(float *data, char *modalidade, char *nome_game);
void coletarDesempenho(Casa *casas);
void aplicarBonus(Casa *casas, char modalidade);
void ordenarPlacar(Casa *casas);
void exibirResultados(Casa *casas, float data, char modalidade, char *nome_game);
void salvarHistorico(Casa *casas, float data, char modalidade, char *nome_game);
void limparBuffer();

// --- FUNÇÃO PRINCIPAL ---
int main() {
    float data;
    char modalidade;
    char nome_game[50] = "";

    // Alocação de memória limpa (calloc)
    Casa *casas = (Casa*) calloc(4, sizeof(Casa));
    if (casas == NULL) {
        printf("Erro critico de memoria.\n");
        return 1;
    }

    inicializarCasas(casas);

    // 1. Coleta Informações
    coletarInfoGeral(&data, &modalidade, nome_game);

    // 2. Coleta Pontos
    coletarDesempenho(casas);

    // 3. Aplica Bônus
    aplicarBonus(casas, modalidade);

    // 4. Ordena e Exibe
    ordenarPlacar(casas);
    exibirResultados(casas, data, modalidade, nome_game);

    // 5. Salva no Arquivo
    salvarHistorico(casas, data, modalidade, nome_game);

    free(casas);
    printf("\nPressione Enter para sair...");
    getchar();

    return 0;
}

// --- IMPLEMENTAÇÃO ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializarCasas(Casa *casas) {
    strcpy(casas[0].nome, "GRIFINORIA"); casas[0].inicial = 'G';
    strcpy(casas[1].nome, "LUFA-LUFA");  casas[1].inicial = 'L';
    strcpy(casas[2].nome, "SONSERINA");  casas[2].inicial = 'S';
    strcpy(casas[3].nome, "CORVINAL");    casas[3].inicial = 'C';
    for(int i=0; i<4; i++) casas[i].pontuacao = 0;
}

void coletarInfoGeral(float *data, char *modalidade, char *nome_game) {
    printf("--- CALCULADORA DE PONTOS (GAMES E RAIOS) ---\n");

    printf("Data (ex: 17.11): ");
    scanf("%f", data);
    limparBuffer();

    printf("Qual a modalidade? [R]aio ou [G]ame: ");
    scanf("%c", modalidade);
    limparBuffer();

    *modalidade = toupper(*modalidade);

    if (*modalidade == 'G') {
        printf("Game: ");
        scanf("%[^\n]", nome_game);
        limparBuffer();
    } else {
        strcpy(nome_game, "Evento de Raio");
    }
}

void coletarDesempenho(Casa *casas) {
    int p1, p2, p3, p4, kibes, paralelas, extra;
    int pontos_calc;

    for (int i = 0; i < 4; i++) {
        printf("\n--- Inserindo dados de desempenho para %s ---\n", casas[i].nome);

        printf("Acertos em 1o (x75): "); scanf("%d", &p1);
        printf("Acertos em 2o (x70): "); scanf("%d", &p2);
        printf("Acertos em 3o (x65): "); scanf("%d", &p3);
        printf("Acertos em 4o (x60): "); scanf("%d", &p4);

        printf("Quantidade de kibes (-50): ");
        scanf("%d", &kibes);

        printf("Quantidade de paralelas (-5): ");
        scanf("%d", &paralelas);

        printf("Pontos extra: ");
        scanf("%d", &extra);

        pontos_calc = (75 * p1) + (70 * p2) + (65 * p3) + (60 * p4);
        pontos_calc -= (kibes * 50);
        pontos_calc -= (paralelas * 5);
        pontos_calc += extra;

        casas[i].pontuacao = pontos_calc;
        limparBuffer();
    }
}

void aplicarBonus(Casa *casas, char modalidade) {
    printf("\n--- BONUS DE APLICACAO ---\n");
    int idx1, idx2;

    if (modalidade == 'R') {
        int qtd_raios;
        printf("Quantidade de raios aplicados: ");
        scanf("%d", &qtd_raios);
        limparBuffer();

        printf("Qual casa aplicou o Raio?\n");
        for(int i=0; i<4; i++) printf("%d - %s\n", i+1, casas[i].nome);
        printf("Escolha: ");
        scanf("%d", &idx1); idx1--;
        limparBuffer();

        if (idx1 >= 0 && idx1 < 4) {
            int bonus = qtd_raios * 5;
            casas[idx1].pontuacao += bonus;
            printf("Bonus de %d pontos para %s!\n", bonus, casas[idx1].nome);
        }

    } else {
        char opcao;
        printf("Houve dois aplicadores? [S/N]: ");
        scanf("%c", &opcao);
        limparBuffer();

        if (toupper(opcao) == 'S') {
            printf("Primeira casa aplicadora:\n");
            for(int i=0; i<4; i++) printf("%d - %s\n", i+1, casas[i].nome);
            printf("Escolha: ");
            scanf("%d", &idx1); idx1--;
            limparBuffer();

            printf("Segunda casa aplicadora:\n");
            for(int i=0; i<4; i++) if(i!=idx1) printf("%d - %s\n", i+1, casas[i].nome);
            printf("Escolha: ");
            scanf("%d", &idx2); idx2--;
            limparBuffer();

            if (idx1 >= 0 && idx1 < 4 && idx2 >= 0 && idx2 < 4) {
                casas[idx1].pontuacao += 25;
                casas[idx2].pontuacao += 25;
                printf("Bonus de 25 pontos para %s e %s!\n", casas[idx1].nome, casas[idx2].nome);
            }
        } else {
            printf("Qual a casa aplicadora (unica)?\n");
            for(int i=0; i<4; i++) printf("%d - %s\n", i+1, casas[i].nome);
            printf("Escolha: ");
            scanf("%d", &idx1); idx1--;
            limparBuffer();

            if (idx1 >= 0 && idx1 < 4) {
                casas[idx1].pontuacao += 50;
                printf("Bonus de 50 pontos para %s!\n", casas[idx1].nome);
            }
        }
    }
}
void ordenarPlacar(Casa *casas) {
    Casa temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3 - i; j++) {
            if (casas[j].pontuacao < casas[j+1].pontuacao) {
                temp = casas[j];
                casas[j] = casas[j+1];
                casas[j+1] = temp;
            }
        }
    }
}

void exibirResultados(Casa *casas, float data, char modalidade, char *nome_game) {
    printf("\n\n----------------------------------\n");
    printf("        RESULTADO FINAL\n");
    printf("----------------------------------\n");
    printf("Data: %.2f\n", data);

    if (modalidade == 'G') {
        printf("Game: %s\n", nome_game);
    } else {
        printf("Modalidade: RAIO\n");
    }
    printf("----------------------------------\n");

    char *posicoes[] = {"1o", "2o", "3o", "4o"};
    for (int i = 0; i < 4; i++) {
        printf("%s %c: %d pontos\n", posicoes[i], casas[i].inicial, casas[i].pontuacao);
    }
    printf("----------------------------------\n");
}

void salvarHistorico(Casa *casas, float data, char modalidade, char *nome_game) {
    printf("\nTentando salvar em 'historico_games_raios.txt'...\n");

    // Modo "a" cria o arquivo se ele não existir
    FILE *arq = fopen("historico_games_raios.txt", "a");

    if (arq == NULL) {
        printf("ERRO CRITICO AO CRIAR ARQUIVO!\n");
        // O comando perror imprime o motivo exato do erro (ex: Permissão negada)
        perror("Detalhe do erro");
        printf("Dica: Tente executar o programa como Administrador ou mover para outra pasta.\n");
        return;
    }

    fprintf(arq, "\n==================================\n");
    fprintf(arq, "DATA: %.2f\n", data);

    if (modalidade == 'G') {
        fprintf(arq, "GAME: %s\n", nome_game);
    } else {
        fprintf(arq, "MODALIDADE: RAIO\n");
    }

    fprintf(arq, "----------------------------------\n");

    char *posicoes[] = {"1o", "2o", "3o", "4o"};
    for (int i = 0; i < 4; i++) {
        fprintf(arq, "%s %s: %d\n", posicoes[i], casas[i].nome, casas[i].pontuacao);
    }
    fprintf(arq, "==================================\n");

    fclose(arq);
    printf("SUCESSO! O arquivo 'historico_games_raios.txt' foi atualizado.\n");
    printf("Procure na mesma pasta onde esta este executavel.\n");
}
