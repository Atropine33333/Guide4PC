This exercise reads no stdin and writes no stdout. Implement a palindrome check; a hidden harness runs the tests.

```cpp
bool is_palindrome(const std::string& text);
```

Rules:

- Consider only ASCII letters and digits, skipping every other character (spaces, punctuation, tabs);
- Compare letters case-insensitively;
- An empty string, or a string without any letters or digits, counts as a palindrome;
- Do not write `main`.

### Hint

Scan from both ends with two indices, skipping non-alphanumeric characters. When classifying a `char`, cast it to `unsigned char` before passing it to `<cctype>` functions.
