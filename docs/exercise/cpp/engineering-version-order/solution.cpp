// 参考实现：engineering-version-order
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::vector<long long> parse_version(const std::string& text) {
    std::vector<long long> parts;
    long long value = 0;
    for (char c : text) {
        if (c == '.') {
            parts.push_back(value);
            value = 0;
        } else {
            value = value * 10 + (c - '0');
        }
    }
    parts.push_back(value);
    return parts;
}

bool version_less(const std::vector<long long>& a,
                  const std::vector<long long>& b) {
    const std::size_t length = a.size() > b.size() ? a.size() : b.size();
    for (std::size_t i = 0; i < length; ++i) {
        const long long left = i < a.size() ? a[i] : 0;
        const long long right = i < b.size() ? b[i] : 0;
        if (left != right) return left < right;
    }
    return false;
}

}  // namespace

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<std::string> versions(n);
    std::vector<std::vector<long long>> parsed(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> versions[i];
        parsed[i] = parse_version(versions[i]);
    }

    std::vector<int> order(n);
    for (int i = 0; i < n; ++i) order[i] = i;

    std::stable_sort(order.begin(), order.end(), [&parsed](int first, int second) {
        return version_less(parsed[first], parsed[second]);
    });

    for (int index : order) std::cout << versions[index] << '\n';
    return 0;
}
