> From two on up to a million's end,
> strike every multiple from the band;
> whatever stays has no divisor found —
> how many primes does `n` command?

Beside the old rules of the *Nine Chapters*, Liu Hui notes: a number that can be divided only by one and by itself is called prime; one does not count. Now a number `n` is given: from two up to `n`, how many are prime?

The method: list the numbers from two to `n`; take two and strike out its doubles, its triples, every multiple that does not exceed `n`; take the smallest number not yet struck out and strike its multiples in the same way. Once the chosen number squared passes `n`, stop. What is left is prime.

**In plain words**: read one integer `n` and print how many primes lie between `2` and `n`, inclusive.

**Input**: one line with an integer `n` (`0 ≤ n ≤ 1000000`).

**Output**: one integer — the count of primes not exceeding `n`.

### Example

Input:

```text
10
```

Output:

```text
4
```

`2`, `3`, `5` and `7` are the primes not exceeding `10` — four in all.

### Contract

- For `0` and `1` there is no prime to count, so print `0`.
- With `n` up to one million, one sieve pass suffices; a marker array of `1000001` entries is enough.
- No trailing spaces at the end of the line.
