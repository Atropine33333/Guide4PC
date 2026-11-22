**THE CODE OF DESTRUCTION ORDER**

Article 1 (Purpose) This Code establishes the order in which objects leave, and forbids disorder in the record, omitted entries, and double entries.

Article 2 (Filing) When a `ScopeTracer` is constructed, its `id` shall be filed; `id()` shall return that number.

Article 3 (Striking off) Upon destruction, the object's `id` shall be appended to the end of the class's internal static log. The log belongs to the whole class and does not split as objects come and go.

Article 4 (Clearing) `clear_log()` empties the log; `destroyed_log()` hands back a copy, after which the internal log and the copy each go their own way.

Article 5 (Order) Automatic objects in the same scope are destroyed in reverse construction order; `delete` removes only the object deleted, and bystanders are not implicated.

Article 6 (Prohibitions) Copy construction and copy assignment shall not be declared. Do not write `main`.

Article 7 (Supplementary) Matters not provided for herein follow the C++ standard.

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

**In plain words**: implement `ScopeTracer`. Remember `id` at construction and append it to the class's static log in the destructor. `clear_log()` empties the log; `destroyed_log()` returns a copy of it.
