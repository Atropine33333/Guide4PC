A scope guard runs a cleanup action automatically when leaving a scope, which is a common way to emulate RAII cleanup where exceptions are unavailable. Complete this class:

```cpp
class ScopeGuard {
public:
    explicit ScopeGuard(void (*action)(void*), void* context);
    ~ScopeGuard();

    void dismiss();  // cancel; later calls have no additional effect

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    // add your own members
};
```

After construction, if `dismiss()` was never called, the destructor must call `action(context)` exactly once. Once dismissed, the destructor performs no action. The guard is not copyable.

**Do not write `main`.**

### Input

None.

### Output

None.

### Example

```cpp
int count = 0;
{
    ScopeGuard guard([](void* p) { ++*static_cast<int*>(p); }, &count);
}  // after leaving the scope, count == 1
```

(The lambda captures nothing, so it converts to a function pointer.)

### Constraints

- Several guards may live in one scope; they are destroyed in reverse construction order, so their actions run in reverse order.
- Moving is not required, and `action == nullptr` need not be handled.
