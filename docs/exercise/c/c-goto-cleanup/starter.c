#include <stddef.h>
#include <stdlib.h>

char *dup_n(const char *s, int n) {
    char *out = NULL;
    int k = 0;

    if (s == NULL || n < 0) goto cleanup;

    /* TODO: 申请 n + 1 字节；把至多 n 个字符抄进新内存，遇到源串的结束符就停；
       末尾补 '\0'；返回 out。 */

    (void)k;

cleanup:
    free(out);
    return NULL;
}
