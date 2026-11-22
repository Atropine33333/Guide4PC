作用域守卫（scope guard）在离开作用域时自动执行一段清理动作，常用于没有异常的环境里模拟 RAII 清理。请补全下面的类：

```cpp
class ScopeGuard {
public:
    explicit ScopeGuard(void (*action)(void*), void* context);
    ~ScopeGuard();

    void dismiss();  // 取消执行；再次调用无副作用

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    // 自行添加成员
};
```

构造后，若没有调用 `dismiss()`，析构函数必须调用一次 `action(context)`；调用过 `dismiss()` 后析构函数不再执行动作。守卫不可拷贝。

**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
int count = 0;
{
    ScopeGuard guard([](void* p) { ++*static_cast<int*>(p); }, &count);
}  // 离开作用域后 count == 1
```

（示例里的 lambda 不捕获任何变量，可以转换为函数指针。）

### 约束

- 同一作用域内可能有多个守卫，它们按构造的逆序析构，因此动作也要按逆序执行。
- 不要求支持移动，也不要求处理 `action == nullptr`。
