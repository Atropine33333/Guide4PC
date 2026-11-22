#include <iostream>
#include <string>
#include <vector>

std::string multiply(const std::string& a, const std::string& b) {
    if (a == "0" || b == "0") return "0";
    std::vector<int> digits(a.size() + b.size(), 0);
    for (int i = static_cast<int>(a.size()) - 1; i >= 0; --i) {
        for (int j = static_cast<int>(b.size()) - 1; j >= 0; --j) {
            digits[i + j + 1] =
                (digits[i + j + 1] + (a[i] - '0') * (b[j] - '0')) % 10;
        }
    }
    std::string out;
    for (int d : digits) {
        if (out.empty() && d == 0) continue;
        out.push_back(static_cast<char>('0' + d));
    }
    return out.empty() ? "0" : out;
}

int main() {
    std::string a;
    std::string b;
    if (std::cin >> a >> b) {
        std::cout << multiply(a, b) << '\n';
    }
    return 0;
}
