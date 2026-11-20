# S1: School C++ (test page)

This page validates the exercise format: the prose stays in markdown, while the starter code and test cases live in `exercises/count-digit-occurrences/`, read by the build and the runner.

## Problem 1: Count a digit

Read a non-negative integer and a digit (0–9); count how many times the digit appears in the integer.

**Input**: two lines — the integer, then the digit.

**Output**: one integer — the number of occurrences.

For example, given `676767` and `6`, print `3`:

```text
676767
6
3
```

:::exercise{id="count-digit-occurrences"}
:::

### Extension: no `<string>`

In normal C++ practice, converting the integer with `std::string` and comparing character by character is perfectly fine. But some school judges ship headers without `<string>`, and this extension exists for exactly that case: solve it without the string library.

This variant is declared in `exercises/count-digit-occurrences/spec.json`; it forbids `<string>` and `std::string` in the submitted source.

:::exercise{id="count-digit-occurrences" variant="no-string"}
:::

## Problem 2: Circular buffer (harness exercise)

This one reads no stdin and writes no stdout. You implement a class; the test logic lives in a hidden harness that is compiled after your code and calls `push`, `pop` and `size` directly.

Implement `CircularBuffer` with a fixed capacity set in the constructor:

- `push(value)`: write and return `true` if not full; return `false` if full;
- `pop(out)`: move the oldest value into `out` and return `true` if not empty; return `false` if empty;
- `size()` / `capacity()`: current element count / total capacity.

Do not write a `main`; the harness provides the entry point. The starter is a compilable placeholder: every check fails until you add the members and fill in the methods.

:::exercise{id="circular-buffer"}
:::
