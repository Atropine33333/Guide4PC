#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *p;
    size_t len;
    size_t cap;
} Buf;

void buf_init(Buf *b);
int buf_append(Buf *b, const char *s);
void buf_free(Buf *b);

void buf_init(Buf *b) {
    /* TODO: 空布：没有内存，长度与容量都是 0。 */
    b->p = NULL;
    b->len = 0;
    b->cap = 0;
}

int buf_append(Buf *b, const char *s) {
    /* TODO: 把 s 整段接在末尾，必要时扩容，末尾留一个 '\0'。 */
    (void)b;
    (void)s;
    return -1;
}

void buf_free(Buf *b) {
    /* TODO: 还回内存，并把三个字段清零。 */
    (void)b;
}
