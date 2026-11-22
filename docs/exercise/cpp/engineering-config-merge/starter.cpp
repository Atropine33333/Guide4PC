#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::pair<std::string, std::string>> config;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty() || line[0] == '#') continue;
        const std::size_t equals = line.find('=');
        if (equals == std::string::npos) continue;
        const std::string key = line.substr(0, equals);
        const std::string value = line.substr(equals + 1);
        // TODO: 去掉两侧空白，并让后出现的值覆盖先出现的值。
        config.emplace_back(key, value);
    }

    // TODO: 按关键字排序，而不是按输入顺序输出。
    for (const auto& entry : config) {
        std::cout << entry.first << '=' << entry.second << '\n';
    }
    return 0;
}
