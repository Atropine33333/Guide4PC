这一题不读 stdin，也不写 stdout。实现两个处理分隔字符串的函数，测试在隐藏的 harness 里。

```cpp
std::vector<std::string> split_fields(const std::string& text, char delimiter);
std::string join_fields(const std::vector<std::string>& fields,
                        const std::string& separator);
```

规则：

- `split_fields` 按单个字符 `delimiter` 拆分：每个分隔符都会产生一个新字段，连续的分隔符产生空字段。
- 空字符串拆成**空 vector**（不是含一个空串的 vector）。
- 字符串开头或结尾的分隔符分别产生开头或结尾的空字段。
- 没有出现分隔符时，整个字符串就是唯一的字段。
- 字段内的空格、制表符、换行等其他字符都要原样保留。
- `join_fields` 用 `separator` 把字段依次连接；空 vector 返回空串；空字段要保留；`separator` 可以是空串。
- 不要写 `main`。

### 提示

可以用 `std::string::find` 从左到右找分隔符，也可以用 `std::getline` 配合 `std::stringstream`，但要额外处理空输入和结尾分隔符。
