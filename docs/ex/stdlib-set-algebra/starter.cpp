#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;
    std::vector<int> a(n);
    std::vector<int> b(m);
    for (int i = 0; i < n; ++i) std::cin >> a[i];
    for (int i = 0; i < m; ++i) std::cin >> b[i];

    // TODO: 用有序区间的集合运算（或双指针）求出并集、交集和差集。
    // 下面的代码只输出空集合。
    std::cout << "empty\nempty\nempty\n";
    return 0;
}
