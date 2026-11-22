// 参考实现：c-hash-linear
#include <stddef.h>

#define HASH_CAP 16

typedef struct {
    int keys[HASH_CAP];
    unsigned char state[HASH_CAP]; /* 0 空，1 占用，2 已删 */
    size_t count;
} HashSet;

static size_t slot_of(int key) {
    return (size_t)((unsigned)key % HASH_CAP);
}

void hash_init(HashSet *s) {
    for (size_t i = 0; i < HASH_CAP; ++i) s->state[i] = 0;
    s->count = 0;
}

int hash_set(HashSet *s, int key) {
    size_t start = slot_of(key);
    size_t dead = HASH_CAP;
    for (size_t step = 0; step < HASH_CAP; ++step) {
        size_t i = (start + step) % HASH_CAP;
        if (s->state[i] == 1) {
            if (s->keys[i] == key) return 0;
        } else if (s->state[i] == 2) {
            if (dead == HASH_CAP) dead = i;
        } else {
            size_t at = (dead != HASH_CAP) ? dead : i;
            s->keys[at] = key;
            s->state[at] = 1;
            s->count += 1;
            return 0;
        }
    }
    if (dead != HASH_CAP) {
        s->keys[dead] = key;
        s->state[dead] = 1;
        s->count += 1;
        return 0;
    }
    return -1;
}

int hash_get(const HashSet *s, int key) {
    size_t start = slot_of(key);
    for (size_t step = 0; step < HASH_CAP; ++step) {
        size_t i = (start + step) % HASH_CAP;
        if (s->state[i] == 0) return 0;
        if (s->state[i] == 1 && s->keys[i] == key) return 1;
    }
    return 0;
}

int hash_remove(HashSet *s, int key) {
    size_t start = slot_of(key);
    for (size_t step = 0; step < HASH_CAP; ++step) {
        size_t i = (start + step) % HASH_CAP;
        if (s->state[i] == 0) return -1;
        if (s->state[i] == 1 && s->keys[i] == key) {
            s->state[i] = 2;
            s->count -= 1;
            return 0;
        }
    }
    return -1;
}

size_t hash_size(const HashSet *s) {
    return s->count;
}
