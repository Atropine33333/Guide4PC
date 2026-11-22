// 参考实现：memory-heap-array
#include <cstddef>

int* make_zero_array(int count) {
    if (count <= 0) return nullptr;
    return new int[static_cast<std::size_t>(count)]();
}

void fill_sequence(int* data, int count, int start) {
    if (data == nullptr || count <= 0) return;
    for (int i = 0; i < count; ++i) data[i] = start + i;
}

long long sum_array(const int* data, int count) {
    if (data == nullptr || count <= 0) return 0;
    long long total = 0;
    for (int i = 0; i < count; ++i) total += data[i];
    return total;
}

void free_array(int* data) {
    delete[] data;
}
