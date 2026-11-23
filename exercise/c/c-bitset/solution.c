// 参考实现：c-bitset
#include <stddef.h>
#include <string.h>

#define BITSET_MAX_BITS 256

typedef struct {
    unsigned char words[BITSET_MAX_BITS / 8];
    int nbits;
} BitSet;

void bitset_init(BitSet *b, int nbits) {
    b->nbits = nbits;
    memset(b->words, 0, sizeof b->words);
}

int bitset_set(BitSet *b, int i) {
    if (i < 0 || i >= b->nbits) return -1;
    b->words[i / 8] = (unsigned char)(b->words[i / 8] | (unsigned char)(1u << (i % 8)));
    return 0;
}

int bitset_clear(BitSet *b, int i) {
    if (i < 0 || i >= b->nbits) return -1;
    b->words[i / 8] = (unsigned char)(b->words[i / 8] & (unsigned char)~(1u << (i % 8)));
    return 0;
}

int bitset_test(const BitSet *b, int i) {
    if (i < 0 || i >= b->nbits) return -1;
    return ((b->words[i / 8] >> (i % 8)) & 1u) ? 1 : 0;
}

int bitset_count(const BitSet *b) {
    int total = 0;
    for (int i = 0; i < b->nbits; ++i) {
        if ((b->words[i / 8] >> (i % 8)) & 1u) {
            ++total;
        }
    }
    return total;
}
