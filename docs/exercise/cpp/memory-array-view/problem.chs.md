这一题不读 stdin，也不写 stdout。实现一个不拥有数据的数组视图类，测试在隐藏的 harness 里。

```cpp
class IntView {
public:
    IntView();
    IntView(int* data, int count);

    int* data() const;
    int size() const;
    bool empty() const;

    int& operator[](int index);
    const int& operator[](int index) const;

    IntView slice(int begin, int count) const;
};
```

规则：

- 默认构造的视图为空：`data()` 为 `nullptr`，`size()` 为 0。
- 构造函数在 `data == nullptr` 或 `count <= 0` 时也构造空视图。
- 视图不拷贝数据，只保存指针和长度；通过视图修改元素会修改原数组。
- `slice(begin, count)` 返回从 `begin` 开始、长度为 `count` 的视图，与原视图共享内存；`slice` 不能修改原视图。
- 切片越界（`begin < 0`、`count < 0`、`begin >= size()`、`begin + count > size()`）或 `count == 0` 时返回空视图。
- `operator[]` 的下标保证合法，不需要做边界检查。
- 不要写 `main`。

### 提示

`slice` 返回的视图只需要保存 `data_ + begin` 和 `count`。检查 `begin + count` 时注意不要用可能溢出的类型累加。
