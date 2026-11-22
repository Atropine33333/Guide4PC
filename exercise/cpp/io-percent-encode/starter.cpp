#include <iostream>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    // TODO: 判断每个字符是否属于安全集合，不是就输出 %XX。
    std::cout << line << '\n';
    return 0;
}
