请实现两个字符串之间的动态规划函数。

需要实现的接口（不要写 `main`）：

```cpp
int edit_distance(const std::string& a, const std::string& b);
int longest_common_subsequence(const std::string& a, const std::string& b);
```

`edit_distance` 返回把 `a` 变成 `b` 的最少操作次数，每次可以插入一个字符、删除一个字符或替换一个字符。`longest_common_subsequence` 返回两个字符串最长公共子序列的长度。

### 输入

无。评测代码会直接调用这些函数。

### 输出

无。函数返回值由评测代码检查。

### 示例

```cpp
edit_distance("kitten", "sitting") == 3
longest_common_subsequence("kitten", "sitting") == 4
```

`kitten` 先替换 `k` 为 `s`，再把 `e` 替换为 `i`，最后追加 `g`，共 3 步。

### 约束

- 两个字符串长度都不超过 200。
- 字符串只含小写英文字母。
- 评测包含固定种子生成的随机字符串对，用来检查对称性、三角不等式等性质，实现必须是确定性的。
