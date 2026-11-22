// 参考实现：c-dyn-buffer
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *p;
    size_t len;
    size_t cap;
} Buf;

void buf_init(Buf *b) {
    b->p = NULL;
    b->len = 0;
    b->cap = 0;
}

static int buf_reserve(Buf *b, size_t need) {
    if (need <= b->cap) return 0;
    size_t cap = b->cap ? b->cap : 8;
    while (cap < need) {
        if (cap > (size_t)-1 / 2) {
            cap = need;
            break;
        }
        cap *= 2;
    }
    char *p = (char *)realloc(b->p, cap);
    if (p == NULL) return -1;
    b->p = p;
    b->cap = cap;
    return 0;
}

int buf_append(Buf *b, const char *s) {
    size_t n = strlen(s);
    if (buf_reserve(b, b->len + n + 1) != 0) return -1;
    memcpy(b->p + b->len, s, n + 1);
    b->len += n;
    return 0;
}

void buf_free(Buf *b) {
    free(b->p);
    b->p = NULL;
    b->len = 0;
    b->cap = 0;
}
