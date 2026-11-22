// 参考实现：syntax-grade-band
#include <iostream>

int main() {
    int score = 0;
    std::cin >> score;

    char grade = 'F';
    if (score >= 90) {
        grade = 'A';
    } else if (score >= 80) {
        grade = 'B';
    } else if (score >= 70) {
        grade = 'C';
    } else if (score >= 60) {
        grade = 'D';
    }

    std::cout << grade << '\n';
    return 0;
}
