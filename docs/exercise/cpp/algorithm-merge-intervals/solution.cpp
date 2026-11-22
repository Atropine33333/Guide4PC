// 参考实现：algorithm-merge-intervals
#include <algorithm>
#include <vector>

struct Interval {
    int left;
    int right;
};

std::vector<Interval> merge_intervals(std::vector<Interval> intervals) {
    if (intervals.empty()) return {};

    std::sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
        if (a.left != b.left) return a.left < b.left;
        return a.right < b.right;
    });

    std::vector<Interval> merged;
    merged.push_back(intervals.front());
    for (std::size_t i = 1; i < intervals.size(); ++i) {
        if (intervals[i].left <= merged.back().right) {
            if (intervals[i].right > merged.back().right) {
                merged.back().right = intervals[i].right;
            }
        } else {
            merged.push_back(intervals[i]);
        }
    }
    return merged;
}
