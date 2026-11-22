（占卜师把 `N` 张牌在桌上排成一列，牌面朝上。）

问卜者：这一列牌，问的是什么？

占卜师：两件事。一是这一列的“平均值”——把每张牌的点数并作一处，再按张数均分。二是这一列里最重的那张。

问卜者：平均值会不会因为牌是整数就被抹掉零头？

占卜师：会，所以读牌的人要拿 `double` 的算盘。整数相加之后再除，零头就留不住了。

问卜者：最重的那张，用哪把尺子量？

占卜师：只用 `<`。谁比谁小，一张张比过去，最大的自然浮出来。两张一样重，读哪张都成，牌面看不出来。

问卜者：万一牌是字句呢？

占卜师：字句也能比大小，按字典次序。不过字句只问最重的那张，不问平均——平均字句这种事，牌阵里没有。

**人话**：实现两个接收数组引用的函数模板 `average` 与 `max_value`，从数组引用推导 `T` 和 `N`；`N >= 1`，`average` 返回 `double` 且不得被整数除法截断，`max_value` 按 `<` 取最大。

```cpp
template <typename T, int N>
double average(const T (&values)[N]);

template <typename T, int N>
T max_value(const T (&values)[N]);
```

规则：

- 两个模板都从数组引用推导 `T` 和 `N`，`N >= 1`。
- `average` 返回所有元素的算术平均值，类型是 `double`；即使 `T` 是整数，也不能用整数除法截断。
- `max_value` 返回最大的元素，返回类型是 `T`；用 `<` 比较，相等时返回任意一个。
- `int`、`double`、`long long`、`char` 数组必须同时支持 `average` 和 `max_value`；`std::string` 数组只需支持 `max_value`。
- 不要写 `main`。

### 提示

遍历数组时把元素累加到 `double` 变量中，再除以 `N`。`char` 的和不要存回 `char`，否则会溢出。`std::string` 数组只测试 `max_value`，不会实例化 `average`。
