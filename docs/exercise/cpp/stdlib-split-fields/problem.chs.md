哦，我亲爱的朋友，你总算来了。快请坐，别客气——就当这儿是你自己的家。

事情是这样的：我们有两台老机器，一台叫 `split_fields`，一台叫 `join_fields`。第一台拿到一段文本和一个小字符，就照着那个字符把文本切成字段；每遇见一个分隔符，就开一个新字段，两个分隔符挨在一起，中间就留下一个空字段。开头有分隔符，第一个字段是空的；结尾有分隔符，最后一个字段也是空的。可要是整段文本本来就是空的呢？那它一个字段也没有，交回来的是一只空箱子，里面空空如也，绝没有一个空字段。

第二台机器正相反：给它一串字段和一个分隔串，它就把字段一个个接起来，中间塞进分隔串。字段是空的，它照样接；箱子是空的，它交回一段空文本；分隔串也可以是空的，上帝作证，它不会抱怨。

还有，别自己写 `main`。

**人话**：实现 `split_fields` 与 `join_fields`。`split_fields` 按单个字符 `delimiter` 拆分，每个分隔符都产生新字段，连续分隔符产生空字段；空字符串拆成空 `vector`，开头或结尾的分隔符分别产生开头或结尾的空字段；没有分隔符时整串是唯一字段；字段内的空格、制表符、换行等字符都原样保留。`join_fields` 用 `separator` 连接字段，空 `vector` 返回空串，空字段保留，`separator` 可以为空串。不要写 `main`。

```cpp
std::vector<std::string> split_fields(const std::string& text, char delimiter);
std::string join_fields(const std::vector<std::string>& fields,
                        const std::string& separator);
```

### 示例

| 调用 | 结果 |
| --- | --- |
| `split_fields("a,b", ',')` | `{"a", "b"}` |
| `split_fields("", ',')` | `{}` |
| `split_fields(",a,", ',')` | `{"", "a", ""}` |
| `join_fields({"a", "", "b"}, "-")` | `"a--b"` |

### 提示

可以用 `std::string::find` 从左到右找分隔符，也可以用 `std::getline` 配合 `std::stringstream`，但要额外处理空输入和结尾分隔符。
