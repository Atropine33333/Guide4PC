The program below should count how many non-empty lines the input contains and how many words there are in total. Words are separated by arbitrary whitespace (spaces or tabs), and blank lines do not count as lines.

```cpp
#include <iostream>
#include <string>

int main() {
    int lines = 0;
    int words = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        ++lines;
        words += 1;
        for (char ch : line) {
            if (ch == ' ') ++words;
        }
    }
    std::cout << lines << ' ' << words << '\n';
    return 0;
}
```

This code produces wrong results for repeated spaces, tabs, blank lines, and leading or trailing whitespace. Fix it so the output matches the requirements.

### Input

Several lines of text until EOF. Each line is at most 1000 characters and the total is at most 100000 characters.

### Output

One line with two integers: the number of non-empty lines and the total number of words.

### Example

Input:

```text
first line

second
```

Output:

```text
2 3
```

### Constraints

- Words and lines are delimited by whitespace; a blank line (only whitespace) is not counted.
- The output is exactly one line.
