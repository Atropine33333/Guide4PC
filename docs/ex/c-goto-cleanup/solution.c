// 参考实现：c-goto-cleanup
#include <stddef.h>
#include <stdlib.h>

char *dup_n(const char *s, int n) {
    char *out = NULL;
    int k = 0;

    if (s == NULL || n < 0) goto cleanup;

    out = (char *)malloc((size_t)n + 1);
    if (out == NULL) goto cleanup;

    while (k < n && s[k] != '\0') {
        out[k] = s[k];
        ++k;
    }
    out[k] = '\0';
    return out;

cleanup:
    free(out);
    return NULL;
}
