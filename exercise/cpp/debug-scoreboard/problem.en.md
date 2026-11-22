This exercise reads no stdin and writes no stdout. The `ScoreBoard` below compiles, but a few behaviors violate the contract. Find and fix the bugs; a hidden harness runs the tests.

The contract:

- `void add(const std::string& player, int points)`: add points to a player. Repeated additions for the same player must **accumulate**. `points` is positive.
- `bool score(const std::string& player, int& out) const`: if the player exists, write the total to `out` and return `true`; otherwise return `false`.
- `int players() const`: return the number of distinct players seen.
- `bool leader(std::string& out) const`: write the name of the player with the highest total to `out` and return `true`; on a tie choose the lexicographically smallest name; return `false` when the board is empty.

Change only the implementation of `ScoreBoard`. Do not change member signatures or write `main`.
