[ DEEP-SPACE STATION · ENVIRONMENT POD PANEL ]

The environment pod has one row of small lamps for its configuration: three for the mode, four for the level, one for the master switch, all crowded onto the same panel.

The duty officer writes the three groups in, then reads them back to check. Before writing, the value has to fit: the mode goes up to seven, the level up to fifteen, and the master switch knows only on and off. A value that does not fit is handed back and the lamps stay as they were.

**In plain words**: implement the functions below on a bit-field struct holding `mode` (3 bits), `level` (4 bits) and `enabled` (1 bit); each setter checks its upper bound, returns `-1` on an out-of-range value and leaves the field alone; each getter returns the current value.

**Interface**:

```c
#define CFG_MODE_MAX 7u
#define CFG_LEVEL_MAX 15u

typedef struct {
    unsigned int mode : 3;
    unsigned int level : 4;
    unsigned int enabled : 1;
} Cfg;

void cfg_init(Cfg *c);
int cfg_set_mode(Cfg *c, unsigned mode);
int cfg_set_level(Cfg *c, unsigned level);
int cfg_set_enabled(Cfg *c, int on);
unsigned cfg_mode(const Cfg *c);
unsigned cfg_level(const Cfg *c);
int cfg_enabled(const Cfg *c);
```

- `cfg_init`: clear all three fields to `0`.
- `cfg_set_mode`: when `mode` is at most `CFG_MODE_MAX`, store it and return `0`; otherwise return `-1`.
- `cfg_set_level`: when `level` is at most `CFG_LEVEL_MAX`, store it and return `0`; otherwise return `-1`.
- `cfg_set_enabled`: accept `0` and `1` only, store the value and return `0`; any other value returns `-1`.
- The three getters return the current field values, with `cfg_enabled` reporting `0` or `1`.

### Example

After `cfg_init` all three fields are `0`. Write mode `5`, level `12` and the master switch `1`: each call returns `0`, and reading back gives exactly `5`, `12` and `1`. Writing `16` into the level returns `-1` and the level stays `12`.

### Contract

- Keep the field names, types, widths and declaration order as given.
- Each setter touches only its own field; the neighbouring fields stay untouched.
- A refused write leaves the field as it was before the call.
- These functions are called directly; do not write `main`.
- Use the standard library only, with no dynamic allocation.
