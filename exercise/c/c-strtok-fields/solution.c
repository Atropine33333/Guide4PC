// 参考实现：c-strtok-fields
#include <stddef.h>
#include <string.h>

#define FIELD_MAX 32
#define FIELD_LEN 64

int split_fields(const char *line, char out[][FIELD_LEN], int max);

int split_fields(const char *line, char out[][FIELD_LEN], int max) {
    int count = 0;
    if (line == NULL || max <= 0) return 0;

    const char *p = line;
    for (;;) {
        const char *end = strchr(p, ',');
        if (end == NULL) end = p + strlen(p);

        const char *start = p;
        while (start < end && *start == ' ') ++start;
        while (end > start && end[-1] == ' ') --end;

        if (start < end) {
            if (count < max) {
                size_t len = (size_t)(end - start);
                if (len > FIELD_LEN - 1) len = FIELD_LEN - 1;
                memcpy(out[count], start, len);
                out[count][len] = '\0';
            }
            ++count;
        }

        if (*end == '\0') break;
        p = end + 1;
    }

    return count < max ? count : max;
}
