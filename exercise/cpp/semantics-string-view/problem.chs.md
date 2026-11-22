《译制片·一封不该誊写的电报》

（书房，壁炉旁。老管家手捧一张纸，向主人躬身。）

管家：哦，我亲爱的老爷，邮差刚送来这份电报。它太长啦，誊一遍准要误了晚饭。

主人：老天爷，那就别誊。您只需用手指点出要读的那一段——从哪儿起，到哪儿止——眼睛看着原纸就行。纸张不动，一个字也不许抄到别处去。

管家：可要是我想裁掉两头的空白呢？

主人：那您就把手指往里挪一挪，纸还是那张纸。哦，还有，要是找不到您要的记号，就老老实实回报「没有」，千万别拿一张空白纸来充数。

管家：遵命，老爷。这份电报会一直摊在桌上，直到您用完为止。

**人话**：用 `std::string_view` 实现 `trim`、`first_word`、`between`、`starts_with` 四个函数：只观察字符，不做拷贝；非空的返回值必须直接指向传入的 `text` 缓冲区，不能指向局部对象或临时 `std::string`。

```cpp
#include <string>
#include <string_view>

std::string_view trim(std::string_view text);
std::string_view first_word(std::string_view text);
std::string_view between(std::string_view text, char open, char close);
bool starts_with(std::string_view text, std::string_view prefix);
```

### 约定

- `trim` 去掉首尾的 ASCII 空格 `' '`，返回剩下的部分；全是空格或空串时返回空视图。
- `first_word` 返回从第一个非空格字符开始、到下一个空格或结尾为止的片段；没有非空格字符时返回空视图。
- `between` 返回第一个 `open` 之后、其后第一个 `close` 之前的片段；找不到 `open`，或 `open` 后面没有 `close` 时返回空视图。
- `starts_with` 判断 `prefix` 是否为 `text` 的前缀；空前缀视为真。
- 返回值必须直接指向传入的 `text`，不能指向局部对象或临时 `std::string`。
- 不要写 `main`。

### 提示

`std::string_view::substr` 返回的视图仍然指向原缓冲区；注意 `find` 返回 `std::string_view::npos` 的情况。
