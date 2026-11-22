MANUAL: ASSEMBLING THE CHANNEL MODULE

§1 Purpose
This module manages four channels on one configuration bus, numbered `0` to `3`. Each channel carries a gain (`0` to `100`) and an on/off mark.

§2 Constants
`CHANNEL_MAX` is `4`; `CHANNEL_MAGIC` is `0x5A`.

§3 Type
The `Channel` struct holds the channel number `id`, the gain `gain` and the on/off mark `open`.

§4 Guards
The declaration block is guarded by `G4_CHANNEL_H`, the implementation block by `G4_CHANNEL_IMPL_H`. Both are written as `#ifndef` name, `#define` name, body, `#endif`, with the name repeated in a comment after `#endif`. When the same file is read a second time, each block is skipped whole, so declarations and definitions each appear once.

§5 Interface

```c
#define CHANNEL_MAX 4
#define CHANNEL_MAGIC 0x5Au

typedef struct {
    int id;
    int gain;
    int open;
} Channel;

int channel_open(Channel *ch, int id, int gain);
int channel_set_gain(Channel *ch, int gain);
int channel_close(Channel *ch);
int channel_is_open(const Channel *ch);
unsigned channel_magic(void);
```

§6 Behaviour
- `channel_open`: returns `-1` when `ch` is null; `-1` when `id` is outside `0` to `CHANNEL_MAX - 1`; `-1` when `gain` is outside `0` to `100`. When all three pass it stores `id` and `gain`, sets `open` to `1` and returns `0`.
- `channel_set_gain`: returns `-1` when `ch` is null or `open` is `0`, and `-1` when `gain` is out of range; otherwise it stores the new gain and returns `0`.
- `channel_close`: returns `-1` when `ch` is null or `open` is `0`; otherwise it sets `open` to `0` and returns `0`.
- `channel_is_open`: `0` when `ch` is null or the channel is closed, `1` when it is open.
- `channel_magic`: returns `CHANNEL_MAGIC`.
- A failed call leaves the struct unchanged.

§7 How it is called
These functions are called directly, so implement the guard blocks and the functions only, and do not write `main`.

**In plain words**: wrap the interface declarations and the function definitions in `#ifndef` guards named `G4_CHANNEL_H` and `G4_CHANNEL_IMPL_H`, then finish the five channel functions. The guards let a second read of the same text skip each block whole.

**Interface**: the declarations in §5.

### Example

```c
Channel ch;
channel_open(&ch, 2, 40);      // 0, and ch.open is 1
channel_set_gain(&ch, 101);    // -1, the gain is still 40
channel_close(&ch);            // 0
channel_close(&ch);            // -1, it is already closed
```

### Contract

- Keep the struct and the function signatures as given; do not rename the fields.
- The two guard macros may be empty; put the closing comment after `#endif`.
- A failed call leaves the struct as it was.
- Use the standard library only; no dynamic allocation, and do not write `main`.
