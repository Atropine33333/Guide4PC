#include <cstddef>
#include <vector>

std::vector<long long> sliding_max(const std::vector<long long>& values,
                                   std::size_t window) {
    std::vector<long long> result;
    if (window == 0 || window > values.size()) return result;

    // TODO: this only reports the maximum of the first window.
    long long best = values[0];
    for (std::size_t i = 1; i < window; ++i) {
        if (values[i] > best) best = values[i];
    }
    result.push_back(best);
    return result;
}
