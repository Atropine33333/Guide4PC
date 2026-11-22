#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 3999) return 0;

    /* TODO: 把 1000、900、500、400、100、90、50、40、10、9、5、4、1
       与对应的 M、CM、D、CD、C、XC、L、XL、X、IX、V、IV、I 列成一张表，
       从大到小试：n 够减就写一遍、减一遍，直到 n 归零。 */

    putchar('\n');
    return 0;
}
