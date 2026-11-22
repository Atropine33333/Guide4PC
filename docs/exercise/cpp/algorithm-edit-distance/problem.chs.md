**第十八章**

天国好像一个人有两个词，一个叫 `a`，一个叫 `b`，彼此相欠。主人说：“把 `a` 改作 `b`，改一字如还一钱；添一字、删一字、换一字，也各算一钱。你且算算，最少还几钱。”

那人便一字一字地比，把账记在纸上，横竖各列一行。主人又问：“这两个词共有的字，按原来的次序能留多少？”那人说：“那是另一本账，也一并算来。”

**人话**：实现两个动态规划函数：`edit_distance` 求把 `a` 变成 `b` 的最少单字操作数（插入、删除、替换各算一步），`longest_common_subsequence` 求两个字符串最长公共子序列的长度。不要写 `main`。

需要实现的接口（不要写 `main`）：

```cpp
int edit_distance(const std::string& a, const std::string& b);
int longest_common_subsequence(const std::string& a, const std::string& b);
```

`edit_distance` 返回把 `a` 变成 `b` 的最少操作次数，每次可以插入一个字符、删除一个字符或替换一个字符。`longest_common_subsequence` 返回两个字符串最长公共子序列的长度。

### 输入

无。

### 输出

无。

### 示例

```cpp
edit_distance("kitten", "sitting") == 3
longest_common_subsequence("kitten", "sitting") == 4
```

`kitten` 先替换 `k` 为 `s`，再把 `e` 替换为 `i`，最后追加 `g`，共 3 步。

### 约束

- 两个字符串长度都不超过 200。
- 字符串只含小写英文字母。
- 固定种子生成的随机字符串对用来检查对称性、三角不等式等性质，实现必须是确定性的。
