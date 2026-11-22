// 参考实现：syntax-coin-greedy
long long greedy_coin_count(long long amount) {
    const long long coins[] = {200, 100, 50, 20, 10, 5, 2, 1};
    long long total = 0;
    for (long long coin : coins) {
        total += amount / coin;
        amount %= coin;
    }
    return total;
}
