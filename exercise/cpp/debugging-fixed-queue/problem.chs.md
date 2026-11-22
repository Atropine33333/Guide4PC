这一题不读 stdin，也不写 stdout。给定的环形队列实现能编译，但 `push` 在队列已满时的行为不对。请找出并修正错误，保持公开接口不变。测试在隐藏的 harness 里。

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

正确行为：

- `capacity >= 0`；容量为 0 时 `push` 和 `pop` 都返回 `false`。
- `push(value)`：队列未满时把 `value` 放到队尾并返回 `true`；已满时不改变队列，返回 `false`。
- `pop(value)`：队列非空时把队首写入 `value`、移除队首并返回 `true`；为空时返回 `false`，`value` 不变。
- `size()` 返回当前元素个数；`capacity()` 返回容量；`clear()` 清空但不释放内部数组。
- 队列已满时绝不能覆盖尚未出队的旧元素。
- 不要写 `main`。

### 提示

starter 在满队列时用新元素覆盖了队首并返回 `true`。把它改成拒绝新元素即可；同时确认回绕、清空和零容量路径仍然正确。
