固定资产管理办公室文件

资办发〔2026〕11 号

关于印发《整数槽位固定池管理办法》的通知

各借出单元、各归还窗口：

为规范槽位借用与归还，杜绝野指针、重复归还与容量虚报，现将《整数槽位固定池管理办法》印发给你们，请遵照执行。

第一条 池子在构造时一次划定 `capacity` 个槽位（`capacity >= 0`），终身不增不减。容量为零时，借用一律无果，归还一律不收。

第二条 `acquire(value)` 从下标 0 起找第一个空闲槽位，占用、写入、交回地址；池满时交回空指针。

第三条 `release(slot)` 只收本池当前借出的槽位，且必须指针相等；外部指针、空指针、已经归还过的槽位，一律不收，账目不动。

第四条 `live()` 如实上报在借数量，`capacity()` 如实上报总容量。虚报者按弄虚作假处理。

第五条 析构时释放内部数组，容量为零也要安全。拷贝构造与拷贝赋值已经删除，不得补写。

第六条 不要写 `main`，不要修改公开接口。

**人话**：补完 `IntPool`。`slots_` 存值，`used_` 记借出状态；`release` 遍历槽位、用地址相等判断归属，别拿两个无关指针比大小。

```cpp
class IntPool {
public:
    explicit IntPool(int capacity);
    ~IntPool();

    IntPool(const IntPool&) = delete;
    IntPool& operator=(const IntPool&) = delete;

    int* acquire(int value);
    bool release(int* slot);
    int live() const;
    int capacity() const;

private:
    int* slots_;
    bool* used_;
    int capacity_;
    int live_;
};
```

### 规则或约定

- `capacity >= 0`；容量为 0 时 `acquire` 永远返回 `nullptr`，`release(nullptr)` 返回 `false`。
- 构造时分配 `capacity` 个槽位，初始都未使用。
- `acquire(value)` 找到第一个空闲槽位，标记为已使用，写入 `value`，返回该槽位的地址；池满时返回 `nullptr`。空闲槽位从下标 0 开始找。
- `release(slot)` 只有当 `slot` 正好指向池内某个当前已使用的槽位时才返回 `true` 并释放它；其他情况（外部指针、`nullptr`、已经释放过）返回 `false`，且不改变状态。
- `live()` 返回当前已分配的槽位数；`capacity()` 返回总容量。
- 析构函数释放内部数组，容量为 0 时也要安全。
- 拷贝构造和拷贝赋值已删除，不要实现它们。
- 不要写 `main`。

### 提示

`slots_` 保存值，`used_` 记录每个槽位是否已借出。`release` 可以遍历槽位，用地址相等判断指针是否属于池。
