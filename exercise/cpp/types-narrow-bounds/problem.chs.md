关于印发《窄化转换审批办法》的通知

各承办单位：

为杜绝窄化转换中的静默截断，现将审批办法印发给你们，请遵照执行。

第一条 适用范围。凡将 `long long` 写入较窄整数类型者，均须报批。
第二条 器具。审查使用 `<cstdint>` 中的定宽整数类型与 `<limits>` 中的边界值。
第三条 批准。目标类型能精确表示该值者，予以批准，写入 `out`，返回 `true`。
第四条 驳回。不能精确表示者，一律驳回，返回 `false`，原 `out` 保持原值，一分不得更动。
第五条 本办法设四道窗口，分别受理 `std::int8_t`、`std::int16_t`、`std::uint8_t`、`std::int32_t`。
第六条 不得书写 `main`。

**人话**：实现四个安全窄化函数，每个都要判断 `value` 能否被目标类型精确表示：能则写入 `out` 并返回 `true`，不能则返回 `false` 且不改动 `out`。

```cpp
bool narrow_i8(long long value, std::int8_t& out);
bool narrow_i16(long long value, std::int16_t& out);
bool narrow_u8(long long value, std::uint8_t& out);
bool narrow_i32(long long value, std::int32_t& out);
```
