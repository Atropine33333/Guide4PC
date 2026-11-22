Guide4PC 练习司文件

练发〔2026〕7 号

关于加强文本统计缓存管理工作的通知

各实现单位：

　　为减少重复劳动、提高统计效率，现就 `TextStats` 类缓存管理事宜通知如下。

　　一、`word_count()` 与 `letter_count()` 均为常量成员函数。首次调用时须将结果写入缓存，故缓存成员一律以 `mutable` 声明。

　　二、文本未变时，重复调用只读缓存，`computations()` 不得增加；每填一次缓存，`computations()` 加一。

　　三、`append()` 修改文本后，两处缓存即行失效，下次查询重新计算。

　　四、只统计英文字母；数字、空格、标点一概不计。

　　五、文本总长不超过 100000 字符；线程安全不作要求。

　　请遵照执行。

**人话**：补全 `TextStats` 类：`word_count()` 与 `letter_count()` 是 `const` 成员，首次调用把结果写入 `mutable` 缓存，此后文本不变就直接返回缓存且不增加 `computations()`，每填一次缓存 `computations()` 加一；`append()` 改动文本后两处缓存失效，下次查询重新计算。单词按连续的英文字母划分。

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
