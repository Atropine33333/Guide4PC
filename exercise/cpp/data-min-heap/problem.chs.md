关于规范最小堆实现的若干规定

各实现单元：

为统一最小堆（min-heap）的实现口径，现将有关事项通知如下。

第一条　本通知所称最小堆，指自行实现的最小堆，禁止调用 `std::priority_queue`。
第二条　必须实现 `push`、`pop`、`top`、`size`、`empty` 五个成员，签名以附件接口为准，不得增删。
第三条　`pop` 弹出并返回当前最小值，`top` 只读不弹；此二者仅在堆非空时调用。
第四条　元素个数上限为 100000；`int` 范围内任意值，含负数，一律照收。
第五条　堆序不变式须在每次 `push`、`pop` 之后成立：任一结点不大于其两个子结点。
第六条　不得自带 `main`。
第七条　固定种子生成随机操作序列，结果与多重集合逐一比对。
第八条　附件中的用法示例，各单位可照此自测。

请遵照执行。

**人话**：自己实现一个最小堆，不要用 `std::priority_queue`，补全 `MinHeap` 类的 `push`、`pop`、`top`、`size`、`empty`。元素最多 100000 个，值可为任意 `int`（含负数）；`pop` 与 `top` 只在堆非空时调用；每次 `push`、`pop` 之后堆序不变式必须成立。**不要写 `main`**。

```cpp
class MinHeap {
public:
    void push(int value);
    int pop();                 // 弹出并返回最小值
    int top() const;           // 返回最小值，不弹出
    std::size_t size() const;
    bool empty() const;
};
```

### 输入

无。

### 输出

无。

### 示例

```cpp
MinHeap heap;
heap.push(5);
heap.push(1);
heap.push(3);
heap.pop();   // 返回 1
heap.top();   // 返回 3
```

### 约束

- 元素个数最多 100000。
- `int` 范围内任意值，包括负数。
- `pop` 和 `top` 只在堆非空时调用。
- 固定种子生成随机操作序列，结果与多重集合比较。
