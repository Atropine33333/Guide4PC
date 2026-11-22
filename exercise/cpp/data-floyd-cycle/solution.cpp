#include <vector>

struct CycleInfo {
    bool has_cycle;
    int entry;
    int length;
};

CycleInfo analyze(const std::vector<int>& next, int start) {
    const int n = static_cast<int>(next.size());
    if (start < 0 || start >= n) {
        return {false, -1, 0};
    }

    std::vector<int> first_seen(static_cast<std::size_t>(n), -1);
    int node = start;
    int step = 0;
    while (node != -1) {
        if (first_seen[static_cast<std::size_t>(node)] != -1) {
            return {true, node,
                    step - first_seen[static_cast<std::size_t>(node)]};
        }
        first_seen[static_cast<std::size_t>(node)] = step;
        node = next[static_cast<std::size_t>(node)];
        if (node < -1 || node >= n) {
            break;
        }
        ++step;
    }
    return {false, -1, 0};
}
