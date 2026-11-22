// 参考实现：debugging-stable-merge
#include <iostream>
#include <vector>

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<long long> a(static_cast<std::size_t>(n));
    std::vector<long long> b(static_cast<std::size_t>(m));
    for (long long& value : a) std::cin >> value;
    for (long long& value : b) std::cin >> value;

    std::vector<long long> merged;
    merged.reserve(a.size() + b.size());

    std::size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] <= b[j]) {
            merged.push_back(a[i++]);
        } else {
            merged.push_back(b[j++]);
        }
    }
    while (i < a.size()) merged.push_back(a[i++]);
    while (j < b.size()) merged.push_back(b[j++]);

    for (std::size_t k = 0; k < merged.size(); ++k) {
        if (k > 0) std::cout << ' ';
        std::cout << merged[k];
    }
    std::cout << '\n';
    return 0;
}
