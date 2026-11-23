（旧书店后间，两张桌子并排，各摊着一份书目。）

甲：这份书目上写着书名，也压着价钱。我想照着抄一份带走，抄完各改各的，行不行？

乙：抄的时候别只抄地址。书名那几个字在别处，你把那一处的地址誊过去，两份书目就指着同一行字了。

甲：那我这份改了书名，你那份不也跟着变？

乙：正是。你得另找一张纸，把书名一个字一个字重新写一遍，两份各指各的纸。

甲：那两份各自收摊呢？

乙：谁的纸谁收。先收哪一份都不碍着另一份。

**人话**：`Player` 里有一个 `char *name` 和一个 `int score`。`player_new` 为结构体和名字各申请一块内存，把名字复制进去；`player_clone` 做深拷贝，副本的 `name` 指向新内存；`player_free` 先归还名字，再归还结构体。

**接口**：

```c
typedef struct {
    char *name;
    int score;
} Player;

Player *player_new(const char *name, int score);
Player *player_clone(const Player *src);
void player_free(Player *p);
```

- `player_new` 要求 `name` 不是 `NULL`；名字按 `strlen(name) + 1` 字节复制。任一申请失败返回 `NULL`。
- `player_new(NULL, score)` 返回 `NULL`。
- `player_clone(NULL)` 返回 `NULL`；副本与 `src` 是两个结构体，`name` 也指向两块不同的内存，内容相同。
- `player_free(NULL)` 什么也不做；每个指针只归还一次。
- 释放其中一份，另一份仍可照常读取和使用。

### 示例

```c
Player *a = player_new("Robin", 42);
Player *b = player_clone(a);   /* b->name 指向另一块内存 */
b->name[0] = 'X';              /* a->name 仍是 "Robin" */
player_free(a);
player_free(b);
```

### 约定

- 只实现这三个函数，不要写 `main`。
- 使用 `malloc`、`free`，不使用固定大小的全局缓冲区。
- 结构体里的 `name` 与调用者传入的缓冲区不重叠；调用者改了自己的缓冲区，存下的名字不变。
- 空串也要复制，得到的是一个只含 `'\0'` 的块。
