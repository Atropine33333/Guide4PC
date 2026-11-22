#include <stddef.h>

#define HASH_CAP 16

typedef struct {
    int keys[HASH_CAP];
    unsigned char state[HASH_CAP]; /* 0 空，1 占用，2 已删 */
    size_t count;
} HashSet;

void hash_init(HashSet *s);
int hash_set(HashSet *s, int key);
int hash_get(const HashSet *s, int key);
int hash_remove(HashSet *s, int key);
size_t hash_size(const HashSet *s);

void hash_init(HashSet *s) {
    /* TODO: 全部槽位清空，计数归零。 */
    for (size_t i = 0; i < HASH_CAP; ++i) s->state[i] = 0;
    s->count = 0;
}

int hash_set(HashSet *s, int key) {
    /* TODO: 从 (unsigned)key % HASH_CAP 起线性探测，已存在返回 0，无处可放返回 -1。 */
    (void)s;
    (void)key;
    return -1;
}

int hash_get(const HashSet *s, int key) {
    /* TODO: 命中返回 1，否则返回 0。 */
    (void)s;
    (void)key;
    return 0;
}

int hash_remove(HashSet *s, int key) {
    /* TODO: 删掉返回 0，找不到返回 -1。 */
    (void)s;
    (void)key;
    return -1;
}

size_t hash_size(const HashSet *s) {
    /* TODO */
    (void)s;
    return 0;
}
