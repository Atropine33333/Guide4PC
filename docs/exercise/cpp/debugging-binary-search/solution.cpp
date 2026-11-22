// 参考实现：debugging-binary-search
#include <vector>

int lower_bound_index(const std::vector<int>& values, int target) {
    int low = 0;
    int high = static_cast<int>(values.size());
    while (low < high) {
        const int mid = low + (high - low) / 2;
        if (values[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
