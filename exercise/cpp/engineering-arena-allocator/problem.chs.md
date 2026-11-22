这一题不读 stdin，也不写 stdout。实现一个简单的线性分配器（arena），测试在隐藏的 harness 里。

实现：

```cpp
class Arena {
public:
    explicit Arena(int capacity);  // 1 <= capacity <= 256

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    // 分配 size 字节，起始地址按 alignment 对齐。
    // size <= 0 或空间不足时返回 nullptr，且不改变已用空间。
    void* allocate(int size, int alignment);

    int used() const;      // 已经占用的字节数（含对齐填充）
    int capacity() const;  // 构造时给定的容量
    void reset();          // 回收全部空间
};
```

要求：

- `alignment` 保证是 2 的幂且不超过 16（例如 1、2、4、8、16）；
- 内部使用一块真正的定长字节缓冲区，不要用 `new` 或 `std::vector` 做每次分配；
- 分配时先向上取整到对齐边界，再占用 `size` 字节；
- 失败时 `used()` 必须保持不变；
- `allocate` 返回的地址必须满足 `地址 % alignment == 0`；
- 不要写 `main`。

提示：让内部缓冲区用 `alignas(16)` 声明；对齐公式为 `(offset + alignment - 1) / alignment * alignment`，而 `alignment` 是 2 的幂时也可以用位运算。
