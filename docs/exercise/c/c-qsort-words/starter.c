#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 200
#define MAX_LEN 20

int cmp_words(const void *a, const void *b) {
    /* TODO: 用 strcmp 比较两个单词。 */
    (void)a;
    (void)b;
    return 0;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 0 || n > MAX_WORDS) return 0;

    static char words[MAX_WORDS][MAX_LEN + 1];
    for (int i = 0; i < n; ++i) {
        if (scanf("%20s", words[i]) != 1) return 0;
    }

    /* TODO: 用 qsort 把 words 里的 n 个单词排好序。 */

    for (int i = 0; i < n; ++i) {
        puts(words[i]);
    }
    return 0;
}
