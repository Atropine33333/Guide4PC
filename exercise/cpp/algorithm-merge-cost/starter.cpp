#include <cstddef>
#include <vector>

long long min_merge_cost(const std::vector<int>& piles) {
    // TODO: 这里只是把每堆石头在每次合并中都算了一遍，结果偏大。
    long long total = 0;
    for (int pile : piles) total += pile;
    return total * static_cast<long long>(piles.size() > 0 ? piles.size() - 1 : 0);
}
