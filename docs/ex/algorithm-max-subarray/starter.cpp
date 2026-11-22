#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    long long current = 0;
    long long best = 0;
    bool first = true;
    for (int i = 0; i < n; ++i) {
        long long value = 0;
        std::cin >> value;
        current = first ? value : (current + value > value ? current + value : value);
        if (first) {
            best = value;
            first = false;
        } else if (current > best) {
            best = current;
        }
        // TODO: 还要考虑“删掉区间里一个数”的情况，单靠这一个状态不够。
    }

    std::cout << best << '\n';
    return 0;
}
