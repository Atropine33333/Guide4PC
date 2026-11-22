#include <assert.h>
#include <stdio.h>

/* TODO: 在文件作用域加一条 _Static_assert，核对 sizeof(int) == 4。 */

int main(void) {
    int n = 0;
    if (scanf("%d", &n) != 1) return 0;

    printf("n=%d int=%d check=unchecked\n", n, (int)sizeof(int));
    return 0;
}
