#include <iostream>
#include <string>

int main() {
    std::string text;
    std::cin >> text;

    // TODO: 按驼峰规则拆分，输出小写单词。
    // 下面只输出了整个串的小写形式。
    for (char& c : text) {
        if (c >= 'A' && c <= 'Z') {
            c = static_cast<char>(c - 'A' + 'a');
        }
    }
    std::cout << text << '\n';
    return 0;
}
