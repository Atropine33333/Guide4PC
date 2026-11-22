A: Take a look at this one — "A man, a plan, a canal: Panama".

B: Look at what?

A: Whether it reads the same backwards.

B: Oh, a palindrome, I know the type: same front to back. But this one has commas, a colon and spaces in it. How do you count that?

A: Pay them no mind. Keep the ASCII letters and digits, treat everything else as absent, and do not distinguish case — `A` and `a` are of one house.

B: And if the whole string has not a letter nor a digit in it?

A: Still a palindrome. So is the empty string.

B: A generous rule. Mind you, there is a pit when you write it: cast the `char` to `unsigned char` before asking `<cctype>`, or those functions will sulk on high-bit characters.

**In plain words**: implement `is_palindrome(text)`, which skips non-alphanumeric characters and ignores case before deciding whether the whole string reads the same both ways.

```cpp
bool is_palindrome(const std::string& text);
```

**Rules**:

- Only ASCII letters and digits count; every other character (spaces, punctuation, tabs) is skipped;
- Letter comparison ignores case;
- An empty string, or a string with no letters and digits at all, counts as a palindrome;
- Do not write `main`.

### Hint

Scan from both ends towards the middle with two indices, skipping any character that is not alphanumeric. When classifying, cast the `char` to `unsigned char` before passing it to the `<cctype>` functions.
