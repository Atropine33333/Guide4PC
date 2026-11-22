实现一个统计文本的类，并让常量成员函数也能缓存结果。补全下面的接口：

```cpp
class TextStats {
public:
    explicit TextStats(std::string text);

    void append(const std::string& extra);

    int word_count() const;     // 单词数：连续字母算一个单词
    int letter_count() const;   // 字母总数

    static int computations();  // 缓存被填充的累计次数

private:
    std::string text_;
    // 自行添加缓存成员（需要 mutable）和计数器
};
```

要求：

- `word_count()` 与 `letter_count()` 都是 `const` 成员函数，但第一次调用时要把结果写进缓存，因此缓存成员必须声明为 `mutable`。
- 只要文本没有变化，重复调用必须返回缓存值，且不增加 `computations()`；每填一次缓存，`computations()` 加一。
- `append()` 修改文本后，两个缓存都失效，下次查询重新计算。
- 只统计英文字母；数字、空格、标点都不算字母。

评测代码会直接包含你的实现并调用这些成员函数。**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
TextStats stats("hello world");
stats.word_count();   // 2，computations() 加一
stats.word_count();   // 仍是 2，computations() 不变
stats.append(" again");
stats.word_count();   // 3，缓存重新计算
```

### 约束

- 文本总长度不超过 100000。
- 不要求线程安全。
