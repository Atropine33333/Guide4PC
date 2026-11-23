// 参考实现：c-fprintf-report
#include <stdio.h>

void report(FILE *f, int id, int score) {
    fprintf(f, "id=%d score=%d\n", id, score);
}
