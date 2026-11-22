#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<std::string> versions(n);
    for (int i = 0; i < n; ++i) std::cin >> versions[i];

    // TODO: 解析成数值段，按版本规则稳定排序，再输出原字符串。
    std::sort(versions.begin(), versions.end());

    for (const std::string& version : versions) std::cout << version << '\n';
    return 0;
}
