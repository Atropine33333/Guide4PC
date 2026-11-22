// 参考实现：algorithm-meeting-count
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    if (!(std::cin >> n)) return 0;
    std::vector<std::pair<long long, long long>> meetings;
    meetings.reserve(n);
    for (int i = 0; i < n; ++i) {
        long long start = 0;
        long long end = 0;
        std::cin >> start >> end;
        meetings.emplace_back(start, end);
    }

    std::sort(meetings.begin(), meetings.end(),
              [](const std::pair<long long, long long>& a,
                 const std::pair<long long, long long>& b) {
                  if (a.second != b.second) return a.second < b.second;
                  return a.first < b.first;
              });

    int count = 0;
    long long last_end = 0;
    bool started = false;
    for (const auto& m : meetings) {
        if (!started || m.first >= last_end) {
            ++count;
            last_end = m.second;
            started = true;
        }
    }
    std::cout << count << '\n';
    return 0;
}
