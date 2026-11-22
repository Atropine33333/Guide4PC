这一题不读 stdin，也不写 stdout。实现四个操作动态数组的函数，测试在隐藏的 harness 里。

```cpp
int* make_zero_array(int count);
void fill_sequence(int* data, int count, int start);
long long sum_array(const int* data, int count);
void free_array(int* data);
```

规则：

- `make_zero_array(count)`：`count <= 0` 时返回 `nullptr`；否则用 `new int[count]()` 分配并把所有元素初始化为 0。
- `fill_sequence(data, count, start)`：把 `data[i]` 设为 `start + i`；`count <= 0` 时什么也不做。
- `sum_array(data, count)`：返回前 `count` 个元素的和；`count <= 0` 或 `data == nullptr` 时返回 0。
- `free_array(data)`：释放 `make_zero_array` 返回的数组，必须能接受 `nullptr`。
- 不要写 `main`。

### 提示

`new int[n]()` 中的空括号表示值初始化，会把每个元素置 0。分配和释放要配对，不要混用 `new[]`/`delete`。
