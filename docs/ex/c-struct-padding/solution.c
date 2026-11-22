// 参考实现：c-struct-padding
#include <stddef.h>
#include <stdio.h>

struct A {
    char c;
    int i;
};

struct B {
    char c;
    int i;
    char d;
};

int main(void) {
    printf("%zu %zu %zu %zu\n",
           sizeof(struct A),
           sizeof(struct B),
           offsetof(struct A, i),
           offsetof(struct B, d));
    return 0;
}
