[ REMOTE SUPPLY DEPOT · DUTY LOG ]

The depot has one eight-bit cargo plate left, and the plate is cut into three cells:

hatch, one bit, `0` open and `1` locked;
crew tier, two bits, `0` to `3`;
oxygen, five bits, `0` to `31`.

The cells run from low bit to high bit: the hatch takes the rightmost bit, the crew tier the next two, the oxygen the leftmost five, which adds up to exactly eight.

The duty officer does two things: write the three cells onto the plate, or read the three cells back from the eight bits on it. Any bit outside the plate is handed back untouched.

**In plain words**: implement the functions below to pack three bit-field members (1, 2 and 5 bits wide, in declaration order) into an 8-bit word, and to unpack that word back into the three members; every out-of-range value is refused.

**Interface**:

```c
#define FLAGS_WORD_MAX 0xFFu

typedef struct {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
} Flags;

void flags_init(Flags *f);
int flags_set(Flags *f, char which, unsigned value);
unsigned flags_get(const Flags *f, char which);
unsigned flags_pack(const Flags *f);
int flags_unpack(Flags *f, unsigned word);
```

- `flags_init`: clear all three members to `0`.
- `flags_set`: `which` is `'a'`, `'b'` or `'c'`, accepting `0..1`, `0..3` and `0..31` respectively, and returns `0` on success. An out-of-range value or an unknown name returns `-1` and leaves the struct unchanged.
- `flags_get`: return the current value of the member; an unknown name returns `0`.
- `flags_pack`: pack the three members into an 8-bit word, with `a` at bit 0, `b` at bits 1 to 2 and `c` at bits 3 to 7, and return it.
- `flags_unpack`: when `word` is at most `FLAGS_WORD_MAX`, load the three members in the same bit order and return `0`; otherwise return `-1` and leave the struct unchanged.

### Example

Set `a = 1`, `b = 2` and `c = 5` in turn, and packing gives `1 + 4 + 40 = 45`; unpacking `45` reads back exactly `a = 1`, `b = 2`, `c = 5`.

Trying to write `4` into `b` is refused, and the packed word is still `45`.

### Contract

- Keep the struct declaration and the function signatures as given; these functions are called directly.
- The three members share one piece of storage, 1, 2 and 5 bits wide, ordered from low bit to high bit as declared.
- The packed word uses only the low 8 bits; higher bits are never accepted.
- A refused write leaves the previous values in place.
- Use the standard library only, and do not write `main`.
