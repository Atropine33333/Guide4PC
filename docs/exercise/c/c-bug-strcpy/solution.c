// 参考实现：c-bug-strcpy
#include <stdio.h>

#define LABEL_CAP 24

int label_make(char *out, int cap, const char *name, const char *code) {
    if (cap <= 0) return 0;
    int written = snprintf(out, (size_t)cap, "%s#%s", name, code);
    if (written < 0) {
        out[0] = '\0';
        return 0;
    }
    if (written > cap - 1) written = cap - 1;
    return written;
}

int label_quote(char *out, int cap, const char *text) {
    if (cap <= 0) return 0;
    int written = snprintf(out, (size_t)cap, "[%s]", text);
    if (written < 0) {
        out[0] = '\0';
        return 0;
    }
    if (written > cap - 1) written = cap - 1;
    return written;
}
