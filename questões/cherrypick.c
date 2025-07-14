#include <stdlib.h> // Para malloc e free
#include <string.h> // Para memset (embora o loop de inicialização seja usado)
#include <limits.h> // Para INT_MIN (se usasse um valor muito baixo em vez de -1 para unreachable)

// Função auxiliar para encontrar o máximo entre dois inteiros
int max(int a, int b) {
    return a > b ? a : b;
}

int cherryPickup(int** grid, int gridSize, int* gridColSize) {
    int n = gridSize;
    
    // dp[k][r1][r2]: máximo de cerejas coletadas quando ambos os robôs deram 'k' passos,
    // o Robô 1 está em (r1, k - r1) e o Robô 2 está em (r2, k - r2).
    // O número total de passos para alcançar (n-1, n-1) é (n-1) + (n-1) = 2*n - 2.
    // 'k' varia de 0 a 2*n - 2. A dimensão para 'k' é 2*n - 1.
    // 'r1' e 'r2' variam de 0 a n-1. A dimensão para 'r1' e 'r2' é n.
    int*** dp = (int***)malloc(sizeof(int**) * (2 * n - 1));
    if (dp == NULL) {
        return 0; // Falha na alocação de memória
    }

    // Aloca memória para a tabela DP 3D e inicializa com -1 (inalcançável)
    for (int k = 0; k < 2 * n - 1; ++k) {
        dp[k] = (int**)malloc(sizeof(int*) * n);
        if (dp[k] == NULL) {
            // Libera memória alocada anteriormente em caso de falha
            for (int i = 0; i < k; ++i) {
                for (int j = 0; j < n; ++j) {
                    free(dp[i][j]);
                }
                free(dp[i]);
            }
            free(dp);
            return 0; 
        }
        for (int r1 = 0; r1 < n; ++r1) {
            dp[k][r1] = (int*)malloc(sizeof(int) * n);
            if (dp[k][r1] == NULL) {
                // Libera memória alocada anteriormente em caso de falha
                for (int j = 0; j < r1; ++j) {
                    free(dp[k][j]);
                }
                free(dp[k]);
                for (int i = 0; i < k; ++i) {
                    for (int j = 0; j < n; ++j) {
                        free(dp[i][j]);
                    }
                    free(dp[i]);
                }
                free(dp);
                return 0;
            }
            // Inicializa todos os estados como -1, indicando que são inalcançáveis
            for (int r2 = 0; r2 < n; ++r2) {
                dp[k][r1][r2] = -1;
            }
        }
    }

    // Caso base: Ambos os robôs iniciam em (0,0) na etapa 0.
    // Conforme as restrições do problema, grid[0][0] nunca será -1.
    dp[0][0][0] = grid[0][0];

    // Itera através do número total de passos 'k'
    for (int k = 1; k < 2 * n - 1; ++k) {
        // Itera através de todas as posições de linha possíveis para o Robô 1 (r1)
        for (int r1 = 0; r1 < n; ++r1) {
            // Deriva a posição da coluna para o Robô 1 (c1)
            int c1 = k - r1;

            // Verifica se (r1, c1) é uma célula válida dentro da grade e não um espinho (-1)
            if (c1 < 0 || c1 >= n || grid[r1][c1] == -1) {
                continue; // O caminho do Robô 1 é inválido, pula para o próximo r1
            }

            // Itera através de todas as posições de linha possíveis para o Robô 2 (r2)
            for (int r2 = 0; r2 < n; ++r2) {
                // Deriva a posição da coluna para o Robô 2 (c2)
                int c2 = k - r2;

                // Verifica se (r2, c2) é uma célula válida dentro da grade e não um espinho (-1)
                if (c2 < 0 || c2 >= n || grid[r2][c2] == -1) {
                    continue; // O caminho do Robô 2 é inválido, pula para o próximo r2
                }

                // Neste ponto, (r1, c1) e (r2, c2) são posições válidas para a etapa k.
                // Agora, calculamos o máximo de cerejas das etapas anteriores (k-1)
                int prev_max_cherries = -1; // Inicializa com -1 para indicar nenhum caminho anterior válido

                // Considera todas as 4 combinações possíveis de movimentos da etapa k-1 para k:
                // Robô 1 (r1,c1) de (r1-1,c1) ou (r1,c1-1)
                // Robô 2 (r2,c2) de (r2-1,c2) ou (r2,c2-1)

                // 1. Ambos os robôs moveram para baixo: Robô 1 de (r1-1, c1), Robô 2 de (r2-1, c2)
                if (r1 > 0 && r2 > 0 && dp[k-1][r1-1][r2-1] != -1) {
                    prev_max_cherries = max(prev_max_cherries, dp[k-1][r1-1][r2-1]);
                }
                // 2. Robô 1 moveu para baixo, Robô 2 moveu para a direita: Robô 1 de (r1-1, c1), Robô 2 de (r2, c2-1)
                if (r1 > 0 && c2 > 0 && dp[k-1][r1-1][r2] != -1) { 
                    prev_max_cherries = max(prev_max_cherries, dp[k-1][r1-1][r2]);
                }
                // 3. Robô 1 moveu para a direita, Robô 2 moveu para baixo: Robô 1 de (r1, c1-1), Robô 2 de (r2-1, c2)
                if (c1 > 0 && r2 > 0 && dp[k-1][r1][r2-1] != -1) {
                    prev_max_cherries = max(prev_max_cherries, dp[k-1][r1][r2-1]);
                }
                // 4. Ambos os robôs moveram para a direita: Robô 1 de (r1, c1-1), Robô 2 de (r2, c2-1)
                if (c1 > 0 && c2 > 0 && dp[k-1][r1][r2] != -1) {
                    prev_max_cherries = max(prev_max_cherries, dp[k-1][r1][r2]);
                }

                // Se nenhum estado anterior válido foi encontrado, então o estado atual (k, r1, r2) é inalcançável
                if (prev_max_cherries == -1) {
                    continue; // Pula para o próximo estado
                }

                // Adiciona as cerejas das células atuais
                int current_path_cherries = grid[r1][c1];
                // Se os robôs não estão na mesma célula, adiciona as cerejas do Robô 2 também
                if (r1 != r2 || c1 != c2) { 
                    current_path_cherries += grid[r2][c2];
                }
                dp[k][r1][r2] = prev_max_cherries + current_path_cherries;
            }
        }
    }

    // A resposta final é dp[2*n-2][n-1][n-1], que representa ambos os robôs
    // alcançando (n-1, n-1) após 2*n-2 passos.
    // Se esse valor for -1, significa que nenhum caminho válido foi encontrado.
    int result = dp[2 * n - 2][n - 1][n - 1];
    if (result == -1) {
        result = 0; // Conforme o problema, se não houver caminho válido, retorna 0
    }

    // Libera a memória alocada para a tabela DP
    for (int k = 0; k < 2 * n - 1; ++k) {
        for (int r1 = 0; r1 < n; ++r1) {
            free(dp[k][r1]);
        }
        free(dp[k]);
    }
    free(dp);

    return result;
}