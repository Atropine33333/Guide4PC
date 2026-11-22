#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<std::pair<int, int>> meetings(n);
    for (auto& m : meetings) std::cin >> m.first >> m.second;

    // TODO: 按开始时间排序并贪心选择，这并不总是最优。
    std::sort(meetings.begin(), meetings.end());
    int count = 0;
    int last_end = -1;
    for (const auto& m : meetings) {
        if (m.first >= last_end) {
            ++count;
            last_end = m.second;
        }
    }
    std::cout << count << '\n';
    return 0;
}
