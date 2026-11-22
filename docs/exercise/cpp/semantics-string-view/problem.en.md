A DUBBED FILM — THE TELEGRAM THAT MUST NOT BE COPIED

(Study, by the fireplace. The old butler bows, holding a sheet of paper.)

BUTLER: Oh, my dear sir, the postman has just brought this telegram. It is dreadfully long, and copying it out would make us late for dinner.

MASTER: Good heavens, then don't copy it. Simply point your finger at the part you wish to read — where it begins, where it ends — and keep your eyes on the original sheet. The paper does not move, and not one letter is to be written out elsewhere.

BUTLER: And if I should wish to trim the blank margins at both ends?

MASTER: Then move your finger inward a little, and the paper remains the very same paper. Oh, and should the mark you seek be missing, report plainly that it is missing; do not hand me a blank sheet in its place.

BUTLER: As you wish, sir. This telegram shall stay on the table until you have finished with it.

**In plain words**: Implement `trim`, `first_word`, `between`, and `starts_with` with `std::string_view`: observe characters only, never copy, and make every non-empty result point straight into the `text` buffer it was given, never into a local object or a temporary `std::string`.

```cpp
#include <string>
#include <string_view>

std::string_view trim(std::string_view text);
std::string_view first_word(std::string_view text);
std::string_view between(std::string_view text, char open, char close);
bool starts_with(std::string_view text, std::string_view prefix);
```

### Rules

- `trim` removes leading and trailing ASCII spaces `' '` and returns the rest; an all-space or empty input yields an empty view.
- `first_word` returns the span from the first non-space character to the next space or the end; it yields an empty view when there is no non-space character.
- `between` returns the span after the first `open` and before the next `close` after it; it yields an empty view when `open` is missing or has no following `close`.
- `starts_with` reports whether `prefix` is a prefix of `text`; an empty prefix counts as true.
- Non-empty results must point directly into `text`, never into a local object or a temporary `std::string`.
- Do not write `main`.

### Hint

A view returned by `std::string_view::substr` still points into the original buffer; watch for `std::string_view::npos` from `find`.
