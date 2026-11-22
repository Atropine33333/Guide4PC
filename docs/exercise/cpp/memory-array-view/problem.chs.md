甲：今儿说个 `IntView`。这位爷不养孩子，只替人看孩子。

乙：怎么个看法？

甲：构造它，得递上 `int* data` 和一串 `count`。`data` 是空指针，或者 `count <= 0`，它就两手一摊，报一个空视图：`data()` 得 `nullptr`，`size()` 得 0。

乙：干脆，省粮。

甲：视图不拷贝数据，只把指针和长度揣兜里。所以 `operator[]` 一改，原数组也跟着变——本来就是一回事。

乙：借人家的锅，炒自家的菜。

甲：`slice(begin, count)` 更省事：从 `begin` 起切一段，还是那块内存，跟原视图合穿一条裤子，原视图一根毫毛不动。

乙：那要是切到墙外头呢？

甲：`begin < 0`、`count < 0`、`begin >= size()`、`begin + count > size()`，或者 `count == 0`，它就还你一个空视图，一声不吭。

乙：得，白跑一趟。

甲：`operator[]` 的下标是保证合法的，只管取，不必设卡；`empty()` 见 `size() == 0` 就报 `true`。

乙：得嘞，齐活。

**人话**：实现 `IntView`：默认构造为空视图；构造函数在 `data == nullptr` 或 `count <= 0` 时也构造空视图。视图只保存指针与长度，不拷贝数据，通过视图改元素会改原数组。`slice(begin, count)` 返回与原视图共享内存的新视图，不修改原视图；越界或 `count == 0` 时返回空视图。`operator[]` 的下标保证合法，无需检查。本题不读 stdin，不写 stdout；不写 `main`。

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
