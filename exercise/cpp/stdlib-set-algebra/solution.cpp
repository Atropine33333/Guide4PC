// 参考实现：stdlib-set-algebra
#include <algorithm>
#include <iostream>
#include <vector>

namespace {

std::vector<int> make_union(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> out;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(out));
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

std::vector<int> make_intersection(const std::vector<int>& a,
                                   const std::vector<int>& b) {
    std::vector<int> out;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(out));
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

std::vector<int> make_difference(const std::vector<int>& a,
                                 const std::vector<int>& b) {
    std::vector<int> out;
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                        std::back_inserter(out));
    out.erase(std::unique(out.begin(), out.end()), out.end());
    return out;
}

void print(const std::vector<int>& values) {
    if (values.empty()) {
        std::cout << "empty\n";
        return;
    }
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    int n = 0;
    int m = 0;
    if (!(std::cin >> n >> m)) return 0;
    std::vector<int> a(n);
    std::vector<int> b(m);
    for (int i = 0; i < n; ++i) std::cin >> a[i];
    for (int i = 0; i < m; ++i) std::cin >> b[i];

    a.erase(std::unique(a.begin(), a.end()), a.end());
    b.erase(std::unique(b.begin(), b.end()), b.end());

    print(make_union(a, b));
    print(make_intersection(a, b));
    print(make_difference(a, b));
    return 0;
}
