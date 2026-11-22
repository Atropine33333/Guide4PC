// 参考实现：memory-null-pointer
long long sum_or_zero(const int* data, int count) {
    if (data == nullptr || count <= 0) return 0;
    long long total = 0;
    for (int i = 0; i < count; ++i) total += data[i];
    return total;
}

int max_or(const int* data, int count, int fallback) {
    if (data == nullptr || count <= 0) return fallback;
    int best = data[0];
    for (int i = 1; i < count; ++i) {
        if (data[i] > best) best = data[i];
    }
    return best;
}

int count_even(const int* data, int count) {
    if (data == nullptr || count <= 0) return 0;
    int even = 0;
    for (int i = 0; i < count; ++i) {
        if (data[i] % 2 == 0) ++even;
    }
    return even;
}
