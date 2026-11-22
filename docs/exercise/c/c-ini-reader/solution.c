// 参考实现：c-ini-reader
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100
#define KEY_MAX 32
#define VAL_MAX 64
#define LINE_MAX 200

typedef struct {
    char key[KEY_MAX];
    char val[VAL_MAX];
} Entry;

static void trim(char *s) {
    size_t len;
    char *start = s;

    while (*start == ' ' || *start == '\t') ++start;
    if (start != s) memmove(s, start, strlen(start) + 1);

    len = strlen(s);
    while (len > 0) {
        char ch = s[len - 1];
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') break;
        s[--len] = '\0';
    }
}

static int by_key(const void *a, const void *b) {
    const Entry *x = (const Entry *)a;
    const Entry *y = (const Entry *)b;
    return strcmp(x->key, y->key);
}

int main(void) {
    char line[LINE_MAX + 2];
    Entry table[MAX_ENTRIES];
    int count = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        char *eq;
        char *key;
        char *val;
        int slot = -1;

        trim(line);
        if (line[0] == '\0' || line[0] == '#') continue;

        eq = strchr(line, '=');
        if (eq == NULL) continue;
        *eq = '\0';

        key = line;
        val = eq + 1;
        trim(key);
        trim(val);
        if (key[0] == '\0') continue;

        if (strlen(key) >= KEY_MAX) key[KEY_MAX - 1] = '\0';
        if (strlen(val) >= VAL_MAX) val[VAL_MAX - 1] = '\0';

        for (int i = 0; i < count; ++i) {
            if (strcmp(table[i].key, key) == 0) {
                slot = i;
                break;
            }
        }
        if (slot < 0) {
            if (count >= MAX_ENTRIES) continue;
            slot = count;
            ++count;
        }
        strcpy(table[slot].key, key);
        strcpy(table[slot].val, val);
    }

    qsort(table, (size_t)count, sizeof table[0], by_key);
    for (int i = 0; i < count; ++i) {
        printf("%s=%s\n", table[i].key, table[i].val);
    }
    return 0;
}
