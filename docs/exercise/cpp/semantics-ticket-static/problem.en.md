This exercise reads no stdin and writes no stdout. Implement a ticket dispenser; a hidden harness runs the tests.

Implement three functions:

- `int next_ticket()`: return the next ticket number, starting at 1 and increasing by 1 per call.
- `int tickets_issued()`: return how many tickets have been issued since the last reset.
- `void reset_tickets()`: reset the dispenser; the next `next_ticket()` returns 1 again, and `tickets_issued()` returns 0.

The state must survive across function calls. Do not write `main`.
