THE GOSPEL OF MATTHEW — THE PARABLE OF THE NUMBERER

The kingdom of heaven is like a man who sits at the gate and gives out numbers. To each one who enters the garden he gives a number in order: the first receives one, the second receives two, the third receives three, and so on until sunset.

A disciple asked him, "Where is the count kept?" He answered, "In my heart, and not on paper. When one company departs, the count remains here; when another comes, it continues from there."

Another disciple asked, "How many have been given out?" He said, "Ask, and I answer; and after you ask, the number is still the same, for asking consumes no ticket."

And at evening the master of the house said, "From this day, begin the numbering anew." So the man cleared the count, and the next to come received number one.

When Jesus had finished this parable, he said, "What is kept in the heart does not scatter with the crowd; what is kept only on paper is gone when the wind blows."

**In plain words**: Implement `next_ticket()`, `tickets_issued()`, and `reset_tickets()`: numbers start at 1 and rise by one per ticket; the count must survive from one call to the next, which a function-scope `static` variable handles (a global in an anonymous namespace works too); `tickets_issued()` only reads; `reset_tickets()` sets the count to zero so numbering starts at 1 again.

### Rules

- `int next_ticket()`: return the next ticket number, starting at 1 and increasing by 1 per call.
- `int tickets_issued()`: return how many tickets have been issued since the last reset.
- `void reset_tickets()`: reset the dispenser; the next `next_ticket()` returns 1 again, and `tickets_issued()` returns 0.
- The state must survive across function calls. Do not write `main`.
