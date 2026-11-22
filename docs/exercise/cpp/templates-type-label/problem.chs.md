这一题不读 stdin，也不写 stdout。用模板特化实现一个类型标签，测试在隐藏的 harness 里。

实现主模板和一个特化族：

```cpp
template <typename T>
struct TypeLabel {
    static const char* name();  // 主模板返回 "other"
    static int rank();          // 主模板返回 0
};
```

特化规则：

| 类型 | `name()` | `rank()` |
| --- | --- | --- |
| `int` | `"int"` | 1 |
| `double` | `"double"` | 2 |
| `bool` | `"bool"` | 3 |
| 任意指针 `T*` | `"pointer"` | 4 |
| 其他 | `"other"` | 0 |

要求：

- 只写头文件式的定义，不要写 `main`；
- 指针规则要对 `int*`、`double*` 以及 `const char*` 都成立；
- 类型标签在编译期确定，不要用 `typeid` 或运行时字符串比较；
- `name()` 返回的字符串字面量至少活到程序结束。
