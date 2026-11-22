template <typename... Args>
long long sum_all(Args... args) {
    (void)sizeof...(args);
    // TODO: 用折叠表达式把每个参数转成 long long 再求和。
    return 0;
}
