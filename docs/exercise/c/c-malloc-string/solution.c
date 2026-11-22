// 参考实现：c-malloc-string
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *dup_str(const char *s);
void free_str(char *s);

char *dup_str(const char *s) {
    if (s == NULL) return NULL;
    size_t n = strlen(s);
    char *p = (char *)malloc(n + 1);
    if (p == NULL) return NULL;
    memcpy(p, s, n + 1);
    return p;
}

void free_str(char *s) {
    free(s);
}
