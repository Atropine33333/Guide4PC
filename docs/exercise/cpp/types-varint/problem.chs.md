这一题不读 stdin，也不写 stdout。实现无符号 32 位整数的 LEB128 变长编码与解码。

```cpp
#include <cstddef>
#include <cstdint>
#include <vector>

std::vector<std::uint8_t> encode_varint(std::uint32_t value);

bool decode_varint(const std::vector<std::uint8_t>& bytes,
                   std::uint32_t& value,
                   std::size_t& consumed);
```

编码规则：

- 把整数按 7 位一组从低位向高位切开。
- 除最后一组外，每个字节的最高位（`0x80`）都置 1，表示后面还有字节。
- 最后一组的最高位为 0。
- `0` 编码成单个字节 `0x00`。

解码规则：

- 从 `bytes` 开头读取一个 LEB128 值。
- 成功时返回 `true`，把结果写入 `value`，把实际读取的字节数写入 `consumed`。允许后面还有不属于这个值的字节。
- 失败时返回 `false`，并且**不要修改** `value` 和 `consumed`。
- 以下情况失败：输入为空、字节在读完之前就用尽、超过 5 个字节仍然没有结束、第 5 个字节含有超出 32 位的高位、或者编码不是规范的（最短）形式。
- 规范形式示例：`{0x80, 0x00}` 和 `{0x81, 0x00}` 都表示 0/1，但不是最短形式，必须拒绝。
- 第 5 个字节只允许低 4 位有值，因此 `{0xff, 0xff, 0xff, 0xff, 0x0f}` 合法，而 `{0xff, 0xff, 0xff, 0xff, 0x10}` 溢出。

不要写 `main`，不要修改函数签名。
