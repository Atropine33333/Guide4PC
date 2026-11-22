// 参考实现：algorithm-max-subarray
#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    if (!(std::cin >> n) || n <= 0) return 0;

    const long long kNegInf = -4000000000000000000LL;

    long long first = 0;
    std::cin >> first;

    long long keep = first;          // 结尾在当前位置、没有删过元素的最大和
    long long dropped = kNegInf;     // 结尾在当前位置、已经删过一个元素的最大和
    long long best = first;

    for (int i = 1; i < n; ++i) {
        long long value = 0;
        std::cin >> value;

        const long long next_dropped =
            (keep > dropped + value) ? keep : dropped + value;
        const long long next_keep =
            (keep + value > value) ? keep + value : value;

        keep = next_keep;
        dropped = next_dropped;
        if (keep > best) best = keep;
        if (dropped > best) best = dropped;
    }

    std::cout << best << '\n';
    return 0;
}
