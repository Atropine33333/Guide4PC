这一题不读 stdin，也不写 stdout。实现一个会记录自己析构顺序的类，测试在隐藏的 harness 里。

实现：

```cpp
class ScopeTracer {
public:
    explicit ScopeTracer(int id);
    ~ScopeTracer();
    ScopeTracer(const ScopeTracer&) = delete;
    ScopeTracer& operator=(const ScopeTracer&) = delete;

    int id() const;

    static void clear_log();
    static std::vector<int> destroyed_log();  // 按析构发生的顺序返回编号
};
```

要求：

- 构造时保存 `id`；
- 析构时把 `id` 追加到类内部的静态日志末尾；
- `clear_log()` 清空日志，`destroyed_log()` 返回日志的副本；
- 不要写 `main`。

提示：同一作用域内的自动对象按构造的逆序析构；`delete` 只析构被删除的那个对象。
