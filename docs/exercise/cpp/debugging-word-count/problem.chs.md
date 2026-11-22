下面的程序应当统计输入中有多少个非空行，以及一共有多少个单词。单词之间用任意空白字符（空格或制表符）分隔，空行不计入行数。

```cpp
#include <iostream>
#include <string>

int main() {
    int lines = 0;
    int words = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        ++lines;
        words += 1;
        for (char ch : line) {
            if (ch == ' ') ++words;
        }
    }
    std::cout << lines << ' ' << words << '\n';
    return 0;
}
```

这份代码在连续空格、制表符、空行和首尾空白下都会给出错误结果。请修好它，使输出符合要求。

### 输入

若干行文本，直到 EOF。每行不超过 1000 个字符，总字符数不超过 100000。

### 输出

一行两个整数：非空行数与单词总数。

### 示例

输入：

```text
first line

second
```

输出：

```text
2 3
```

### 约束

- 单词与行都以空白字符为界，空行（只含空白字符的行）不计入行数。
- 输出只包含一行。
