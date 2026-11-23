When a machine puts a 32-bit integer into memory, its four bytes follow a fixed order. Read a hexadecimal number with a `0x` prefix, decide whether this machine is little-endian, then report the four bytes in the order they actually occupy in memory here.

**Input**: one line with a 32-bit unsigned integer written in hexadecimal with a `0x` prefix, from `0x00000000` to `0xFFFFFFFF`; letter case does not matter.

**Output**: two lines.

- The first line is `little=<0/1>`: `little=1` when this machine is little-endian, `little=0` otherwise.
- The second line is `bytes=<b0> <b1> <b2> <b3>`: the four bytes in increasing address order, as decimal values from `0` to `255`, separated by single spaces.

### Example

Input:

```text
0x12345678
```

Output:

```text
little=1
bytes=120 86 52 18
```

`0x12` is the highest byte and lands last; `0x78` is the lowest byte and lands first, so this machine is little-endian.

### Contract

- Store the value in a `uint32_t` and build the `scanf` format string with the `SCNx32` macro.
- To detect little-endian order, inspect the bytes of an integer whose value is `1`: if the byte at the lowest address is `1`, the machine is little-endian.
- Read the object's byte representation through an `unsigned char *`; C allows character types to inspect the representation of any object.
- This machine is x86-64 little-endian, so `little` is `1`.
- No trailing spaces at the end of the line.
