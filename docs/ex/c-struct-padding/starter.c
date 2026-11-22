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
    /* TODO: 输出 sizeof(struct A)、sizeof(struct B)、offsetof(struct A, i)、offsetof(struct B, d)。 */
    printf("0 0 0 0\n");
    return 0;
}
