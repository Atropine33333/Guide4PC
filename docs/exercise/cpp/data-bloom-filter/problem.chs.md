布隆过滤器使用须知

本过滤器使用须知如下，请照章填写。凡与须知不符者，过滤器不认。

第一条　本过滤器有 `bits` 个位，`hash_count` 为每次插入或查询所用的哈希次数。构造时参数小于 1 的，一律按 1 处理——数字再小，也得有位可坐。
第二条　`add` 把 `hash_count` 个哈希位置为 1。同一个值插多少次，点亮的都是同一批位，任何一位都不会被清回 0。
第三条　`maybe_contains` 只准说两句话：“可能存在”与“一定不存在”。凡是 `add` 过的值，必须答“可能存在”。答成“一定不存在”，即为假阴性，本过滤器不能容忍。
第四条　允许假阳性——没插过的值也可能被答“可能存在”。这是本过滤器的本性，不必修，也修不掉。
第五条　`set_bits()` 报出当前为 1 的位数，`bit_count()` 报出总位数。两数须与位图一致。
第六条　本过滤器不提供删除，位图也不对外公开。
第七条　不读 stdin，不写 stdout，不改动公开接口，不写 `main`。须知至此为止，不再解释。

**人话**：实现一个固定大小的布隆过滤器 `BloomFilter`，不读 stdin、不写 stdout，测试在隐藏的 harness 里。`bits` 是位数组长度，`hash_count` 是每个元素使用的哈希次数，两者小于 1 时都按 1 处理；`add` 用 `hash_count` 个确定性哈希把对应位置 1，重复插入只重复点亮，不清任何位；`maybe_contains` 允许误报（假阳性），但绝不允许漏报（假阴性），插入过的值必须返回 `true`；`set_bits` 返回当前为 1 的位数，`bit_count` 返回总位数。不要求删除，也不要改公开接口，**不要写 `main`**。

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

### 规则

- 过滤器有 `bits` 个位。构造函数参数小于 1 时分别按 1 处理。
- `hash_count` 是每次插入/查询使用的探测次数；用任意确定性的字符串哈希实现即可。
- `add` 把对应的位设为 1，重复插入同一个值不会清除任何位。
- `maybe_contains` 只能返回“可能存在”或“一定不存在”；对已经插入过的值必须返回 `true`（不能有假阴性）。
- `set_bits()` 返回当前为 1 的位数，`bit_count()` 返回总位数。
- 不要求删除，也不要求位图能被外部访问。
- 不要写 `main`，不要修改公开接口。
