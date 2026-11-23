Writing a log is like keeping a ledger: one line per entry, the level recorded in square brackets first, the body after it. The numbers and strings in the body are handed in by the caller, so the builder takes variable arguments, and a macro keeps the call short.

Implement `log_write` and add the `LOG` macro.

**Interface**:

```c
#include <stddef.h>

#define LOG(buf, cap, level, ...) /* expands to one call to log_write */

int log_write(char *buf, size_t cap, int level, const char *fmt, ...);
```

- `LOG` hands `buf`, `cap`, `level` and `__VA_ARGS__` to `log_write` unchanged.
- `log_write` puts the level name in square brackets, adds one space, appends the body formatted by `fmt`, writes the result into `buf` and terminates it with a null character.
- Levels `0` to `3` are written as `DEBUG`, `INFO`, `WARN` and `ERROR`; any other integer `n` is written as `LVLn`.
- Return the full length the line needs, without the terminating null. The return value counts the full length even when the line does not fit.
- Return `-1` when `buf` is null, `cap` is `0` or `fmt` is null, and do not write `buf` in that case.
- When the line does not fit, write `cap - 1` characters and then the null character.

### Example

```c
char buf[64];
LOG(buf, sizeof buf, 1, "start %d", 7);
// buf holds "[INFO] start 7" and the call returns 14

char small[10];
LOG(small, sizeof small, 1, "%s", "abcdefghij");
// the full length is 17 and does not fit; small holds "[INFO] ab", return 17
```

### Contract

- Keep the signature and the macro name as given; `log_write` is a variadic function.
- Use the standard library only; the tools in `stdarg.h` and `stdio.h` are enough.
- These functions and the macro are called directly; do not write `main`.
- No dynamic allocation; keep temporary buffers on the stack.
