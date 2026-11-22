这一题不读 stdin，也不写 stdout。实现一个时长格式化函数，测试在隐藏的 harness 里。

实现：

- `std::string format_duration(long long total_seconds)`：把秒数格式化为 `H:MM:SS`。

规则：

- `H` 是小时数，不补前导零，可以超过 99；
- `MM` 和 `SS` 都必须是恰好两位，不足两位时补 `0`；
- `total_seconds` 保证非负。

例如 `0` 输出 `0:00:00`，`3599` 输出 `0:59:59`，`3600` 输出 `1:00:00`。不要写 `main`。
