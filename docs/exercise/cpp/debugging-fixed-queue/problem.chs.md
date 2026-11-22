环形队列管理办公室文件

环队办发〔2026〕13 号

关于开展固定容量环形队列专项整改的通知

各队首、各队尾：

为规范固定容量队列的入队、出队、清空与回绕，杜绝满队覆盖、容量错记、旧元素被新元素顶替，经研究，决定开展专项整改。现将有关事项通知如下。

一、各单位应当按照下列接口实现 `FixedQueue`，不得擅自增删成员函数。

```cpp
class FixedQueue {
public:
    explicit FixedQueue(int capacity);
    ~FixedQueue();

    FixedQueue(const FixedQueue&) = delete;
    FixedQueue& operator=(const FixedQueue&) = delete;

    bool push(int value);
    bool pop(int& value);
    int size() const;
    int capacity() const;
    void clear();

private:
    int* data_;
    int capacity_;
    int head_;
    int count_;
};
```

二、`capacity >= 0`；容量为 0 时，`push` 与 `pop` 一并返回 `false`。

三、`push(value)`：队未满时把 `value` 置于队尾并返回 `true`；队已满时不得改动队列，返回 `false`。

四、`pop(value)`：队非空时把队首写入 `value`、移除队首并返回 `true`；队空时返回 `false`，`value` 保持原值。

五、`size()` 如实上报当前元素个数；`capacity()` 如实上报容量；`clear()` 清空队列，内部数组不得释放。

六、队满时不得覆盖尚未出队的旧元素。此条为本次整改重点。

七、请勿编写 `main`。各项行为由隐藏的 harness 从外部核查。

本通知自印发之日起执行。

环形队列管理办公室
2026 年 11 月 22 日

抄送：栈管理委员会，动态数组管理委员会。

**人话**：补完 `FixedQueue`：内部用环形数组，`head_` 记队首位置，`count_` 记当前元素个数，写回绕用 `(head_ + count_) % capacity_`。队满时 `push` 返回 `false` 且队列不动；队列空时 `pop` 返回 `false`。starter 在满队时用新元素覆盖队首并返回 `true`，改掉这处，再确认回绕、清空与零容量三条路径。

### 提示

满队判断写作 `count_ == capacity_`，先判断再写入；`clear()` 只需把 `head_` 与 `count_` 归零。
