// 参考实现：templates-variadic-sum
template <typename... Args>
long long sum_all(Args... args) {
    return (0LL + ... + static_cast<long long>(args));
}
