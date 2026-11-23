《The Checksum Seal》

A telegram leaves the coast and crosses seven checkpoints on its way inland. At every checkpoint someone asks: is this still the same string of characters?

Now there is the CRC16 Checksum Seal. However long the ASCII string, one stamp leaves just four hexadecimal digits. The receiver stamps the message again; if the two stamps match, not a single character was lost.

The seal follows three rules:

- initial value `0xFFFF`;
- polynomial `0x1021`;
- push from the highest bit to the lowest, eight times per byte. Bits are not reflected, and there is no final XOR.

Spaces count as text; the line ending does not. The four digits are uppercase, padded with leading zeros to four places.

**In plain words**: Read one line of ASCII text, compute its CRC-16/CCITT-FALSE value, and print `CRC16=0x` followed by four uppercase hexadecimal digits. The initial value is `0xFFFF` and the polynomial is `0x1021`; for each byte, XOR it into the high eight bits of the CRC, then shift left eight times, XORing the polynomial whenever the top bit is `1`.

**Input**: one line of ASCII text, length between `0` and `200`, with character codes from `0x20` to `0x7E`. The line may end with a newline or not. An empty line is valid input.

**Output**: one line, `CRC16=0x` followed by four uppercase hexadecimal digits, for example `CRC16=0x29B1`.

### Example

Input:

```text
123456789
```

Output:

```text
CRC16=0x29B1
```

### Contract

- Treat each character as one byte, using its ASCII code; the trailing `\n` and `\r` are not counted.
- The checksum of an empty line is `0xFFFF`.
- Convert each character to `unsigned char` before the arithmetic, so a signed `char` cannot sign-extend it.
- Keep the CRC as a 16-bit unsigned value; after each shift keep only the low 16 bits.
- Pad the output to four digits with leading zeros and use uppercase letters.
- No trailing spaces at the end of the line.
