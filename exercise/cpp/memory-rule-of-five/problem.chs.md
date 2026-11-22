实现一个遵守“五法则”的资源类：它持有一个堆上的 `int`，并提供拷贝、移动、赋值与析构的完整语义。补全下面的接口：

```cpp
class Resource {
public:
    explicit Resource(int value);
    Resource(const Resource& other);
    Resource(Resource&& other) noexcept;
    Resource& operator=(const Resource& other);
    Resource& operator=(Resource&& other) noexcept;
    ~Resource();

    int value() const;      // 没有资源时返回 -1
    void set(int value);    // 没有资源时什么也不做

    static int live();      // 当前仍被持有的资源数量
    static int copies();    // 拷贝构造 + 拷贝赋值的累计次数
    static int moves();     // 移动构造 + 移动赋值的累计次数

private:
    int* value_ = nullptr;
};
```

要求：

- 构造时分配一个 `int`，`live()` 加一；析构时释放，`live()` 减一。
- 拷贝构造与拷贝赋值都分配新的 `int`，并把 `copies()` 加一。
- 移动构造与移动赋值都直接接管指针，把源对象置为空，并把 `moves()` 加一；移动后源对象的 `value()` 返回 `-1`。
- 自赋值（`a = a`）不做任何事，`copies()` 不变。
- 移动操作必须标 `noexcept`，否则 `std::vector` 会退化为拷贝。

评测代码会直接包含你的实现并调用这些成员函数。**不要写 `main`。**

### 输入

无。

### 输出

无。评测代码检查返回值与静态计数。

### 示例

```cpp
Resource a(7);
Resource b(std::move(a));
// b.value() == 7, a.value() == -1, Resource::live() == 1
```

### 约束

- 不要使用 `std::shared_ptr` 或 `std::unique_ptr`，必须自己管理指针。
- 不检查内存分配失败。
