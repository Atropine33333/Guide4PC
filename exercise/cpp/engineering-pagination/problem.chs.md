实现一个分页器，为列表页提供页码与区间信息：

```cpp
class Pager {
public:
    Pager(int total_items, int page_size, int current_page);

    int page_count() const;      // 总页数
    int current() const;         // 当前页，从 1 开始
    bool has_prev() const;
    bool has_next() const;

    int first_index() const;     // 当前页第一条的 0 基下标；没有数据时为 -1
    int last_index() const;      // 当前页最后一条的 0 基下标；没有数据时为 -1

    std::vector<int> window(int radius) const;  // 当前页前后各 radius 页的页码
};
```

不变量：

- `page_size` 至少为 1，`total_items` 为 0 表示没有数据。
- `current_page` 可任意传入，但内部必须钳制到 `[1, page_count]`；没有数据时 `page_count() == 0`、`current() == 1`。
- `window(r)` 返回从 `current() - r` 到 `current() + r` 的所有页码（升序），并裁剪到合法范围；`r < 0` 按 0 处理。

**不要写 `main`。**

### 输入

无。

### 输出

无。

### 示例

```cpp
Pager pager(95, 10, 5);
pager.page_count();   // 10
pager.first_index();  // 40
pager.window(2);      // {3, 4, 5, 6, 7}
```

### 约束

- `total_items` 不超过 `10^9`，`page_size` 不超过 `10^6`。
- `window` 的半径不超过 `10^4`。
