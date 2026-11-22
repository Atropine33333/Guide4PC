// 参考实现：algorithm-inversions
#include <vector>

namespace {

long long merge_count(std::vector<int>& values, std::vector<int>& buffer,
                      std::size_t left, std::size_t right) {
    if (right - left <= 1) return 0;

    const std::size_t middle = left + (right - left) / 2;
    long long count = merge_count(values, buffer, left, middle) +
                      merge_count(values, buffer, middle, right);

    std::size_t i = left;
    std::size_t j = middle;
    std::size_t out = left;
    while (i < middle && j < right) {
        if (values[i] <= values[j]) {
            buffer[out++] = values[i++];
        } else {
            count += static_cast<long long>(middle - i);
            buffer[out++] = values[j++];
        }
    }
    while (i < middle) buffer[out++] = values[i++];
    while (j < right) buffer[out++] = values[j++];

    for (std::size_t k = left; k < right; ++k) values[k] = buffer[k];
    return count;
}

}  // namespace

long long count_inversions(const std::vector<int>& values) {
    std::vector<int> working = values;
    std::vector<int> buffer(working.size());
    return merge_count(working, buffer, 0, working.size());
}
