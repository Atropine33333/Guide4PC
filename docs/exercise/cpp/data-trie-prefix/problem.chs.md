实现一棵支持前缀计数的小写字母前缀树。

```cpp
#include <string>

class Trie {
public:
    Trie();
    ~Trie();
    void insert(const std::string& word);
    int count_prefix(const std::string& prefix) const;
    int size() const;
    bool contains(const std::string& word) const;
};
```

规则：

- 只处理小写字母 `a`–`z` 组成的字符串；字符串可以为空。
- `insert` 插入一次就计一个单词，重复插入同一个单词要分别计数。
- `count_prefix(prefix)` 返回以 `prefix` 为前缀的单词个数（重复插入的也分别计数）；空前缀返回 `size()`。
- `contains(word)` 判断这个单词是否被插入过（至少一次）；空单词只有被插入过才返回 `true`。
- `size()` 返回插入的总次数。
- 必须正确释放节点，不能依赖进程退出来回收内存。
- 不要写 `main`。

### 示例

| 调用序列 | 结果 |
| --- | --- |
| `insert("a"); insert("ab"); insert("abc")` | `count_prefix("a")` 为 3，`count_prefix("ab")` 为 2 |
| `insert("x"); insert("x")` | `size()` 为 2，`count_prefix("x")` 为 2 |
| `insert("cat"); insert("car")` | `contains("ca")` 为 `false`，`count_prefix("ca")` 为 2 |

### 提示

节点里保存 26 个指向子节点的指针、一个“经过这里的单词数”和一个“在此结束的单词数”。析构函数递归释放子节点；拷贝构造和拷贝赋值可以声明为 `= delete`，避免两个对象释放同一批节点。
