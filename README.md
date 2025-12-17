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
