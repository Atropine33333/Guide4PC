《会被改的灯》

第 3 观测站只有一盏状态灯。灯说灭就灭，说亮就亮，谁也保证不了它在两瞥之间是同一个样子。

值班员的做法很朴素：先给自己定一个最多看几眼的次数 `limit`，然后一眼一眼看过去。哪一眼看见灯亮着，就记 `1` 并收工；看满了 `limit` 眼灯还灭着，就记 `0`。灯随时可能被别处改掉，所以每一眼都得重新看那盏灯，不能拿第一眼记住的样子代替。

**接口**：

```c
volatile int flag;          /* 灯，初始为 0 */

void set_flag(void);
void clear_flag(void);
int wait_until_set(int limit);
```

- `set_flag` 把 `flag` 置为 `1`，`clear_flag` 把 `flag` 置为 `0`。
- `wait_until_set` 至多看 `limit` 眼：某一眼发现 `flag` 非 `0` 就返回 `1`；看满 `limit` 眼仍是 `0` 就返回 `0`。
- `limit ≤ 0` 时一眼也不看，直接返回 `0`。

**人话**：实现 `set_flag`、`clear_flag` 和 `wait_until_set`。`wait_until_set(limit)` 最多循环 `limit` 次，每次检查全局变量 `flag`；看到它非 `0` 返回 `1`，否则返回 `0`。

### 示例

```c
clear_flag();
wait_until_set(5)   // 灯一直灭着，看满 5 眼，返回 0
set_flag();
wait_until_set(1)   // 第一眼就亮着，返回 1
clear_flag();
wait_until_set(0)   // 一眼也不看，返回 0
```

### 约定

- 只实现这三个函数，不要写 `main`；全局变量 `flag` 已在起点代码里定义并初始化为 `0`，保持它全局可见。
- `flag` 带着 `volatile`，每一次循环都要重新读它，不要先抄进局部变量再判断。
- `limit` 可能为 `0` 或负数，此时循环一次也不该执行。
- 函数必须会结束，不能因为灯没亮就一直转下去。
