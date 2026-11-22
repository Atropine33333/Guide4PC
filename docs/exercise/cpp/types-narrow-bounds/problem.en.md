This exercise reads no stdin and writes no stdout. Implement four safe narrowing functions; a hidden harness runs the tests.

For each function: if `value` is exactly representable in the target type, write the converted result to `out` and return `true`; otherwise return `false` and **do not modify** `out`.

- `bool narrow_i8(long long value, std::int8_t& out)`
- `bool narrow_i16(long long value, std::int16_t& out)`
- `bool narrow_u8(long long value, std::uint8_t& out)`
- `bool narrow_i32(long long value, std::int32_t& out)`

Use the fixed-width integer types from `<cstdint>` and the limits from `<limits>`. Do not write `main`.
