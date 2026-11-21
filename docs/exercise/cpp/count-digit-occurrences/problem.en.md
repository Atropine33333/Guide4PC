Read a non-negative integer and a digit (0–9); count how many times the digit appears in the integer.

**Input**: two lines — the integer, then the digit.

**Output**: one integer — the number of occurrences.

For example, given `676767` and `6`, print `3`:

```text
676767
6
3
```

### Extension: no `<string>`

Converting the integer with `std::string` and comparing character by character is perfectly fine — unless the school judge ships headers without `<string>`. This variant exists for exactly that case: solve it without the string library. Switch to the “No `<string>` on the school machine” variant.
