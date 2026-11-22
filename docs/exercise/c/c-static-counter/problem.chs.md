（醒木一拍，台上只一张桌、一本账。）

说书人：街口有间小账房，柜台后头坐着位先生。先生面前只有一本账，谁来都翻这一本。

看客：进账怎么记？

说书人：来一位客人，先生把账上添一，报出添完的数。退一笔账，先生从账上减去这个数，也报剩下的数。有人只问不记，先生就报眼下的数，账不动。

看客：要重新开张呢？

说书人：说一声“清”，账归零，从头记起。四位伙计轮着来，写的仍是同一本账。

**人话**：实现四个函数，共用一个 `static` 计数器。计数器在程序开始时为 `0`：`counter_next` 加一并返回新值，`counter_add` 加上 `delta` 并返回新值，`counter_get` 返回当前值且不改动它，`counter_reset` 把它归零。

**接口**：

```c
void counter_reset(void);
int counter_next(void);
int counter_add(int delta);
int counter_get(void);
```

- `counter_reset`：把计数器置为 `0`。
- `counter_next`：计数器加 `1`，返回加完后的值。
- `counter_add`：计数器加上 `delta`（可正、可负、可为零），返回加完后的值。
- `counter_get`：返回当前值，不做任何改动。

### 示例

计数器初始为 `0`：`counter_get()` 返回 `0`；调用两次 `counter_next()` 后为 `2`；`counter_add(-5)` 后为 `-3`；`counter_reset()` 之后 `counter_get()` 又返回 `0`。

### 约定

- 用一个文件作用域的 `static int` 保存计数，四个函数读写同一份状态。
- 计数器在程序开始时为 `0`，调用之间一直保留。
- `delta` 可以是正数、负数或 `0`，运算不会溢出。
- 只实现这四个函数，不要写 `main`，也不要输出题面之外的文字。
