这一题不读 stdin。实现定宽列的切片与解析函数。

```cpp
#include <string>

struct Person {
    std::string name;
    int id;
    int score;
};

std::string slice(const std::string& line, int start, int width);
Person parse_person(const std::string& line);
```

每一行的列布局是固定的：

| 字段 | 起始列 | 宽度 |
| --- | --- | --- |
| `id` | 0 | 4 |
| `name` | 4 | 10 |
| `score` | 14 | 6 |

约定：

- `slice` 返回从第 `start` 列开始、最多 `width` 个字符组成的子串，并去掉首尾的 ASCII 空格。
- 如果 `start < 0`、`width <= 0` 或 `start` 已经到达行尾，`slice` 返回空串。
- 行可能短于 20 个字符：缺失的列按空白处理。
- `parse_person` 用 `slice` 取出三个字段；`id` 或 `score` 为空白时按 `0` 处理；`name` 保留内部空格并去掉首尾空格。
- 不要写 `main`。

### 提示

可以先用 `substr` 截取，再手动跳过首尾空格。数字字段可能带前导空格，但内容保证是合法的十进制整数（或者为空）。
