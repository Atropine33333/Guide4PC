"Oh, my dear," said the old dispatcher, pushing his spectacles up his nose, "wherever is that word hiding in this telegram?"

"Good heavens," said the young telegraphist, "I have read it from top to bottom, over and over — my eyes are worn out."

"My child, there is an art to reading," said the old man. "First measure the word itself and note where it repeats itself — that is the prefix function. Then walk the telegram once; when a letter refuses to match, do not begin again from the start, step back to where you belong. Thus each letter is looked at once, and every position is found, overlaps and all."

"Oh, how marvellous," said the girl.

**In plain words**: implement `find_all`, which uses KMP (the prefix function) to return the starting indices of every occurrence of the pattern in the text in ascending order, overlaps included; return an empty `vector` for an empty pattern or a pattern longer than the text. Nothing is read from stdin and nothing is written to stdout; a hidden harness runs the tests, so do not write `main` and do not change the signature.

```cpp
#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> find_all(const std::string& text,
                                  const std::string& pattern);
```

### Rules

- Return every starting index `pos` such that `text[pos + i] == pattern[i]` for all valid `i`, in ascending order.
- Overlaps are allowed. For example, searching for `"aaa"` in `"aaaaa"` returns `{0, 1, 2}`.
- Return an empty `vector` for an empty pattern.
- Return an empty `vector` when the pattern is longer than the text.
- The strings contain ASCII characters only and comparisons are case-sensitive.
- Use KMP (the prefix function); do not call `std::string::find` repeatedly.
- Do not write `main`, and do not change the signature.
