这一题不读 stdin。实现一个管理若干行文本的类，并依赖编译器自动生成的特殊成员函数。

```cpp
#include <string>
#include <vector>

class Notebook {
public:
    void add(const std::string& line);
    std::size_t size() const;
    std::string at(std::size_t index) const;
    std::string join(const std::string& separator) const;
};
```

约定：

- 内部只保存一个 `std::vector<std::string>`。
- **不要**声明析构函数、拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符；让编译器自动生成它们（rule of zero）。
- `add` 追加一行。
- `at` 返回第 `index` 行（从 `0` 开始）；越界时返回 `"?"`。
- `join` 用 `separator` 连接所有行；没有行时返回空串。
- 不要写 `main`。

harness 会验证拷贝出来的对象与原对象互不影响，也会验证移动之后目标对象的内容。
