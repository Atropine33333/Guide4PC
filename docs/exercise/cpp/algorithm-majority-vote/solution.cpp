#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    std::cin >> n;
    std::vector<int> values(n);
    for (int& value : values) {
        std::cin >> value;
    }

    int candidate = 0;
    int balance = 0;
    for (int value : values) {
        if (balance == 0) {
            candidate = value;
            balance = 1;
        } else if (value == candidate) {
            ++balance;
        } else {
            --balance;
        }
    }

    long long count = 0;
    for (int value : values) {
        if (value == candidate) {
            ++count;
        }
    }

    if (count * 2 > n) {
        std::cout << candidate << '\n';
    } else {
        std::cout << "none\n";
    }
    return 0;
}
