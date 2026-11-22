// Reference solution: data-sliding-max
#include <cstddef>
#include <deque>
#include <vector>

std::vector<long long> sliding_max(const std::vector<long long>& values,
                                   std::size_t window) {
    std::vector<long long> result;
    if (window == 0 || window > values.size()) return result;

    std::deque<std::size_t> indices;
    for (std::size_t i = 0; i < values.size(); ++i) {
        while (!indices.empty() && indices.front() + window <= i) {
            indices.pop_front();
        }
        while (!indices.empty() && values[indices.back()] <= values[i]) {
            indices.pop_back();
        }
        indices.push_back(i);
        if (i + 1 >= window) result.push_back(values[indices.front()]);
    }
    return result;
}
