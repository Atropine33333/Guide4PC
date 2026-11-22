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
        // TODO: 统计每个单词的出现次数。
    }

    std::string best;
    int bestCount = 0;
    // TODO: 遍历 counts，选出次数最多且字典序最小的单词。

    std::cout << best << '\n';
    return 0;
}
