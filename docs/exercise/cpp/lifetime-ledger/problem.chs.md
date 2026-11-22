内存编制管理办公室文件

内存编发〔2026〕19 号

关于加强对象存续登记工作的通知

各作用域、各临时对象：

为摸清在册对象底数，杜绝漏登、重登与虚报，现就将 `Tracked` 类纳入存续登记管理有关事项通知如下。

一、登记范围。凡 `Tracked` 之对象，自构造之时登记在册，至析构之时注销；拷贝构造另立一户。本题不读 stdin，不写 stdout，账目由 harness 核验。

二、接口与口径，不得增删。

- `Tracked()`：构造，值为 `0`。
- `explicit Tracked(int value)`：构造，值为 `value`。
- `Tracked(const Tracked& other)`：拷贝构造，复制值；这是一个**新对象**。
- `Tracked& operator=(const Tracked& other)`：赋值，复制值；对象数量不变，自赋值安全。
- `~Tracked()`：析构。
- `int value() const` 与 `void setValue(int value)`：读写值。
- `static int alive()`：返回当前存活的对象个数。

三、台账纪律。初始存活数为 `0`；构造与拷贝构造各记增一；析构记减一；赋值不增不减，自赋值亦然。

四、核验方式。上级 harness 在嵌套作用域中创建对象、拷贝对象、放入数组，作用域结束时核查计数归零。

五、请勿自行编写 `main`。本通知自印发之日起执行。

抄送：栈管理委员会，堆管理委员会，析构清算小组。

**人话**：实现 `Tracked`：构造与拷贝构造让 `alive()` 加一，析构减一，赋值不改计数且自赋值安全；值经 `value()` / `setValue()` 存取。harness 造对象、拷贝、放入数组，离开作用域后要求 `alive()` 归零。
