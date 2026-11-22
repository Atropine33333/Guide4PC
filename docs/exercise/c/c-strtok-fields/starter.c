#include <stddef.h>
#include <string.h>

#define FIELD_MAX 32
#define FIELD_LEN 64

int split_fields(const char *line, char out[][FIELD_LEN], int max);

int split_fields(const char *line, char out[][FIELD_LEN], int max) {
    /* TODO: 按逗号切分，去掉每段首尾空格，跳过空段，写入 out 并返回字段数。 */
    (void)line;
    (void)out;
    (void)max;
    return 0;
}
