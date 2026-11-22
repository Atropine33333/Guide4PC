这一题不读 stdin，也不写 stdout。实现四个安全窄化函数，测试在隐藏的 harness 里。

对每个函数：如果 `value` 能被目标类型精确表示，就把转换结果写入 `out` 并返回 `true`；否则返回 `false`，并且**不修改** `out`。

- `bool narrow_i8(long long value, std::int8_t& out)`
- `bool narrow_i16(long long value, std::int16_t& out)`
- `bool narrow_u8(long long value, std::uint8_t& out)`
- `bool narrow_i32(long long value, std::int32_t& out)`

使用 `<cstdint>` 中的定宽整数类型和 `<limits>` 中的边界值。不要写 `main`。
