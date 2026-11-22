惠子谓庄子曰：“吾有一物，名曰 `Notebook`，内藏一个 `std::vector<std::string>`，收纳若干行文字。吾将为它作析构，作拷贝，作赋值，作移动，五者毕具，然后敢用。”

庄子曰：“子为此五者，是犹代庖人宰牛也。编译器爪牙已利，何不使之自割？”

惠子曰：“不写则浅，不写则漏，吾惧焉。”

庄子曰：“子所惧者，裸指针也。今子所藏乃一个 `std::vector`，它能自深其拷贝，自移其内容，自放其旧物。子若只手不伸，深浅皆宜；子若伸手写了其中一个，余者随之作废，反受其乱。”

惠子曰：“然则 `at` 越界奈何？”

庄子曰：“归 `"?"` 而已。`join` 者，以 `separator` 连诸行；行尽，则空串，此亦自然。`add` 追加一行，`size` 报行数，皆不足挂齿。”

惠子曰：“虽然，尚有一事：`main` 可写乎？”

庄子曰：“天刑之，安可解！”

**人话**：实现 `Notebook`：内部只存一个 `std::vector<std::string>`，析构、拷贝构造、拷贝赋值、移动构造、移动赋值五个都不许声明，一律交给编译器生成。`add` 追加一行；`at` 返回第 `index` 行，越界返回 `"?"`；`join` 用 `separator` 连接所有行，没有行时返回空串。本题不读 stdin；不写 `main`。harness 会验证拷贝独立，也会验证移动之后目标对象的内容。

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
