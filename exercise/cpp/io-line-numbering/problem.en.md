> Lines of writing, unnumbered still,
> count them first, and know the fill;
> width once fixed by digit count,
> then number each without a fault.

The *Nine Chapters* has a rule: to number the lines, count the lines first; once the width is fixed, numbering may begin.

A bundle of writing lies before you, its number of lines unknown until the scroll ends. How shall it be numbered?

Answer: let `n` be the total number of lines, empty lines included, and let `w` be the number of decimal digits in `n`. Line `k`, counted from 1, is written as the number right-aligned in width `w` with spaces on the left, then one bar `|` with no space after it, then the original line copied exactly, spaces at either end untouched. An empty line prints its number and `|`, then ends. If the scroll holds not one line, write nothing.

Rule: read to the end before writing. The text may or may not end with a newline; a final segment without one still counts as a line. No line exceeds 200 characters, and there are at most 1000. You cannot write first and mend the numbers later: without the total, the width cannot be known.

**In plain words**: read to EOF collecting every line, find the total `n` and its digit count `w`, then print each line as "number right-aligned in `w`, one `|`, the original line"; empty input prints nothing.

### Output format

Let `n` be the total number of lines, including empty lines. Let `w` be the number of decimal digits in `n`. For line `k` (starting at 1):

```text
<line number right-aligned in width w>|<original line>
```

Pad the line number with spaces on the left. Do not put a space after `|`. Reproduce the original line exactly, including leading and trailing spaces. An empty line prints its number and `|`, then ends immediately.

If the input is empty (zero lines), print nothing.

### Example

Input:

```text
alpha

gamma
```

There are 3 lines and the second one is empty, so `w = 1`. Output:

```text
1|alpha
2|
3|gamma
```

### Constraints

- The input may or may not end with a newline; a final segment without a newline still counts as a line.
- Each line has at most 200 characters and there are at most 1000 lines.
- Lines contain only printable ASCII characters and spaces; no tabs or `\r`.
- You cannot print first and fix the numbers later: the total is needed to choose the width.
