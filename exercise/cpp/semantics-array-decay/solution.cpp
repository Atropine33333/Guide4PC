// Reference solution: semantics-array-decay
#include <cstddef>
#include <string>

std::size_t array_bytes(const int (&values)[6]) {
    return sizeof(values);
}

std::size_t pointer_bytes(const int* values) {
    return sizeof(values);
}

std::string which_overload(int (&values)[6]) {
    (void)values;
    return "array";
}

std::string which_overload(const int* values) {
    (void)values;
    return "pointer";
}

int sum_array(const int (&values)[6]) {
    int total = 0;
    for (int value : values) total += value;
    return total;
}

int sum_range(const int* first, const int* last) {
    int total = 0;
    for (const int* it = first; it != last; ++it) total += *it;
    return total;
}
