// Reference solution: debugging-window-average
#include <cstddef>
#include <vector>

std::vector<double> window_average(const std::vector<int>& values,
                                   std::size_t window) {
    if (window == 0 || window > values.size()) return {};

    std::vector<double> result;
    long long sum = 0;
    for (std::size_t i = 0; i < window; ++i) sum += values[i];
    result.push_back(static_cast<double>(sum) / static_cast<double>(window));

    for (std::size_t i = window; i < values.size(); ++i) {
        sum -= values[i - window];
        sum += values[i];
        result.push_back(static_cast<double>(sum) /
                         static_cast<double>(window));
    }
    return result;
}
