// 参考实现：c-qsort-words
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 200
#define MAX_LEN 20

static int cmp_words(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > MAX_WORDS) return 0;

    static char words[MAX_WORDS][MAX_LEN + 1];
    for (int i = 0; i < n; ++i) {
        if (scanf("%20s", words[i]) != 1) return 0;
    }

    qsort(words, (size_t)n, sizeof words[0], cmp_words);

    for (int i = 0; i < n; ++i) {
        puts(words[i]);
    }
    return 0;
}
