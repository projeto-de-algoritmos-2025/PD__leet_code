class Solution:
    def rob(self, nums):
        n = len(nums)
        
        # Se não há casas, não há o que roubar
        if n == 0:
            return 0
        
        # Se só há uma casa, o máximo é o valor dela
        if n == 1:
            return nums[0]
        
        # Cria um array dp onde dp[i] representa o máximo que pode ser roubado até a casa i
        dp = [0] * n

        # Caso base: só há uma casa, rouba ela
        dp[0] = nums[0]
        
        # Caso base: entre a casa 0 e 1, escolhe a de maior valor
        dp[1] = max(nums[0], nums[1])
        
        # Itera do índice 2 até o final
        for i in range(2, n):
            # Escolhe o maior entre:
            # - não roubar a casa i (fica com dp[i-1])
            # - roubar a casa i (soma nums[i] com dp[i-2], já que i-1 não pode ser roubada)
            dp[i] = max(dp[i-1], nums[i] + dp[i-2])
        
        # O último elemento de dp guarda o valor máximo que pode ser roubado
        return dp[-1]
