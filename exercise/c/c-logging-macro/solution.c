// 参考实现：c-logging-macro
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define LOG(buf, cap, level, ...) log_write((buf), (cap), (level), __VA_ARGS__)

int log_write(char *buf, size_t cap, int level, const char *fmt, ...);

static void level_head(char *head, size_t size, int level) {
    if (level == 0) {
        snprintf(head, size, "[DEBUG] ");
    } else if (level == 1) {
        snprintf(head, size, "[INFO] ");
    } else if (level == 2) {
        snprintf(head, size, "[WARN] ");
    } else if (level == 3) {
        snprintf(head, size, "[ERROR] ");
    } else {
        snprintf(head, size, "[LVL%d] ", level);
    }
}

int log_write(char *buf, size_t cap, int level, const char *fmt, ...) {
    char head[32];
    va_list ap;
    va_list copy;
    int head_len;
    int body;
    int total;

    if (buf == NULL || cap == 0 || fmt == NULL) return -1;

    level_head(head, sizeof head, level);
    head_len = (int)strlen(head);

    va_start(ap, fmt);
    va_copy(copy, ap);
    body = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    if (body < 0) {
        va_end(copy);
        buf[0] = '\0';
        return -1;
    }

    total = head_len + body;
    if ((size_t)head_len < cap) {
        memcpy(buf, head, (size_t)head_len);
        vsnprintf(buf + head_len, cap - (size_t)head_len, fmt, copy);
    } else {
        memcpy(buf, head, cap - 1);
        buf[cap - 1] = '\0';
    }
    va_end(copy);
    return total;
}
