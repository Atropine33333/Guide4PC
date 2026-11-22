// 参考实现：algorithm-lis
#include <algorithm>
#include <vector>

int lis_length(const std::vector<int>& values) {
    std::vector<int> tails;
    tails.reserve(values.size());
    for (int value : values) {
        const auto it = std::lower_bound(tails.begin(), tails.end(), value);
        if (it == tails.end()) {
            tails.push_back(value);
        } else {
            *it = value;
        }
    }
    return static_cast<int>(tails.size());
}
