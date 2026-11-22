MIDNIGHT TV SHOPPING — THE TICKET MACHINE

(Late night. The studio lights flicker a little. The host lifts a small machine.)

HOST: "Friends, still writing numbers on scraps of paper? Still arguing in the queue? Look at this ticket machine! Press once and it hands you number 1; press again, number 2; again, number 3 — every press goes up by one, and it never skips a single number!"

"But wait, there's more! This machine costs only $19.99! Every machine keeps its own count: the one next door starts at 1, yours starts at 1, and neither disturbs the other! Press `issued()` and it reports how many tickets went out today; press `reset()` and it starts over from 1!"

"Order now and we throw in a class! No `main` required!"

(The host sets the machine down. A voice off camera lowers itself: the counter belongs to the object, not to a function or to the whole program.)

**In plain words**: implement class `TicketMachine`: `next()` issues and returns the next ticket number, with a fresh machine starting at `1` and increasing by one; `issued() const` reports how many tickets this machine has issued; `reset()` clears that count so the next `next()` returns `1` again. Each object keeps its own counter.

### Rules

- `int next()`: issues and returns the next ticket number. A fresh machine returns `1` first, then `2`, `3`, and so on.
- `int issued() const`: returns how many tickets this machine has issued.
- `void reset()`: sets the issued count back to zero, so the next `next()` returns `1` again.
- Do not write `main` inside `TicketMachine`.

### Hint

A function-local `static` counter is shared by every machine. The counter belongs to each object, not to a function or to the whole program.
