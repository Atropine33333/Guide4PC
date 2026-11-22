#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 100
#define NAME_MAX 24

typedef struct {
    char name[NAME_MAX];
    int score;
} Student;

static int compare_student(const void *lhs, const void *rhs) {
    /* TODO: 分数高的在前；同分时姓名按 strcmp 从小到大。 */
    (void)lhs;
    (void)rhs;
    return 0;
}

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    Student students[MAX_N];
    for (int i = 0; i < n; ++i) {
        if (scanf("%23s %d", students[i].name, &students[i].score) != 2) return 0;
    }

    qsort(students, (size_t)n, sizeof students[0], compare_student);

    for (int i = 0; i < n; ++i) {
        printf("%s %d\n", students[i].name, students[i].score);
    }
    return 0;
}
