Read a 32-bit value written in hexadecimal; it may be the bit pattern of a `float`. Extract its sign bit, exponent field and mantissa field, and decide which category it belongs to.

### Input

One line with 1 to 8 hexadecimal digits (either case), without a `0x` prefix. For example `3F800000` is `1.0f`.

### Output

One line with four fields separated by single spaces:

```
<kind> <sign> <exponent> <mantissa>
```

- Decide the kind in this order: if the exponent field is `255`, output `inf` when the mantissa field is `0`, otherwise `nan`; if the exponent field is `0`, output `zero` when the mantissa field is `0`, otherwise `subnormal`; otherwise output `normal`.
- Output the sign as `0` (positive) or `1` (negative); negative zero prints `1`.
- The exponent field is the raw value of bits 23–30 (`0`–`255`), without subtracting `127`.
- The mantissa field is the raw value of bits 0–22 (`0`–`8388607`), without the implicit leading bit.

### Example

| Input | Output |
| --- | --- |
| `3F800000` | `normal 0 127 0` |
| `BF800000` | `normal 1 127 0` |
| `00000001` | `subnormal 0 0 1` |
| `7F800000` | `inf 0 255 0` |
| `7FC00000` | `nan 0 255 4194304` |

### Constraints

- The hex string has 1 to 8 digits and may start with zeros.
- Use lowercase letters and decimal digits only; end the line with a newline.
