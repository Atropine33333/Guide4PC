实现两个带默认参数的函数。

```cpp
int clamp_value(int value, int low = 0, int high = 100);
std::string join_text(const std::vector<std::string>& parts,
                      const std::string& separator = ",");
```

规则：

- `clamp_value` 把 `value` 限制在闭区间 `[low, high]` 内；如果 `low > high`，先交换两个边界再处理；三个参数都可能是负数。
- `join_text` 用 `separator` 把 `parts` 中的字符串依次连接起来；`parts` 为空时返回空串；空的元素要保留；`separator` 可以是空串。
- 默认参数只在函数定义里写一次；`clamp_value` 要能用 1 个、2 个和 3 个实参调用，`join_text` 要能用 1 个和 2 个实参调用。
- 不要写 `main`。

### 提示

默认参数在调用点生效：没写的实参由声明里的默认值补上。
