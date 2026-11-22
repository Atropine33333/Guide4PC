MANUAL: THE READ-ONLY MEASUREMENT INTERFACE

§1 Purpose
This interface measures how long a character sequence ending in `'\0'` is. It reads; it never writes.

§2 Prototype

```c
size_t str_len(const char *s);
```

§3 Parameter
`s` points at the first character of the sequence. The caller may pass `NULL`, and may pass a string literal.

§4 Return value
The number of characters before the first `'\0'`; `0` when `s` is `NULL`.

§5 The read-only promise
The implementation must not write any character through `s`. Literals, `const` arrays and ordinary arrays may all be passed directly; the caller need not make a writable copy.

§6 How it is called
These functions are called directly, so implement the functions only and do not write `main`.

**In plain words**: implement `size_t str_len(const char *s)` to return the length of the string; return `0` when `s` is `NULL`, and never modify what `s` points at.

**Interface**: `size_t str_len(const char *s);`

### Example

```c
str_len("hello")                  // 5
str_len("")                       // 0
str_len(NULL)                     // 0
char buf[4] = "abc"; str_len(buf) // 3, and buf is still "abc"
```

### Contract

- Count the characters before `'\0'`; the terminator itself does not count.
- Handle `NULL` first, before touching the first character.
- No `main`; the function needs no input and produces no output.
- The standard library only; no dynamic allocation.
