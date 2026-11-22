#include <vector>

struct CycleInfo {
    bool has_cycle;
    int entry;
    int length;
};

CycleInfo analyze(const std::vector<int>& next, int start) {
    // TODO: 沿 next 前进并记录访问步数，找出环入口和环长。
    (void)next;
    (void)start;
    return CycleInfo{false, -1, 0};
}
