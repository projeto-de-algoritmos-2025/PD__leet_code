class Solution:
    def minDistance(self, word1, word2):
        m = len(word1)
        n = len(word2)
        
        # Cria uma matriz dp de tamanho (m+1) x (n+1)
        dp = [[0] * (n + 1) for _ in range(m + 1)]

        # Inicializa a primeira coluna: deletar todos os caracteres de word1
        for i in range(m + 1):
            dp[i][0] = i

        # Inicializa a primeira linha: inserir todos os caracteres de word2
        for j in range(n + 1):
            dp[0][j] = j

        # Preenche o resto da matriz
        for i in range(1, m + 1):
            for j in range(1, n + 1):
                if word1[i - 1] == word2[j - 1]:
                    # Se os caracteres forem iguais, nenhuma operação necessária
                    dp[i][j] = dp[i - 1][j - 1]
                else:
                    # Caso contrário, escolhe a melhor entre inserir, remover ou substituir
                    dp[i][j] = 1 + min(
                        dp[i - 1][j],    # remover
                        dp[i][j - 1],    # inserir
                        dp[i - 1][j - 1] # substituir
                    )

        # A resposta final é dp[m][n]
        return dp[m][n]
