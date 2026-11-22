这一题不读 stdin。实现 CSV 一行的写出与解析，隐藏的 harness 会调用。

```cpp
#include <string>
#include <vector>

std::string csv_quote(const std::string& field);
std::vector<std::string> csv_split(const std::string& line);
```

### 写出规则（`csv_quote`）

- 如果字段不含逗号 `,`、双引号 `"` 和换行 `\n` 中的任何一个，原样返回。
- 否则给字段加上一对双引号，并把字段内原有的每个双引号写成两个双引号。

### 解析规则（`csv_split`）

- 逗号分隔字段；两个逗号之间的内容（可以为空）就是一个字段。
- 以双引号开头的字段进入引号模式：引号内的逗号不作为分隔符；连续两个双引号表示一个真正的双引号。
- 空行解析出一个空字段；行尾的逗号解析出一个空字段。
- 保证输入是 `csv_quote` 能产生的合法格式，不需要处理未闭合的引号。

### 示例

| 调用 | 结果 |
| --- | --- |
| `csv_quote("abc")` | `abc` |
| `csv_quote("a,b")` | `"a,b"` |
| `csv_quote("say \"hi\"")` | `"say ""hi"""` |
| `csv_split("a,b,c")` | 三个字段 `a`、`b`、`c` |
| `csv_split("\"a,b\",c")` | 两个字段 `a,b`、`c` |
| `csv_split("a,")` | 两个字段 `a`、空串 |

### 提示

写出时先判断“是否需要引号”，再逐字符拼接；解析时用一个 `in_quotes` 状态，遇到引号先看下一个字符是不是引号。
