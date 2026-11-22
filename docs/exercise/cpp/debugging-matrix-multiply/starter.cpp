#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int p = 0;
    int q = 0;
    std::cin >> n >> p >> q;

    std::vector<std::vector<int>> a(n, std::vector<int>(p));
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < p; ++k) {
            std::cin >> a[i][k];
        }
    }
    std::vector<std::vector<int>> b(p, std::vector<int>(q));
    for (int k = 0; k < p; ++k) {
        for (int j = 0; j < q; ++j) {
            std::cin >> b[k][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < q; ++j) {
            int sum = 0;
            for (int k = 0; k < p; ++k) {
                // BUG: B 的行列下标写反了。
                sum += a[i][k] * b[j][k];
            }
            if (j != 0) {
                std::cout << ' ';
            }
            std::cout << sum;
        }
        std::cout << '\n';
    }
    return 0;
}
