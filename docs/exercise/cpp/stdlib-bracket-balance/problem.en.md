This exercise reads no stdin and writes no stdout. Implement a bracket-matching function; a hidden harness runs the tests.

Implement:

- `bool balanced(const std::string& text)`: return `true` if and only if all round, square, and curly brackets in `text` are correctly paired and nested.

`text` contains only the six characters `(`, `)`, `[`, `]`, `{`, `}`, and may be empty. Do not write `main`.

Hint: push every opening bracket; on a closing bracket check the top of the stack for the matching type, and require an empty stack at the end. Use `std::vector<char>` or `std::stack<char>`.
