这一题不读 stdin，也不写 stdout。实现一个写时复制（copy-on-write）的字符串缓冲区。

```cpp
#include <cstddef>
#include <string>

class CowBuffer {
public:
    CowBuffer();
    explicit CowBuffer(const std::string& data);
    CowBuffer(const CowBuffer& other);
    CowBuffer& operator=(const CowBuffer& other);
    ~CowBuffer();

    const std::string& str() const;
    void append(const std::string& suffix);
    std::size_t shares() const;

    static int live_blocks();
};
```

规则：

- 拷贝构造和拷贝赋值只共享底层存储，不复制字符串，也不增加 `live_blocks()`。
- `shares()` 返回当前对象与多少个 `CowBuffer` 对象共享同一块存储（包含自身，至少为 1）。
- `append` 必须先确保存储唯一：如果 `shares() > 1`，先复制出自己的存储，再追加；原共享对象的字符串不能改变。
- `live_blocks()` 返回当前仍存活的、互不相同的底层存储块数量（不是对象数量）。
- 拷贝赋值要正确处理自赋值；被替换掉的存储若已无对象使用，应及时释放。
- 空缓冲区也必须有一块有效存储。
- 不要写 `main`，不要修改公开接口。
