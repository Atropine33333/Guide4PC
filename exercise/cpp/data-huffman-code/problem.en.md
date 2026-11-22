Build a Huffman code for one line of lowercase text, then print the encoded bit string and each character's code.

### Input

One line containing only lowercase letters `a` through `z`, with length at most 1000. The input may be empty.

### Output

The first line is the encoded bit string (only `0` and `1`, possibly empty).

Then print one line per distinct character in ASCII order, in the form `<character> <code>`.

### Construction rules

Use this deterministic Huffman construction; otherwise equal frequencies could produce a different answer:

- Each leaf stores `(frequency, smallest character in its subtree)`.
- Repeatedly pop the two smallest nodes by the lexicographic key `(frequency, smallest character)`. The first popped node is the left child and the second is the right child.
- An internal node has the summed frequency and the smallest character of its left child (the left key is no greater than the right key).
- The edge to a left child is `0`; the edge to a right child is `1`.
- If the whole tree is one leaf, that character's code is `0`.

### Example

Input:

```text
aabbbcccc
```

Output:

```text
1010111110000
a 10
b 11
c 0
```

### Constraints

- For empty input, print just one empty line.
- Use only the standard library; do not use files, exceptions, or threads.
