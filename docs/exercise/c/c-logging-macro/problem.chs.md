写日志像记账：每条记一行，前面用方括号记下档位，后面记正文。正文里的数字和字符串由调用方临时给，所以拼装函数要接变长参数，再用一个宏把调用写短。

请实现 `log_write` 并补上 `LOG` 宏。

**接口**：

```c
#include <stddef.h>

#define LOG(buf, cap, level, ...) /* 展开成对 log_write 的一次调用 */

int log_write(char *buf, size_t cap, int level, const char *fmt, ...);
```

- `LOG` 把 `buf`、`cap`、`level` 与 `__VA_ARGS__` 原样交给 `log_write`。
- `log_write` 把档位名放进方括号、后面跟一个空格，再接上按 `fmt` 格式化出来的正文，写进 `buf`，并以空字符收尾。
- 档位 `0` 到 `3` 依次写成 `DEBUG`、`INFO`、`WARN`、`ERROR`；其他整数 `n` 写成 `LVLn`。
- 返回整行应有的长度，不含收尾的空字符。即使装不下，返回值也按完整长度算。
- `buf` 为空、`cap` 为 `0`、`fmt` 为空时返回 `-1`，此时不写 `buf`。
- 装不下时只写 `cap - 1` 个字符，再加空字符。

### 示例

```c
char buf[64];
LOG(buf, sizeof buf, 1, "start %d", 7);
// buf 里是 "[INFO] start 7"，返回 14

char small[10];
LOG(small, sizeof small, 1, "%s", "abcdefghij");
// 完整长度 17，装不下，small 里是 "[INFO] ab"，返回 17
```

### 约定

- 签名与宏名保持原样，`log_write` 是变长参数函数。
- 只使用标准库；`stdarg.h` 与 `stdio.h` 里的工具够用。
- 这些函数和宏会被直接调用，不要写 `main`。
- 不要用动态分配，临时缓冲区放在栈上。
