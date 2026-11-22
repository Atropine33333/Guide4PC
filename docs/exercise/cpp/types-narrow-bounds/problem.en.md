Circular on the Issuance of the Measures for Approving Narrowing Conversions

To all handling units:

In order to put an end to silent truncation in narrowing conversions, the approval measures are hereby issued to you for compliance.

Article 1. Scope. Any conversion that writes a `long long` into a narrower integer type shall be submitted for approval.
Article 2. Instruments. Review shall use the fixed-width integer types of `<cstdint>` and the boundary values of `<limits>`.
Article 3. Approval. Where the target type can represent the value exactly, approval is granted: write the value to `out` and return `true`.
Article 4. Rejection. Where exact representation is impossible, the request is rejected: return `false` and leave `out` at its original value, unaltered in the slightest.
Article 5. Four counters are established, receiving `std::int8_t`, `std::int16_t`, `std::uint8_t` and `std::int32_t` respectively.
Article 6. No `main` shall be written.

**In plain words**: implement four safe narrowing functions. Each decides whether `value` is exactly representable in its target type: if so, write it to `out` and return `true`; otherwise return `false` and leave `out` untouched.

```cpp
bool narrow_i8(long long value, std::int8_t& out);
bool narrow_i16(long long value, std::int16_t& out);
bool narrow_u8(long long value, std::uint8_t& out);
bool narrow_i32(long long value, std::int32_t& out);
```
