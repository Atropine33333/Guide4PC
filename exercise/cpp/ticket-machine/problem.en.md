This exercise reads no stdin and writes no stdout. Implement a ticket machine class; the hidden harness provides the tests and the program entry point.

Implement `TicketMachine`; every machine keeps its own counter:

- `int next()`: issues and returns the next ticket number. A fresh machine returns `1` first, then `2`, `3`, and so on.
- `int issued() const`: returns how many tickets this machine has issued.
- `void reset()`: sets the issued count back to zero, so the next `next()` returns `1` again.

Do not write `main` inside `TicketMachine`, and do not touch standard input or output.

### Hint

A function-local `static` counter is shared by every machine. The counter belongs to each object, not to a function or to the whole program.
