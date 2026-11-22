#include <cstddef>
#include <string>

std::size_t array_bytes(const int (&values)[6]) {
    (void)values;
    // TODO: return the size of the whole array.
    return 0;
}

std::size_t pointer_bytes(const int* values) {
    (void)values;
    // TODO: return the size of the pointer.
    return 0;
}

std::string which_overload(int (&values)[6]) {
    (void)values;
    // TODO: distinguish the array-reference overload.
    return "unknown";
}

std::string which_overload(const int* values) {
    (void)values;
    // TODO: distinguish the pointer overload.
    return "unknown";
}

int sum_array(const int (&values)[6]) {
    (void)values;
    // TODO: add all six elements.
    return 0;
}

int sum_range(const int* first, const int* last) {
    (void)first;
    (void)last;
    // TODO: add the elements in [first, last).
    return 0;
}
