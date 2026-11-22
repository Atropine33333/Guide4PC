// 参考实现：c-big-add
#include <stdio.h>
#include <string.h>

#define NUM_MAX 1000

int main(void) {
    char a[NUM_MAX + 1];
    char b[NUM_MAX + 1];
    if (scanf("%1000s", a) != 1) return 0;
    if (scanf("%1000s", b) != 1) return 0;

    int ia = (int)strlen(a) - 1;
    int ib = (int)strlen(b) - 1;

    char digits[NUM_MAX + 2];
    int pos = 0;
    int carry = 0;

    while (ia >= 0 || ib >= 0 || carry != 0) {
        int da = ia >= 0 ? a[ia--] - '0' : 0;
        int db = ib >= 0 ? b[ib--] - '0' : 0;
        int sum = da + db + carry;
        digits[pos++] = (char)('0' + sum % 10);
        carry = sum / 10;
    }

    while (pos > 1 && digits[pos - 1] == '0') {
        --pos;
    }

    for (int i = pos - 1; i >= 0; --i) {
        putchar(digits[i]);
    }
    putchar('\n');
    return 0;
}
