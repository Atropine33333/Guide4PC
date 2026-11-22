#include <iostream>
#include <unordered_set>

int main() {
    long long n = 0;
    std::cin >> n;

    std::unordered_set<long long> seen;
    int steps = 0;
    while (n != 1 && seen.find(n) == seen.end()) {
        seen.insert(n);
        long long next = 0;
        while (n > 0) {
            const long long digit = n % 10;
            next += digit * digit;
            n /= 10;
        }
        n = next;
        ++steps;
    }

    std::cout << (n == 1 ? steps : -1) << '\n';
    return 0;
}
