class Solution {
    public int maxProfit(int k, int[] prices) {
        int n = prices.length;
        if (n== 0)
            return 0;
        int[][] dp = new int[k+1][n];
        
        for (int i =0; i<=k; i++) {
            for(int j=0; j<n; j++) {
                dp[i][j] = 0;
            }
        }
   
        // if no stock, no profit;
        for (int i=0; i<n; i++) {
            dp[0][i] = 0;
        }
        // if no days , then no need to buy
        for (int i=0; i <= k; i++) {
            dp[i][0] = 0;
        }
        for ( int i=1; i <=k ; i++) {
            int bestBuy = 0 - prices[0]; // losing money so negative!!
            for ( int j=1; j <n; j++) {
                int DontbuyToday = dp[i][j-1]; 
                int buyTodayAndProfit = bestBuy /* previous buy and sell transaction + best buy */ +
                                        prices[j] /* buy today */;
                dp[i][j] = Math.max(DontbuyToday, buyTodayAndProfit);
                if (bestBuy < dp[i-1][j-1] - prices[j]) // profit i got in previous transaction (if done) -  current selling price if greater than the best cost price so far, then that was the current bestbuy for next transaction 
                    bestBuy = dp[i-1][j-1] - prices[j];                    
                
            }            
        }
        return dp[k][n-1];
    }
}
