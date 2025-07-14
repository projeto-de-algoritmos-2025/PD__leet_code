#include <stdlib.h> // Para malloc e free
#include <string.h> // Para memset
#include <stdio.h>  // Para printf (apenas para testes, pode ser removido)

// Função auxiliar para encontrar o mínimo entre dois inteiros
int min(int a, int b) {
    return a < b ? a : b;
}

int profitableSchemes(int n, int minProfit, int* group, int groupSize, int* profit, int profitSize) {
    // MOD constante para operações de módulo
    const int MOD = 1e9 + 7;

    // dp[j][k] armazena o número de maneiras de conseguir 'j' membros
    // e 'k' de lucro.
    // 'j' vai de 0 a n (total de membros).
    // 'k' vai de 0 a minProfit (lucro, limitado a minProfit).
    // Qualquer lucro que exceda minProfit é tratado como minProfit.
    int** dp = (int**)malloc(sizeof(int*) * (n + 1));
    if (dp == NULL) {
        return 0; // Falha na alocação de memória
    }

    for (int i = 0; i <= n; ++i) {
        dp[i] = (int*)malloc(sizeof(int) * (minProfit + 1));
        if (dp[i] == NULL) {
            // Limpa a memória alocada anteriormente em caso de falha
            for (int k = 0; k < i; ++k) {
                free(dp[k]);
            }
            free(dp);
            return 0;
        }
        // Inicializa todos os estados dp para 0
        memset(dp[i], 0, sizeof(int) * (minProfit + 1));
    }

    // Caso base: Há uma maneira de conseguir 0 membros e 0 lucro (não escolhendo nenhum crime).
    dp[0][0] = 1;

    // Itera através de cada crime
    for (int i = 0; i < groupSize; ++i) {
        int current_group_members = group[i];
        int current_profit_val = profit[i];

        // Itera através dos membros (de n para baixo até current_group_members)
        // Isso garante que cada crime seja considerado apenas uma vez (propriedade do knapsack 0/1)
        for (int j = n; j >= current_group_members; --j) {
            // Itera através do lucro (de minProfit para baixo até 0)
            // 'k' aqui representa o lucro *antes* de adicionar o crime atual
            for (int k = minProfit; k >= 0; --k) {
                // Calcula o novo lucro se adicionarmos o crime atual
                // Limita o novo lucro em minProfit
                int new_profit_capped = k + current_profit_val;
                if (new_profit_capped > minProfit) {
                    new_profit_capped = minProfit;
                }

                // Adiciona o número de maneiras do estado anterior (antes de adicionar o crime atual)
                // ao estado atual.
                // dp[j - current_group_members][k] representa as maneiras de obter
                // 'j - current_group_members' membros e 'k' lucro usando crimes anteriores.
                dp[j][new_profit_capped] = (dp[j][new_profit_capped] + dp[j - current_group_members][k]) % MOD;
            }
        }
    }

    // Calcula o número total de esquemas lucrativos
    // Um esquema é lucrativo se seu lucro for pelo menos minProfit.
    // Devido à limitação, todos os esquemas com lucro >= minProfit são acumulados em dp[j][minProfit].
    int total_profitable_schemes = 0;
    for (int j = 0; j <= n; ++j) {
        total_profitable_schemes = (total_profitable_schemes + dp[j][minProfit]) % MOD;
    }

    // Libera a memória alocada
    for (int i = 0; i <= n; ++i) {
        free(dp[i]);
    }
    free(dp);

    return total_profitable_schemes;
}
