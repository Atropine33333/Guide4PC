A string in C has no home of its own. All you hold is a `const char *`, and the memory behind it can be taken away or overwritten at any time. To keep a piece of text, you have to ask for a patch of memory yourself, move the characters in one by one, and read them back from the start of that patch.

**In plain words**: write `dup_str` to allocate `strlen(s) + 1` bytes, copy `s` into them along with the terminating `'\0'`, and return the start address; `free_str` gives that memory back.

**Interface**:

```c
char *dup_str(const char *s);
void free_str(char *s);
```

- `dup_str(NULL)` returns `NULL`.
- If the memory cannot be obtained, return `NULL`.
- The copy is fresh memory that does not overlap `s`; changing characters in the copy leaves the source untouched.
- The copy keeps every character, however long the string is.
- `free_str(NULL)` does nothing; `free_str` accepts only pointers returned by `dup_str`.

### Example

```c
char *p = dup_str("hello");   /* p points at "hello", not at the literal */
p[0] = 'H';                   /* the literal "hello" is unaffected */
free_str(p);
```

### Contract

- Implement only these two functions; do not write `main`.
- Use only standard facilities such as `malloc` and `free`; no variable-length arrays, no global buffer.
- Return each copy exactly once.
