#include <stddef.h>

#define BITSET_MAX_BITS 256

typedef struct {
    unsigned char words[BITSET_MAX_BITS / 8];
    int nbits;
} BitSet;

void bitset_init(BitSet *b, int nbits);
int bitset_set(BitSet *b, int i);
int bitset_clear(BitSet *b, int i);
int bitset_test(const BitSet *b, int i);
int bitset_count(const BitSet *b);

void bitset_init(BitSet *b, int nbits) {
    /* TODO: 记下位数，并把所有字节清空。 */
    (void)b;
    (void)nbits;
}

int bitset_set(BitSet *b, int i) {
    /* TODO: 越界返回 -1，否则把第 i 位置 1 并返回 0。 */
    (void)b;
    (void)i;
    return -1;
}

int bitset_clear(BitSet *b, int i) {
    /* TODO: 越界返回 -1，否则把第 i 位清 0 并返回 0。 */
    (void)b;
    (void)i;
    return -1;
}

int bitset_test(const BitSet *b, int i) {
    /* TODO: 置位返回 1，未置位返回 0，越界返回 -1。 */
    (void)b;
    (void)i;
    return -1;
}

int bitset_count(const BitSet *b) {
    /* TODO: 返回 [0, nbits) 中置位的个数。 */
    (void)b;
    return 0;
}
