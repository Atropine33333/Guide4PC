// 参考实现：engineering-config-merge
#include <cctype>
#include <iostream>
#include <map>
#include <string>

namespace {

std::string trim(const std::string& text) {
    std::size_t begin = 0;
    std::size_t end = text.size();
    while (begin < end && std::isspace(static_cast<unsigned char>(text[begin]))) ++begin;
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) --end;
    return text.substr(begin, end - begin);
}

}  // namespace

int main() {
    std::map<std::string, std::string> config;
    std::string line;
    while (std::getline(std::cin, line)) {
        const std::string text = trim(line);
        if (text.empty() || text[0] == '#') continue;
        const std::size_t equals = text.find('=');
        if (equals == std::string::npos) continue;
        const std::string key = trim(text.substr(0, equals));
        const std::string value = trim(text.substr(equals + 1));
        if (key.empty()) continue;
        config[key] = value;
    }

    for (const auto& entry : config) {
        std::cout << entry.first << '=' << entry.second << '\n';
    }
    return 0;
}
