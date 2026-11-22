This exercise reads no stdin and writes no stdout. Implement a class that records the order in which instances are destroyed; a hidden harness runs the tests.

Implement:

```cpp
class ScopeTracer {
public:
    explicit ScopeTracer(int id);
    ~ScopeTracer();
    ScopeTracer(const ScopeTracer&) = delete;
    ScopeTracer& operator=(const ScopeTracer&) = delete;

    int id() const;

    static void clear_log();
    static std::vector<int> destroyed_log();  // ids in destruction order
};
```

Requirements:

- Store `id` at construction;
- Append `id` to the class's internal static log in the destructor;
- `clear_log()` empties the log and `destroyed_log()` returns a copy;
- Do not write `main`.

Hint: automatic objects in one scope are destroyed in reverse construction order; `delete` destroys only the object being deleted.
