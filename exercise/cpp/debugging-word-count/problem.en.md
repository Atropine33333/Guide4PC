Oh, my dear, do look at this program.

It means to count two things: the lines that are not empty, and all the words. Instead it treats every newline as a line and every single space as a word — a run of spaces becomes a run of empty words. Good heavens, that little passage holds three words, and it insists on five.

You see, what stands between two words is whitespace: a space, a tab, one of them or a whole string of them, each counts as the same wall. Whitespace at the start or the end of a line is not a word. A blank line — a line of nothing but whitespace — is not a line.

As for the loop, read line by line until the end. For each line, first look whether it holds any non-whitespace character; only then count it as a line. As for words, split on whitespace and count the pieces.

Oh, and one more thing, my dear: the output is a single line with two integers and one space between them. Not more, not less.

**In plain words**: read lines until EOF and count the non-empty lines and the total number of words. Words are separated by arbitrary whitespace (spaces or tabs), and a run of whitespace separates them only once; a line holding nothing but whitespace does not count. Fix the starter's habit of counting spaces as words and every line as a line, and print two integers on one line.

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
