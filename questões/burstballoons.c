#include <stdlib.h> // Para malloc e free
#include <string.h> // Para memset
#include <stdio.h>  // Para printf (apenas para testes)

// Função auxiliar para encontrar o máximo entre dois inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

int maxCoins(int* nums, int numsSize) {
    // 1. Adicionar 1s nas bordas do array original
    // Isso simplifica o tratamento dos casos de limite (i-1 ou i+1 fora dos bounds)
    int n_ext = numsSize + 2;
    int* nums_ext = (int*)malloc(sizeof(int) * n_ext);
    if (nums_ext == NULL) {
        // Lidar com erro de alocação de memória
        return 0; 
    }

    nums_ext[0] = 1;
    for (int i = 0; i < numsSize; ++i) {
        nums_ext[i + 1] = nums[i];
    }
    nums_ext[n_ext - 1] = 1;

    // 2. Criar a tabela DP
    // dp[i][j] representa a pontuação máxima de estourar os balões
    // entre os índices i e j (exclusivos) do nums_ext.
    // Ou seja, os balões a serem estourados são de nums_ext[i+1] até nums_ext[j-1].
    // Os balões nums_ext[i] e nums_ext[j] são os "pivôs" que permanecem.
    int** dp = (int**)malloc(sizeof(int*) * n_ext);
    if (dp == NULL) {
        free(nums_ext);
        return 0;
    }
    for (int i = 0; i < n_ext; ++i) {
        dp[i] = (int*)malloc(sizeof(int) * n_ext);
        if (dp[i] == NULL) {
            // Lidar com erro de alocação de memória e liberar o que já foi alocado
            for (int k = 0; k < i; ++k) free(dp[k]);
            free(dp);
            free(nums_ext);
            return 0;
        }
        // Inicializa a linha com zeros
        memset(dp[i], 0, sizeof(int) * n_ext);
    }

    // 3. Preencher a tabela DP
    // 'len' é o comprimento do subarray que estamos considerando,
    // variando de 1 até numsSize (o número original de balões).
    // Note que i e j aqui se referem aos índices *extremos* do segmento,
    // ou seja, os balões nums_ext[i] e nums_ext[j] são os que NÃO são estourados ainda.
    // O segmento a ser estourado é (i+1) a (j-1).
    for (int len = 1; len <= numsSize; ++len) { // Comprimento dos balões a estourar
        for (int i = 0; i <= n_ext - len - 2; ++i) { // Início do segmento (pivô esquerdo)
            int j = i + len + 1; // Fim do segmento (pivô direito)

            // Iterar sobre todos os balões 'k' que podem ser o último a estourar
            // dentro do subarray (i+1) ... (j-1)
            for (int k = i + 1; k < j; ++k) {
                // dp[i][k] é a pontuação de estourar (i+1)...(k-1)
                // dp[k][j] é a pontuação de estourar (k+1)...(j-1)
                // nums_ext[i] * nums_ext[k] * nums_ext[j] é a pontuação de estourar 'k' por último
                // (quando i e j são os vizinhos restantes)
                int coins = nums_ext[i] * nums_ext[k] * nums_ext[j];
                coins += dp[i][k] + dp[k][j];
                dp[i][j] = max(dp[i][j], coins);
            }
        }
    }

    // O resultado final está em dp[0][n_ext-1], que representa a pontuação
    // máxima de estourar todos os balões entre os 1s adicionados.
    int result = dp[0][n_ext - 1];

    // 4. Liberar a memória alocada
    for (int i = 0; i < n_ext; ++i) {
        free(dp[i]);
    }
    free(dp);
    free(nums_ext);

    return result;
}