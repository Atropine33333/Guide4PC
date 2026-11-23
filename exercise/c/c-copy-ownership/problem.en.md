(The back room of a second-hand bookshop. Two tables stand side by side, each with a catalogue on it.)

A: This catalogue lists titles and prices. I want to copy it and take the copy away, and afterwards each of us edits their own. Will that work?

B: When you copy, do not copy just the address. The letters of each title live somewhere else; write that address down and both catalogues point at the same line of text.

A: Then if I change a title in mine, yours changes too?

B: Exactly. You need another sheet of paper, write the title out letter by letter, and let the two catalogues point at separate sheets.

A: And when each of us puts the catalogue away?

B: Whoever owns the sheet gives it back. Putting away either one first does not disturb the other.

**In plain words**: `Player` holds a `char *name` and an `int score`. `player_new` allocates one block for the struct and another for the name, copying the text in; `player_clone` makes a deep copy whose `name` points at fresh memory; `player_free` returns the name first, then the struct.

**Interface**:

```c
typedef struct {
    char *name;
    int score;
} Player;

Player *player_new(const char *name, int score);
Player *player_clone(const Player *src);
void player_free(Player *p);
```

- `player_new` requires a non-`NULL` `name` and copies `strlen(name) + 1` bytes. If either allocation fails it returns `NULL`.
- `player_new(NULL, score)` returns `NULL`.
- `player_clone(NULL)` returns `NULL`; the copy is a separate struct whose `name` points at a different block holding the same text.
- `player_free(NULL)` does nothing; each pointer is returned exactly once.
- Freeing one copy leaves the other fully usable.

### Example

```c
Player *a = player_new("Robin", 42);
Player *b = player_clone(a);   /* b->name points at another block */
b->name[0] = 'X';              /* a->name is still "Robin" */
player_free(a);
player_free(b);
```

### Contract

- Implement only these three functions; do not write `main`.
- Use `malloc` and `free`; no fixed-size global buffer.
- The stored `name` does not overlap the caller's buffer, so changing that buffer leaves the stored name unchanged.
- The empty string is copied as well, producing a block that holds only `'\0'`.
