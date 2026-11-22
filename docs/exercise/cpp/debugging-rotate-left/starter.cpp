#include <cstddef>
#include <vector>

void rotate_left(std::vector<int>& values, int k) {
    const std::size_t n = values.size();
    if (n == 0) return;
    const std::size_t shift = static_cast<std::size_t>(k) % n;
    std::vector<int> rotated(values.begin() + static_cast<std::ptrdiff_t>(shift), values.end());
    rotated.insert(rotated.end(), values.begin(),
                   values.begin() + static_cast<std::ptrdiff_t>(shift));
    values = std::move(rotated);
}
