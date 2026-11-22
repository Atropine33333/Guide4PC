《析构次序法典》

第一条（目的）为明确对象离场之先后，杜绝乱序、漏记与重复记账，制定本法典。

第二条（存档）`ScopeTracer` 构造之时，须将 `id` 记录在案，`id()` 可取回此数。

第三条（注销）析构之时，须将该对象之 `id` 追加于本类内部静态日志之末。日志为全类共有，不因对象生灭而分裂。

第四条（清理）`clear_log()` 清空日志；`destroyed_log()` 交还日志之副本，此后类内日志与副本各自为政。

第五条（次序）同一作用域内之自动对象，后建者先殁；`delete` 只送走被删除的那一个，旁人不受牵连。

第六条（禁令）不得声明拷贝构造与拷贝赋值；不得编写 `main`。本题不读 stdin，不写 stdout，取证之事交由隐藏 harness。

第七条（附则）本法未定之处，以 C++ 标准为准。

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

**人话**：实现 `ScopeTracer`：构造时记住 `id`，析构时把 `id` 追加到类内静态日志末尾；`clear_log()` 清空日志，`destroyed_log()` 返回日志副本。harness 摆出若干作用域，核对日志里编号出现的先后。
