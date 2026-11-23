// 参考实现：c-string-dup
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char *dup_string(const char *s) {
    size_t len = strlen(s);
    char *copy = (char *)malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}
