// Reference solution: stdlib-set-window
#include <cstddef>
#include <iterator>
#include <set>
#include <vector>

std::vector<int> window_values(const std::set<int>& values, int low, int high) {
    std::vector<int> result;
    if (low > high) return result;
    const std::set<int>::const_iterator first = values.lower_bound(low);
    const std::set<int>::const_iterator last = values.upper_bound(high);
    for (std::set<int>::const_iterator it = first; it != last; ++it) {
        result.push_back(*it);
    }
    return result;
}

std::size_t window_count(const std::set<int>& values, int low, int high) {
    if (low > high) return 0;
    return static_cast<std::size_t>(
        std::distance(values.lower_bound(low), values.upper_bound(high)));
}
