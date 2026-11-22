// 参考实现：templates-swap-values
template <typename T>
void swap_values(T& first, T& second) {
    T temporary = first;
    first = second;
    second = temporary;
}

template <typename T>
T min_value(T first, T second) {
    return first < second ? first : second;
}
