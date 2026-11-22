#include <iomanip>
#include <iostream>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::string name;
        long long score = 0;
        std::cin >> name >> score;
        // TODO: 名字左对齐宽度 12，竖线，分数右对齐宽度 8。
        std::cout << name << '|' << score << '\n';
    }
    return 0;
}
