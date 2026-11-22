内存资源管理办公室文件

内存办发〔2026〕7 号

关于开展线性分配器专项整理工作的通知

各分配单元、各对齐边界：

为进一步规范内存资源使用，杜绝越界、错位与重复占用，经研究，决定对所辖线性分配器（arena）开展专项整理。现将有关事项通知如下。

一、各单元应当按照下列接口实现 `Arena` 类，不得擅自增删成员函数。

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

二、`alignment` 为 2 的幂且不超过 16；返回的地址必须满足 `地址 % alignment == 0`。

三、内部使用一块真正的定长字节缓冲区；不得以 `new` 或 `std::vector` 逐次分配。

四、分配时先将偏移向上取整到对齐边界，再占用 `size` 字节；对齐填充计入 `used()`。

五、`size <= 0` 或空间不足时返回 `nullptr`，`used()` 保持原值，不得部分占用，不得回滚。

六、`reset()` 回收全部空间；`used()` 与 `capacity()` 应当如实上报。

七、请勿编写 `main`。内存使用情况由隐藏的 harness 从外部核查。

本通知自印发之日起执行。

内存资源管理办公室
2026 年 11 月 22 日

抄送：栈管理委员会，堆管理委员会。

**人话**：补完上面的 `Arena`。分配前先对齐、再占字节；失败时账本不许动；缓冲区用 `alignas(16)` 的定长数组，别每次 `new`。`reset()` 把偏移归零即可。

附：对齐公式为 `(offset + alignment - 1) / alignment * alignment`；`alignment` 是 2 的幂时，也可以写成位运算。
