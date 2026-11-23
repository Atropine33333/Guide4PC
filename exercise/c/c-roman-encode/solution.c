// 参考实现：c-roman-encode
#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 3999) return 0;

    static const struct {
        int value;
        const char *symbol;
    } table[] = {
        {1000, "M"},  {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"},   {90, "XC"},  {50, "L"},  {40, "XL"},
        {10, "X"},    {9, "IX"},   {5, "V"},   {4, "IV"},
        {1, "I"}
    };
    int count = (int)(sizeof table / sizeof table[0]);

    for (int i = 0; i < count; ++i) {
        while (n >= table[i].value) {
            fputs(table[i].symbol, stdout);
            n -= table[i].value;
        }
    }
    putchar('\n');
    return 0;
}
