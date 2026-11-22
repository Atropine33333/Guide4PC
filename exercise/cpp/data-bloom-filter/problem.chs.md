这一题不读 stdin，也不写 stdout。实现一个固定大小的布隆过滤器（Bloom filter）。

```cpp
#include <cstddef>
#include <string>

class BloomFilter {
public:
    explicit BloomFilter(std::size_t bits, std::size_t hash_count);

    void add(const std::string& value);
    bool maybe_contains(const std::string& value) const;
    std::size_t set_bits() const;
    std::size_t bit_count() const;
};
```

规则：

- 过滤器有 `bits` 个位。构造函数参数小于 1 时分别按 1 处理。
- `hash_count` 是每次插入/查询使用的探测次数；用任意确定性的字符串哈希实现即可。
- `add` 把对应的位设为 1，重复插入同一个值不会清除任何位。
- `maybe_contains` 只能返回“可能存在”或“一定不存在”；对已经插入过的值必须返回 `true`（不能有假阴性）。
- `set_bits()` 返回当前为 1 的位数，`bit_count()` 返回总位数。
- 不要求删除，也不要求位图能被外部访问。
- 不要写 `main`，不要修改公开接口。
