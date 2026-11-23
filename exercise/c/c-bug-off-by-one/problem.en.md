(A midnight shopping channel. The host slaps a stack of bills.)

Host: Look — `n` bills, not one more, not one less. Our master adds them up on the spot and calls the total!

Assistant: Done! Calling it now!

Host: Not so fast. This machine always leaves the last bill sitting on the desk, so the total comes out one entry short...

(The camera zooms in: one entry is missing from the screen.)

**In plain words**: the program in `starter.c` should read `n` integers and print their sum. It drops the last number every time, so the result is too small. Fix that one spot and let the output come out right.

**Input**: the first line holds an integer `n` (`1 ≤ n ≤ 200000`); the second line holds `n` integers, each with absolute value at most `10^9`, separated by single spaces.

**Output**: one line, the sum of the `n` numbers.

### Example

Input:

```text
3
2 -5 10
```

Output:

```text
7
```

### Contract

- The sum can leave 32-bit range; keep it in a `long long`.
- Change only the broken spot; keep the input and output format as it is.
- Print one line, ending with a newline.
