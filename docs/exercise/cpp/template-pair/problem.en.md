(Late at night, in the castle study. The old butler arrives with a velvet box that holds exactly two places.)

Butler: Oh, my dear sir, this box is prepared for two guests. One on the left, one on the right, and neither may sit in the wrong place.

Master: Good heavens, only two places, and they still must be told apart?

Butler: Of course, sir. Take the left guest out and that place stands empty; to exchange them we prepare a second box, seat them the other way round, and the first box never stirs.

Master: And if the two guests are identical?

Butler: Oh, then they are equal, sir. Equal guests may sit on either side, yet we still say "equal" by the rules. Equality means both places have been compared.

Master: May one read them, or change them?

Butler: Both, sir. The box is lettered `first()` and `second()`, each with two keys: one for ordinary guests, one for guests who may only look.

Master: And an empty box?

Butler: An empty box can be made too, sir. Both places are laid with default values first, and the guests sit down afterwards.

**In plain words**: implement the class template `Pair<T>` with a default constructor, a two-value constructor, `const` and non-`const` `first()` / `second()`, `swapped()`, and `operator==`, exactly as itemised below.

- `Pair()`: value-initializes both elements.
- `Pair(const T& first, const T& second)`: constructs from the given values.
- `T& first()` / `const T& first() const`: access the first element; both overloads are required.
- `T& second()` / `const T& second() const`: access the second element; both overloads are required.
- `Pair<T> swapped() const`: return a new pair with the elements exchanged, leaving the original unchanged.
- `bool operator==(const Pair<T>& other) const`: return `true` when both elements are equal.

The template is instantiated with `int`, `std::string`, and `Pair<int>`; a `const` object must also allow reading its elements.
