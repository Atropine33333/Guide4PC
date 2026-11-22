**Chapter One**

In those days a man came before the scorekeeper and said, "Write me down for four points." And the scorekeeper wrote four. The same man came again and said, "Six more." And the scorekeeper added the six to the four, and the book read ten. The kingdom of heaven is like a faithful scorekeeper: however many times the same man comes, the account adds up as many times, and not a point is missing.

Then someone asked, "When two men hold the same total, who takes the first seat?" It is written: the name that comes first is called first. Where the totals are equal, take the name that is smallest in lexicographic order; only a higher total earns the first seat on its own.

Then someone asked, "And if the book is empty, who takes the first seat?" The scorekeeper answered, "With no name in the book, there is no first seat. You shall not invent one out of air." And so it returns `false`.

As for how many are in the book, count the names: the same name, however often it comes, is counted once.

**In plain words**: implement the members of `ScoreBoard`: `add` accumulates points per player; `score` reports the total and returns `false` for an unknown player; `players` returns the number of distinct players; `leader` returns the highest scorer's name, breaking ties by the lexicographically smallest name, and returns `false` when the board is empty. Several behaviours in the starter break the contract; fix them one by one, keep the signatures, and do not write `main`.

This exercise reads no stdin and writes no stdout. A hidden harness runs the tests.

The contract:

- `void add(const std::string& player, int points)`: add points to a player. Repeated additions for the same player must **accumulate**. `points` is positive.
- `bool score(const std::string& player, int& out) const`: if the player exists, write the total to `out` and return `true`; otherwise return `false`.
- `int players() const`: return the number of distinct players seen.
- `bool leader(std::string& out) const`: write the name of the player with the highest total to `out` and return `true`; on a tie choose the lexicographically smallest name; return `false` when the board is empty.

Change only the implementation of `ScoreBoard`. Do not change member signatures or write `main`.
