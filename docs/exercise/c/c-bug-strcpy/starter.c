#include <stdio.h>
#include <string.h>

#define LABEL_CAP 24

int label_make(char *out, int cap, const char *name, const char *code);
int label_quote(char *out, int cap, const char *text);

int label_make(char *out, int cap, const char *name, const char *code) {
    (void)cap; /* 坏在这里：容量根本没被看过 */
    strcpy(out, name);
    strcat(out, "#");
    strcat(out, code);
    return (int)strlen(out);
}

int label_quote(char *out, int cap, const char *text) {
    (void)cap;
    out[0] = '[';
    strcpy(out + 1, text);
    strcat(out, "]");
    return (int)strlen(out);
}
