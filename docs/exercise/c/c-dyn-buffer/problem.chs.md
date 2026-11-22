还在为容量发愁？新一代自扩布料上市了！

你只管往里缝：头一针下去它只有巴掌大，越缝越长，每次不够就翻倍，接缝平整，收尾处永远留着一位安静的 `\0`。缝完一抖，`buf_free` 把整块布收走，不留线头；下一回再缝，又是一条崭新的布。

**人话**：`Buf` 是一个会自己长大的字符缓冲区。`buf_init` 把它置空；`buf_append` 把整段 C 字符串接到末尾，容量不够就扩容，并保证 `p` 一定以 `\0` 收尾、`cap` 至少为 `len + 1`；`buf_free` 归还内存，把 `p`、`len`、`cap` 全部清零。

**接口**：

```c
#include <stddef.h>

typedef struct {
    char *p;
    size_t len;
    size_t cap;
} Buf;

void buf_init(Buf *b);
int buf_append(Buf *b, const char *s);
void buf_free(Buf *b);
```

- `buf_init` 后：`p == NULL`、`len == 0`、`cap == 0`。
- `buf_append` 成功返回 `0`，`len` 增加被追加字符串的长度，`p[0..len-1]` 是新内容，`p[len]` 为 `'\0'`；内存不足返回 `-1`，缓冲区保持原样。
- `buf_free` 释放内存并把三个字段清零；对刚 `buf_init` 过的空缓冲区调用也安全。

### 示例

```c
Buf b;
buf_init(&b);
buf_append(&b, "ab");
buf_append(&b, "cde");
/* 此时 b.len == 5，b.p 是 "abcde"，b.cap >= 6 */
buf_free(&b);
/* 此时 b.p == NULL，b.len == 0，b.cap == 0 */
```

### 约定

- 只实现这些函数，不要写 `main`。
- 容量策略自由，只要求 `cap >= len + 1` 且分配失败时返回 `-1`。
- 追加空串不会改变内容。
- 结构体保持题面给出的字段，函数签名不变。
