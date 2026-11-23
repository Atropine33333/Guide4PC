《错误码的秩序》

第一条 目的
传感器一共有四条，编号 `0` 到 `3`。每次登记把一条读数写进调用方给出的位置，写不进去就报出原因。

第二条 常量

```c
#define ERR_OK 0
#define ERR_ARG_NULL (-1)
#define ERR_ARG_RANGE (-2)

#define SENSOR_COUNT 4
#define READING_MAX 1000
```

第三条 接口

```c
int record_reading(int sensor, int value, int *out);
```

第四条 查验次序
先看 `out`，为空即返回 `ERR_ARG_NULL`。再看 `sensor`，落在 `0` 到 `SENSOR_COUNT - 1` 之外返回 `ERR_ARG_RANGE`。最后看 `value`，落在 `0` 到 `READING_MAX` 之外返回 `ERR_ARG_RANGE`。

第五条 登记
三项都通过时，把 `value` 写进 `*out`，返回 `ERR_OK`。

第六条 被拒
任何一次拒绝都不动 `*out`，调用方手里的值保持原样。

第七条 调用方式
`record_reading` 会被直接调用，请实现函数本身，不要写 `main`。

**人话**：按第四条的顺序查三个条件，合格就写 `*out` 并返回 `0`，不合格就返回 `-1` 或 `-2`，并且不要碰 `*out`。

**接口**：见第三条。

### 示例

```c
int v = -1;
record_reading(0, 25, &v);     // 0，v 为 25
record_reading(0, -1, &v);     // -2，v 仍是 25
record_reading(4, 10, &v);     // -2，编号越界
record_reading(1, 10, 0);      // -1，输出指针为空
```

### 约定

- 出错时返回错误码，不要打印任何内容。
- 先判 `out`，保证 `*out` 在任何情况下都不会被解引用两次或误写。
- 只使用标准库，不要动态分配，不要写 `main`。
