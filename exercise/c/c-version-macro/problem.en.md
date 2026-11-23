A software version is written as `major.minor.patch`, with one dot between neighbouring parts. The three numbers come from the input, and the assembly is left to a macro: the format string is defined once as a macro and applied to all three integers in a single print.

**Input**: one line with three integers `major`, `minor` and `patch`, each between `0` and `99`.

**Output**: one line holding a string of the form `major.minor.patch`, with one dot between neighbouring parts.

### Example

Input:

```text
10 20 30
```

Output:

```text
10.20.30
```

### Contract

- The version string is assembled from the macro `VERSION_FMT`, applied once to the three integers.
- Each part is printed in decimal as it is, with no leading zeros added.
- Print only after all three parts have been read.
- The line ends with a newline and no trailing spaces.
