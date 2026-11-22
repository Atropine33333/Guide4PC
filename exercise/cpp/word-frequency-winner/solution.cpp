// 参考实现：word-frequency-winner
#include <iostream>
#include <map>
#include <string>

int main() {
    int n = 0;
    if (!(std::cin >> n)) return 0;

    std::map<std::string, int> counts;
    for (int i = 0; i < n; ++i) {
        std::string word;
        std::cin >> word;
        ++counts[word];
    }

    std::string best;
    int bestCount = 0;
    for (const auto& entry : counts) {
        if (entry.second > bestCount) {
            best = entry.first;
            bestCount = entry.second;
        }
    }

    std::cout << best << '\n';
    return 0;
}
