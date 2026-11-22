> A grid of numbers, zero and up,
> strike out the twos and fill the cup;
> strike on until no strike remains,
> what stays is prime, and prime it stays.
>
> How many primes lie in the grid?
> And which of them is the greatest?

The *Nine Chapters* has a rule: lay out the numbers from two to `n`; take two and strike out every multiple of two; then take the smallest number not yet struck and strike out every multiple of it; repeat until nothing more can be struck. What remains is prime.

A row of numbers runs from zero to `n`. How many of them are prime, and which is the greatest? If the row holds no prime at all, both answers are zero.

**In plain words**: read an integer `n`, count the primes not exceeding `n`, and report the largest of them; print `0 0` when there is none. Use a sieve of Eratosthenes in `O(n log log n)` time.

**Input**: one line with an integer `n` (`0 ≤ n ≤ 10^6`).

**Output**: one line with two integers separated by a single space: the number of primes and the largest prime not exceeding `n`. If there is no prime, print `0 0`.

### Example

Input:

```text
10
```

Output:

```text
4 7
```

The primes up to 10 are 2, 3, 5, and 7.

### Constraints

- `n` may be as small as 0 or 1.
- Trial division up to `10^6` is slow; use a sieve of Eratosthenes for `O(n log log n)` time.
- `1` is not prime.
