这一题不读 stdin。隐藏的 harness 会调用下面几个函数，检查你是否分清了值、引用和拷贝。

```cpp
#include <vector>

struct Data {
    int id;
    std::vector<int> values;
};

int total(const Data& data);
void append_copy(Data data, int value);
void append_ref(Data& data, int value);
Data make_data(int id, int count);
const int& first_value(const Data& data);
```

要求：

- `total` 返回 `values` 中所有元素的和；空向量返回 `0`。
- `append_copy` 按值接收 `Data`，在它自己的副本末尾追加 `value`，调用方的对象必须保持原样。
- `append_ref` 按引用接收 `Data`，在原对象末尾追加 `value`，并保持 `id` 不变。
- `make_data` 返回一个 `id` 为参数 `id`、`values` 依次为 `0, 1, …, count - 1` 的对象；`count` 为 `0` 时返回空向量。
- `first_value` 返回 `data.values[0]` 的引用（调用方保证向量非空），不能返回副本。
- `Data` 的直接拷贝必须是独立的：修改拷贝的 `id` 或 `values` 不得影响原对象。
- 不要写 `main`。

### 提示

`std::vector` 的拷贝构造会深拷贝元素，所以 `Data` 的默认拷贝就是独立的；注意 `append_copy` 的形参不是引用，`first_value` 的返回类型不是 `int`。
