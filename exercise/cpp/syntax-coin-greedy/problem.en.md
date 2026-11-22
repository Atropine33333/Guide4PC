**Chapter 20**

The kingdom of heaven is likened unto a certain householder, which went out early to hire labourers into his vineyard. He agreed with them for a penny a day, and commanded his steward, saying: "Whoso cometh for his wages, let him take the greatest coin first, then the next greatest, until the sum be paid, and not one coin beyond."

So the steward brought forth eight kinds of coin: two hundred, one hundred, fifty, twenty, ten, five, two, and one. Ever he grasped the greatest first, and turned to the lesser only when the greater would grasp no more. And the householder counted the pieces in his hand and said, "This man's taking is well pleasing."

**In plain words**: implement `greedy_coin_count(amount)`, counting the amount greedily from the largest denomination down, and return the total number of coins taken.

```cpp
long long greedy_coin_count(long long amount);
```

**Rules**:

- The denominations are `200, 100, 50, 20, 10, 5, 2, 1` (in cents).
- `amount` is a non-negative integer, `0 ≤ amount ≤ 10^18`.
- Walk the denominations left to right: at each one take as many coins as possible, then move on.
- Return the total number of coins taken.
- Do not write `main`.

### Example

`greedy_coin_count(388)` takes one `200`, one `100`, one `50`, one `20`, one `10`, one `5`, one `2` and one `1` — eight coins in all.

### Hint

For each denomination `coin`, take `amount / coin` coins and then `amount %= coin`. Keep the total in `long long` as well: `10^18` cents in 200-cent coins needs `5·10^15` pieces, which `int` cannot hold.
