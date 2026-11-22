#include <cstddef>

int* make_zero_array(int count) {
    if (count <= 0) return nullptr;
    // TODO: 值初始化分配（当前已分配，但后续函数仍未完成）。
    return new int[static_cast<std::size_t>(count)]();
}

void fill_sequence(int* data, int count, int start) {
    (void)data;
    (void)count;
    (void)start;
    // TODO: data[i] = start + i。
}

long long sum_array(const int* data, int count) {
    (void)data;
    (void)count;
    // TODO: 求和；空指针或非正长度返回 0。
    return 0;
}

void free_array(int* data) {
    // nullptr 也可以安全地传给 delete[]。
    delete[] data;
}
