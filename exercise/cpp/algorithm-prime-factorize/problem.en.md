**Chapter 1**

In the beginning was the number, and the number was one, and the void was upon the face of the deep.

And the LORD spake unto Moses, saying,

"Take thou this integer `n`, and divide it among the primes, and leave nothing over. A prime is a number that no other number divideth, save one and itself. Begin from the least, and pass not over it; and write the portion of each prime as its exponent. Divide until it cannot be divided; and if a number greater than one remain, that also is prime, and unto it shalt thou give its portion."

And Moses did as the LORD commanded.

**In plain words**: factor `n` into primes, and output each as `p^e`, smallest prime first; write `p^1` even when the exponent is 1.

**Input**: one integer `n` on a single line (`2 ≤ n ≤ 10^12`).

**Output**: several terms of the form `p^e`, separated by single spaces, where `p` is prime and `e` is its exponent.

### Example

Input:

```text
12
```

Output:

```text
2^2 3^1
```

### Constraints

- List the prime factors from smallest to largest.
- Each prime appears once; write its exponent in decimal, without leading zeros.
- If `n` is prime, output the single term `n^1`.
- Trial division up to `sqrt(n)` is enough; remember to check whether the remainder is greater than 1.
