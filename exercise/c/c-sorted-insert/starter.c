#include <stddef.h>

#define ARR_CAP 64

typedef struct {
    int data[ARR_CAP];
    int size;
} SortedArray;

void sa_init(SortedArray *a);
int sa_insert(SortedArray *a, int value);
int sa_size(const SortedArray *a);

void sa_init(SortedArray *a) {
    /* TODO: 置空。 */
    (void)a;
}

int sa_insert(SortedArray *a, int value) {
    /* TODO: 插到合适位置让数组保持非递减；满了返回 -1。 */
    (void)a;
    (void)value;
    return -1;
}

int sa_size(const SortedArray *a) {
    /* TODO: 返回元素个数。 */
    (void)a;
    return 0;
}
