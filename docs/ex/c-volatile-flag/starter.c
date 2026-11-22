volatile int flag = 0;

void set_flag(void);
void clear_flag(void);
int wait_until_set(int limit);

void set_flag(void) {
    /* TODO: 把灯点亮。 */
}

void clear_flag(void) {
    /* TODO: 把灯熄掉。 */
}

int wait_until_set(int limit) {
    /* TODO: 最多看 limit 眼；某一眼看到 flag 非 0 就返回 1，
       看满 limit 眼仍为 0 就返回 0。每一眼都要重新读 flag。 */
    (void)limit;
    return 0;
}
