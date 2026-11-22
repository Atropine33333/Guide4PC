The kingdom of heaven is like a scribe who weighs letters. In his hand is one line of lowercase letters from `a` to `z`: some come often, some but once. He sets each letter on the scale by the number of its comings: few comings are light, many comings are heavy.

Each time he takes the two lightest from the scale and joins them into one, whose weight is the sum of the two. When two on the scale weigh the same, he first takes the one whose subtree holds the smaller letter and sets it on the left, and the other on the right. So he joins and joins again, until one thing alone remains on the scale.

Then he ties cords to it: the cord from a father to its left child is marked `0`, and the cord to its right child `1`. A letter's code is what one sees of `0` and `1` while walking down from the top along the cords. If but one letter stood on the scale from the beginning, its code is `0`. In the kingdom's account, every light and every heavy is reckoned plainly.

**In plain words**: read one line of lowercase text (possibly empty) and compress it with Huffman coding. Count each character's occurrences; with a min-heap keyed by `(frequency, smallest character in the subtree)`, repeatedly pop the two smallest nodes and join them, the first popped as the left child and the second as the right child, until a single root remains; append `0` for a left child and `1` for a right child. Print the encoded bit string, then one line per distinct character in ASCII order as `<character> <code>`. The tie-breaking is fixed by the construction rules below, so follow them exactly.

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
