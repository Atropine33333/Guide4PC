（客厅。壁炉。两位绅士各执一杯。）

“哦，我亲爱的，你看这份手稿，我们两人竟共用同一叠纸。”

“老天爷，那可省事了——落笔的时候，谁也别把墨水滴到对方那一页上。”

“所以叫写时复制。拷贝构造、拷贝赋值，只把这份纸记在名下，谁也不另抄一份，`live_blocks()` 不动。”

“`shares()` 呢？”

“数一数有几位先生共用这叠纸，算上自己，至少一位。”

“可我要添一句话。`append` 一来，就得先看看是否独享；`shares() > 1` 的，先给自己另抄一份，再落笔。别人的稿子，一个字也不许改。”

“哦，那要是我把自己赋值给自己呢？”

“亲爱的，那就什么也别做。至于被换下去的那叠纸，没人用了，就当场烧掉。”

“空稿子呢？”

“空稿子也得有一叠纸，`live_blocks()` 照记。”

**人话**：实现 `CowBuffer`：拷贝构造和拷贝赋值只共享底层存储，不复制字符串，也不增加 `live_blocks()`；`shares()` 返回共享同一块存储的对象数，含自身，至少为 1；`append` 前先确保存储唯一，`shares() > 1` 时先复制再追加，原共享对象的字符串不变；`live_blocks()` 返回当前存活的互不相同的存储块数；拷贝赋值要处理自赋值，被替换且已无对象使用的存储及时释放；空缓冲区也必须有一块有效存储。不写 `main`，不改公开接口。

```cpp
#include <cstddef>
#include <string>

class CowBuffer {
public:
    CowBuffer();
    explicit CowBuffer(const std::string& data);
    CowBuffer(const CowBuffer& other);
    CowBuffer& operator=(const CowBuffer& other);
    ~CowBuffer();

    const std::string& str() const;
    void append(const std::string& suffix);
    std::size_t shares() const;

    static int live_blocks();
};
```
