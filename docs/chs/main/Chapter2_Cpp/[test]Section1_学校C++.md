# S1：学校 C++（试验页）

这一页用来验证练习格式：题面留在 markdown 里，初始代码和测试用例放进 `exercises/count-digit-occurrences/`，由构建脚本和运行器读取。

## 第一题：数字出现计数

输入一个非负整数和一个数字（0–9），统计这个数字在该整数中出现的次数。

**输入**：两行。第一行是一个非负整数，第二行是一个数字。

**输出**：一个整数，表示出现的次数。

例如，输入 `676767` 和 `6`，输出 `3`：

```text
676767
6
3
```

:::exercise{id="count-digit-occurrences"}
:::

### 扩展：测试机没有 `<string>`

在正常的 C++ 实践里，用 `std::string` 把整数转成字符串再逐位比较，是毫无问题的写法。但有些学校测试机提供的头文件里没有 `<string>`，这道扩展题就是为此准备的：不许用字符串库，也要能逐位统计。

这个变体在 `exercises/count-digit-occurrences/spec.json` 里声明，要求源码中不出现 `<string>` 与 `std::string`。

:::exercise{id="count-digit-occurrences" variant="no-string"}
:::

## 第二题：环形缓冲区（harness 题）

这一题不读 stdin，也不写 stdout。你只需要实现一个类，测试逻辑放在隐藏的 harness 里：构建时 harness 会拼在你的代码后面，直接调用 `push`、`pop` 和 `size`。

实现 `CircularBuffer`，容量在构造时固定：

- `push(value)`：没满就写入并返回 `true`，满了返回 `false`；
- `pop(out)`：不为空就把最早写入的值取到 `out` 并返回 `true`，为空返回 `false`；
- `size()` / `capacity()`：返回当前元素数和总容量。

不要写 `main`：harness 会提供程序入口。初始代码只是能编译的占位实现，所有检查都会失败；把成员和方法补全即可。

:::exercise{id="circular-buffer"}
:::
