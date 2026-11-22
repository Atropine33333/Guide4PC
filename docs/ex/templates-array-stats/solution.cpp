// 参考实现：templates-array-stats
template <typename T, int N>
double average(const T (&values)[N]) {
    double total = 0.0;
    for (int i = 0; i < N; ++i) total += values[i];
    return total / N;
}

template <typename T, int N>
T max_value(const T (&values)[N]) {
    T best = values[0];
    for (int i = 1; i < N; ++i) {
        if (best < values[i]) best = values[i];
    }
    return best;
}
