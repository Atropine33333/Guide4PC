实现一种简单的 TLV（tag-length-value）记录编码与解码。

```cpp
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

struct DecodeResult {
    bool ok;
    std::vector<std::pair<unsigned char, std::string>> records;
    std::size_t bad_offset;
};

std::vector<unsigned char> encode_records(
    const std::vector<std::pair<unsigned char, std::string>>& records);

DecodeResult decode_records(const std::vector<unsigned char>& data);
```

编码格式：每条记录依次是

1. 一个字节的 tag；
2. 一个字节的长度 `len`（payload 的字节数，0 到 255）；
3. 恰好 `len` 个 payload 字节。

记录之间没有分隔符，整体也没有头部。

约定：

- `encode_records` 按输入顺序拼接记录；payload 长度不会超过 255。
- `decode_records` 从左到右解析。全部解析成功时 `ok = true`、`bad_offset = 0`。
- 如果剩余字节不足以构成完整记录（缺 tag、缺长度或 payload 被截断），`ok = false`，`records` 只包含出错之前成功解析的记录，`bad_offset` 是**出错记录的第一个字节**在原数组中的下标。
- 空输入是合法的，返回 `ok = true`、空 `records`、`bad_offset = 0`。
- 不要写 `main`。

### 示例

记录 `{(1, "A"), (2, "BC")}` 编码为字节 `01 01 41 02 02 42 43`。

对 `{01 01 41 02 05 42}` 解码：第一条记录成功，第二条声明长度 `5` 但只剩 `1` 个字节，所以 `ok = false`、`records = {(1, "A")}`、`bad_offset = 3`。

### 提示

用 `std::size_t offset` 遍历；每一步先检查 `data.size() - offset >= 2`，再检查 payload 是否完整。构造 `std::string` 时可以用 `std::string(begin, length)`。
