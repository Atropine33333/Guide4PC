实现三个查找与比较函数。

```cpp
#include <cstddef>
#include <string>
#include <vector>

std::size_t common_prefix(const std::string& a, const std::string& b);
long long last_index_of(const std::vector<int>& values, int target);
bool is_rotation(const std::string& a, const std::string& b);
```

约定：

- `common_prefix` 返回两个字符串相同前缀的长度；任一为空，或首字符不同时为 `0`。
- `last_index_of` 返回 `target` 在 `values` 中最后一次出现的下标；不存在时返回 `-1`，空数组也返回 `-1`。
- `is_rotation` 判断 `b` 是否由 `a` 循环移位得到；长度不同返回 `false`，两个空串视为互为旋转。
- 不要写 `main`。

### 提示

`std::mismatch` 可以同时遍历两个范围并停在第一处不同。`is_rotation` 可以先比较长度，再用 `(a + a).find(b)`；注意 `find("")` 总是返回 `0`，所以长度检查不能省。
