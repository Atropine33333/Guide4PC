A stack has one habit: last in, first out. You are given a stack of up to 128 `int` slots. Manage it: push, pop, look at the top, and report how many elements it holds.

**Interface**:

```c
#define INT_STACK_CAP 128

typedef struct {
    int data[INT_STACK_CAP];
    size_t size;
} IntStack;

void stack_init(IntStack *s);
int stack_push(IntStack *s, int value);
int stack_pop(IntStack *s, int *out);
int stack_peek(const IntStack *s, int *out);
int stack_size(const IntStack *s);
```

- `stack_push` returns `0` on success, `-1` when all 128 slots are taken (the stack stays unchanged).
- `stack_pop` writes the top element into `*out` and returns `0`; on an empty stack it returns `-1` and leaves `*out` alone.
- `stack_peek` looks without removing; returns `-1` on an empty stack.
- `stack_size` returns the number of elements.

### Example

Push `7`, then `9`: the top is `9`. Pop once and get `9`; pop again and get `7`. The stack is now empty, so another pop returns `-1`.

### Contract

- Implement only these functions; do not write `main`.
- Copying the struct by value produces an independent stack.
- No dynamic allocation; the capacity is `INT_STACK_CAP`.
