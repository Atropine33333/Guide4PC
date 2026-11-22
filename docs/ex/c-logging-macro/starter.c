#include <stddef.h>

/* TODO: 定义 LOG 宏，把参数原样交给 log_write：
   LOG(buf, cap, level, ...) 展开成 log_write((buf), (cap), (level), __VA_ARGS__) 。 */

int log_write(char *buf, size_t cap, int level, const char *fmt, ...);

int log_write(char *buf, size_t cap, int level, const char *fmt, ...) {
    /* TODO: 按档位取前缀，拼上正文，写进 buf 并返回整行应有的长度。 */
    (void)level;
    (void)fmt;
    if (buf != NULL && cap > 0) buf[0] = '\0';
    return -1;
}
