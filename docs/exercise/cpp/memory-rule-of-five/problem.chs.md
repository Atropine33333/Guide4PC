甲：今儿说个 `Resource`，手里攥着一个堆上的 `int`。

乙：一人一个坑。

甲：它得会五样：拷贝构造、移动构造、拷贝赋值、移动赋值，外加析构。

乙：五样，一样不能少。

甲：先说拷贝。`Resource b(a);` 之后，两位各揣各的 `int`，`copies()` 记一笔，`live()` 涨一格。

乙：各过各的日子，互不串门。

甲：移动就不同了。`std::move` 一到，指针连行李整体搬家，源对象两手空空，`value()` 报 `-1`，`moves()` 记一笔。

乙：搬完记得把旧屋的钥匙留下。

甲：一处容易栽跟头：两个移动函数都得挂 `noexcept`。

乙：不挂呢？

甲：不挂，`std::vector` 见它不保证不动窝，宁可多拷贝几份。

乙：合着它被自己的履历吓住了。

甲：自赋值也得留神。`a = a` 只当没发生，`copies()` 不许涨。

乙：自家的东西，何必先扔再捡。

甲：都写在这张纸上了。

**人话**：实现 `Resource`：构造 `new` 一个 `int` 并让 `live()` 加一，析构释放并减一；拷贝构造与拷贝赋值各分配新 `int`，`copies()` 加一；移动构造与移动赋值接管指针、把源对象置空、`moves()` 加一，移动后源对象 `value()` 为 `-1`；自赋值不动 `copies()`；两个移动函数标 `noexcept`。

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

### 输入

无。

### 输出

无。

### 示例

```cpp
Resource a(7);
Resource b(std::move(a));
// b.value() == 7, a.value() == -1, Resource::live() == 1
```

### 约束

- 不要使用 `std::shared_ptr` 或 `std::unique_ptr`，必须自己管理指针。
- 不检查内存分配失败。
- **不要写 `main`。**
