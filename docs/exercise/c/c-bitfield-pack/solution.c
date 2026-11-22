// 参考实现：c-bitfield-pack
#include <stddef.h>

#define FLAGS_WORD_MAX 0xFFu

typedef struct {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
} Flags;

void flags_init(Flags *f) {
    f->a = 0;
    f->b = 0;
    f->c = 0;
}

int flags_set(Flags *f, char which, unsigned value) {
    switch (which) {
        case 'a':
            if (value > 1u) return -1;
            f->a = value;
            return 0;
        case 'b':
            if (value > 3u) return -1;
            f->b = value;
            return 0;
        case 'c':
            if (value > 31u) return -1;
            f->c = value;
            return 0;
        default:
            return -1;
    }
}

unsigned flags_get(const Flags *f, char which) {
    switch (which) {
        case 'a': return (unsigned)f->a;
        case 'b': return (unsigned)f->b;
        case 'c': return (unsigned)f->c;
        default: return 0u;
    }
}

unsigned flags_pack(const Flags *f) {
    unsigned word = 0u;
    word |= (unsigned)f->a;
    word |= (unsigned)f->b << 1;
    word |= (unsigned)f->c << 3;
    return word;
}

int flags_unpack(Flags *f, unsigned word) {
    if (word > FLAGS_WORD_MAX) return -1;
    f->a = word & 1u;
    f->b = (word >> 1) & 3u;
    f->c = (word >> 3) & 31u;
    return 0;
}
