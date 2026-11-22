Read an integer `n`, count the primes not exceeding `n`, and find the largest one.

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
