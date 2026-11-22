#include <stddef.h>

char *dup_str(const char *s);
void free_str(char *s);

char *dup_str(const char *s) {
    /* TODO: 申请一块能放下 s 连同结尾 '\0' 的内存，把内容复制进去后返回。
       s 为 NULL 或内存不够时返回 NULL。 */
    (void)s;
    return NULL;
}

void free_str(char *s) {
    /* TODO: 归还 dup_str 申请到的内存。 */
    (void)s;
}
