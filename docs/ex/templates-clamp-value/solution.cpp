// 参考实现：templates-clamp-value
template <typename T>
T clamp_value(const T& value, const T& low, const T& high) {
    if (value < low) return low;
    if (high < value) return high;
    return value;
}
