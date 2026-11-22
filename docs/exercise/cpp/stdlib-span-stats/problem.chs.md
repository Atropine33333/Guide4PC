内存资源管理办公室文件

内存办发〔2026〕8 号

关于开展连续内存段只读统计与裁剪工作的通知

各 span、各视图：

为进一步规范连续内存段的只读检查，杜绝越界访问、所有权混淆与悬空视图，经研究，决定开展本次专项工作。现将有关事项通知如下。

一、各单元应当按照下列接口实现函数，不得擅自增删签名。

```cpp
#include <cstddef>
#include <span>
#include <vector>

struct Stats {
    std::size_t count;
    long long sum;
    int min;
    int max;
};

Stats summarize(std::span<const int> values);
std::span<const int> trim_zeros(std::span<const int> values);
```

二、`summarize` 应当统计元素个数、总和、最小值与最大值；总和须用 `long long` 累加，防止溢出。空输入返回 `{0, 0, 0, 0}`。

三、`trim_zeros` 应当去掉开头与结尾的连续零，返回原内存上的子视图；全部为零或输入为空时，返回空视图。

四、返回的视图不得自己拥有数据，不得指向新建的 `std::vector`。

五、请勿编写 `main`。

本通知自印发之日起执行。

内存资源管理办公室
2026 年 11 月 22 日

抄送：只读视图监督委员会，指针别名核查组。

**人话**：实现 `summarize` 与 `trim_zeros`。`summarize` 返回元素个数、总和、最小值、最大值，空输入返回 `{0, 0, 0, 0}`，总和用 `long long` 累加。`trim_zeros` 去掉首尾连续的零并返回原缓冲区上的 `std::span` 子视图；全零或空输入返回空视图。返回的视图不得拥有数据，也不得指向临时容器。`subspan(offset, count)` 仍指向原缓冲区，可用于裁剪。
