实现一个管理动态数组的小类，正确处理拷贝、赋值与析构。补全下面的接口：

```cpp
class IntBuffer {
public:
    explicit IntBuffer(std::size_t size);            // 分配 size 个 int，全部置 0
    IntBuffer(const IntBuffer& other);               // 深拷贝
    IntBuffer& operator=(const IntBuffer& other);    // 深拷贝，自赋值安全
    ~IntBuffer();                                    // 释放内存

    std::size_t size() const;
    int& at(std::size_t index);
    const int& at(std::size_t index) const;

private:
    int* data_ = nullptr;
    std::size_t size_ = 0;
};
```

要求：拷贝和赋值之后两个对象各自拥有独立的内存；析构函数释放内存且不会重复释放；允许 `size == 0`，此时不分配内存。

**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
IntBuffer a(3);
a.at(0) = 7;
IntBuffer b(a);      // 深拷贝
b.at(0) = 9;
// a.at(0) 仍然是 7
```

### 约束

- 下标保证在范围内。
- 不要求支持移动语义。
- 只比较值，不比较地址。
