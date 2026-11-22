《相声·传值与传引用》

甲：您给瞧瞧，这 `Data` 递进函数，递的到底是什么？

乙：递的是它自己——不过得看形参怎么写。

甲：形参写 `Data data`，递过去的是什么？

乙：一份拷贝。人家在副本上添一笔，您手里这份一个数都不多。

甲：形参写 `Data& data` 呢？

乙：那递的是本人。人家在本人身上添一笔，您这份也就多了一笔，躲都躲不开。

甲：那 `const Data&` 呢？

乙：递的还是本人，可人家只准看不准动，您这份原封不动。

甲：返回 `Data` 与返回 `const int&`，又有何不同？

乙：返回 `Data`，您接到的是一份新数据，改了也不牵连原主；返回 `const int&`，您接到的是原元素的别名，指错了地方，台下观众可要喊倒好。

甲：得，您这一说，值、引用、拷贝，全明白了。

**人话**：实现 `total`、`append_copy`、`append_ref`、`make_data`、`first_value` 五个函数：按值传参的只能改副本，按引用传参的才改原对象，`first_value` 必须返回 `values[0]` 的引用。

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

### 要求

- `total` 返回 `values` 中所有元素的和；空向量返回 `0`。
- `append_copy` 按值接收 `Data`，在它自己的副本末尾追加 `value`，调用方的对象必须保持原样。
- `append_ref` 按引用接收 `Data`，在原对象末尾追加 `value`，并保持 `id` 不变。
- `make_data` 返回一个 `id` 为参数 `id`、`values` 依次为 `0, 1, …, count - 1` 的对象；`count` 为 `0` 时返回空向量。
- `first_value` 返回 `data.values[0]` 的引用（调用方保证向量非空），不能返回副本。
- `Data` 的直接拷贝必须是独立的：修改拷贝的 `id` 或 `values` 不得影响原对象。
- 不要写 `main`。

### 提示

`std::vector` 的拷贝构造会深拷贝元素，所以 `Data` 的默认拷贝就是独立的；注意 `append_copy` 的形参没有 `&`，`first_value` 的返回类型是 `const int&`。
