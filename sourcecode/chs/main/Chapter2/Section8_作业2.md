# 第8节：作业2

> **说明**：本作业完全脱离本教程中的 PDE 数值计算背景，聚焦于 **现代 C++（C++17/20/23）语言特性、数据结构与算法实现、异常安全保证、以及工程架构设计模式**。
>
> 这不是一份为了应付大学考试而设计的“刷题卷”，难度很高，推荐先进行一段时间的学习再来看看，或者用这份作业当作学习方向指导。
>
> **作业结构**：每题后附有「思路点拨」，为解题提供方向性指引（不是答案）。完整的参考答案集中附于文末。建议先独立思考并编码实现。
>
> 看来我的水平还是在k老师之下很多，我写的初版答案被k老师一通抨击。"基本思路正确，但严重不足"

---

## 一、语言特性（6 题）

---

**1. const 与引用折叠**

现有以下模板函数：
```cpp
template<typename T>
void forward(T&& arg) {
    // 函数体
}
```
（a）当 `forward` 被调用时，`T&&` 是什么引用类型？请解释引用折叠规则。  
（b）如果传入一个左值 `int a = 5;`，`T` 会被推导为什么类型？如果传入一个右值 `forward(10);`，`T` 又是什么类型？

> **思路点拨**：
> - `T&&` 在模板推导语境下有一个专门的名称，它既不是普通的右值引用也不是左值引用。
> - 引用折叠发生在模板参数推导与显式类型指定结合时。可以用一张 2×2 表格来推演：`&` 与 `&&` 相遇时，`&` 胜出。
> - 尝试分别用一个左值变量和一个字面常量调用 `forward`，然后问自己：`T` 被推导成了什么？如果 `T` 是 `int&`，那么 `T&&` 折叠后是什么？

---

**2. 移动语义与 std::move 的本质**

（a）`std::move` 做了什么事？它是如何将左值转为右值引用的？  
（b）假设有一个类 `Buffer`，包含 `std::vector<int> data`。实现它的移动构造函数和移动赋值运算符（要求处理自赋值），并说明为什么移动后应将源对象置于“有效但未指定”的状态。

> **思路点拨**：
> - `std::move` 实际上不移动任何东西，它只是一个编译时的类型转换工具。
> - 移动操作的本质是“窃取”资源，而不是复制。窃取后，源对象应该不再拥有该资源，否则析构时会重复释放。
> - 处理自赋值时，检查 `this != &other` 是标准做法。C++14 起可以用 `std::exchange` 让代码更简洁。
> - 思考：如果移动后不置空源对象的指针，会发生什么？

---

**3. lambda 捕获与生命周期**

解释下面代码的可能风险，并提出两种修复方案：
```cpp
std::function<int()> makeCounter() {
    int count = 0;
    return [&]() { return ++count; };
}
int main() {
    auto f = makeCounter();
    std::cout << f();
}
```

> **思路点拨**：
> - `makeCounter` 返回后，局部变量 `count` 发生了什么？lambda 捕获的是引用还是副本？
> - “悬垂引用”这个概念在这里很关键。如果 lambda 在函数返回后仍被调用，它访问的内存已经无效。
> - 方案一：让 lambda 捕获值的副本，但注意 `mutable` 关键字的作用。
> - 方案二：用 `std::shared_ptr` 将计数器放在堆上，让 lambda 和计数器共享生命周期。
> - C++23 的 `std::move_only_function` 与 `std::function` 在此场景下的区别是什么？

---

**4. SFINAE 与概念（Concepts）**

你需要编写一个模板函数 `toString`，它能将任何类型转换为 `std::string`，但只对**算术类型**（整数、浮点数）生效，对其他类型则启用另一个重载。分别用 C++17 的 `std::enable_if` 和 C++20 的 `concepts` 给出实现。

> **思路点拨**：
> - SFINAE 的全称是“替换失败并非错误”。关键在于让不满足条件的重载在模板参数替换时失败，从而被从重载集中移除。
> - 使用 `std::enable_if_t` 时，放在返回类型位置比放在默认模板参数位置更安全，可以避免重载歧义。
> - C++20 的 Concepts 提供了更清晰的语法。定义一个 `Arithmetic` 概念，然后在函数模板上用 `requires` 或简写语法约束。
> - 注意：两个重载必须互斥，否则会产生二义性。

---

**5. 可变参数模板与折叠表达式**

请实现一个函数 `sum_all`，它可以接受任意数量的数值参数，并返回它们的和。要求：
- 使用 C++17 的折叠表达式。
- 至少接受一个参数（拒绝空参数包）。
- 返回类型为所有参数的公共类型（例如 `int` 和 `double` 混合时返回 `double`）。

> **思路点拨**：
> - 折叠表达式有四种形式：一元左折叠、一元右折叠、二元左折叠、二元右折叠。
> - 如果参数包可能为空，一元折叠会编译失败。用至少一个参数的模板头可以强制非空。
> - `std::common_type_t` 可以计算参数包的公共类型，但注意它要求所有类型都能隐式转换到同一个类型。
> - 二元折叠可以指定初始值，例如 `(args + ... + 0)`，但这里要求至少一个参数，所以一元折叠即可。

---

**6. 自定义字面量**

C++11 允许定义用户字面量后缀。请实现一个字面量后缀 `_km`，将 `10_km` 转换为 `double` 类型的米数（1 km = 1000 m）。写出完整的运算符重载函数，并用 `consteval`（C++20）强制编译期求值。

> **思路点拨**：
> - 用户定义字面量的后缀名必须以 `_` 开头（不带下划线的后缀保留给标准库）。
> - 字面量运算符的签名取决于字面量的类型：整数字面量、浮点字面量、字符字面量、字符串字面量各有不同的重载形式。
> - `constexpr` 允许编译期求值，`consteval`（C++20）强制编译期求值——如果无法在编译期计算则会报错。
> - 使用 `constexpr auto meters = 10.0_km;` 可以在编译期完成单位转换。

---

## 二、数据结构与算法实现（6 题）

---

**7. 循环缓冲区（Circular Buffer）**

设计一个类 `CircularBuffer<T>`，容量固定（构造时指定），支持：
- `push_back(const T&)`：若缓冲区满，覆盖最旧元素。
- `pop_front()`：移除最旧元素（空时抛异常）。
- `size()` 和 `capacity()`。
- `operator[](size_t idx)` 随机访问（按存储顺序，越界抛异常）。

要求使用 `std::vector` 作为底层存储，正确处理索引环绕，并区分“空”与“满”状态。

> **思路点拨**：
> - 用 `head` 和 `tail` 两个指针（索引）分别指向队首和队尾。当 `head == tail` 时，队列可能是空的，也可能是满的——需要额外变量区分。
> - 常见的三种区分方式：维护 `count` 变量、牺牲一个槽位、使用 `bool` 标志。推荐 `count` 方式最直观。
> - `tail = (tail + 1) % capacity` 是环绕的关键。C++ 的 `%` 运算在 `size_t` 上安全。
> - 如果缓冲区满时执行 `push_back`，需要覆盖最旧元素——此时 `head` 也要前移。

---

**8. 手动实现 shared_ptr**

手动实现一个简化版 `SharedPtr<T>`，包含：
- 构造函数（从原始指针）、析构函数。
- 拷贝构造、拷贝赋值、移动构造、移动赋值。
- `get()`、`use_count()`、`reset()`。
- `operator*` 和 `operator->`。
- 显式 `operator bool`。

使用独立的引用计数控制块，暂不考虑线程安全。

> **思路点拨**：
> - 引用计数控制块应该包含一个 `size_t` 计数器，独立于被管理的对象分配在堆上。
> - 拷贝构造时递增引用计数，析构时递减。当计数归零时，同时删除对象和控制块。
> - 移动构造时直接“窃取”指针和计数，并将源对象置空。移动后源对象的 `use_count()` 应为 0。
> - `reset()` 需要释放当前资源，然后接管新资源（如果有）。
> - `operator*` 返回引用，`operator->` 返回指针。`operator bool` 应标记为 `explicit` 以防止意外转换。

---

**9. LRU 缓存**

设计一个 `LRUCache<Key, Value>` 类，容量固定，提供 `get(key)` 和 `put(key, value)`，时间复杂度要求 O(1)。请使用 `std::unordered_map` 和 `std::list` 实现，并确保 `get` 和 `put` 的操作都会更新访问顺序（最近使用的移到最前）。

> **思路点拨**：
> - 核心数据结构组合：`std::list` 存储键值对（按访问顺序排列），`std::unordered_map` 存储键到链表节点的迭代器。
> - `get` 时如果键存在，将对应节点 `splice` 到链表头部，然后返回值。
> - `put` 时如果键已存在，更新值并将节点移到头部；如果键不存在且缓存已满，删除链表尾部节点（最久未使用），再从 map 中移除，然后插入新节点到头部。
> - `std::list::splice` 是 O(1) 操作，不会复制或移动元素。
> - 容量为 0 应该被拒绝（构造时抛异常）。

---

**10. 线程安全的阻塞队列**

设计一个模板类 `BlockingQueue<T>`，支持：
- `push(T value)`：入队，若队列已满则阻塞等待。
- `pop(T& out)`：出队，若队列空则阻塞等待。
- `try_pop()` 返回 `std::optional<T>`（非阻塞）。
- `stop()`：唤醒所有等待线程并拒绝新操作。

请使用 `std::mutex`、`std::condition_variable` 和 `std::atomic<bool>` 实现。

> **思路点拨**：
> - 需要两个条件变量：`not_empty`（非空时唤醒消费者）和 `not_full`（非满时唤醒生产者）。
> - 使用 `std::unique_lock` 配合条件变量等待，注意 wait 的谓词要检查停止标志。
> - `stop()` 设置 `stopped = true` 后要 `notify_all()`，唤醒所有等待的线程。
> - `try_pop` 用 `std::lock_guard` 加锁，检查队列非空则出队，否则返回 `std::nullopt`。
> - 思考：为什么 `std::atomic<bool>` 还需要配合互斥锁使用？如果在互斥锁外检查停止标志，会发生什么？

---

**11. 简易 std::function 类型擦除**

写一个类 `Function<Signature>`，它能存储任何可调用对象（函数指针、lambda、函数对象），并重载 `operator()` 来调用它。要求**不使用 `std::function`**，自行实现类型擦除（通过模板 + 抽象基类 + `std::unique_ptr`）。

例如：
```cpp
Function<int(double, double)> f = [](double a, double b) { return (int)(a + b); };
int result = f(1.5, 2.5);  // result = 4
```

> **思路点拨**：
> - 类型擦除的核心：用一个非模板基类定义接口（虚函数 `invoke`），用模板派生类存储具体的可调用对象。
> - 基类指针（`std::unique_ptr<CallableBase>`）擦除了具体类型，`operator()` 通过虚函数调用 `invoke`。
> - 拷贝构造需要 `clone()` 虚函数来拷贝派生类对象，因为 `unique_ptr` 不可拷贝。
> - 移动构造可以直接 `= default`，因为 `unique_ptr` 支持移动。
> - 如果 `callable` 为空，`operator()` 应抛出 `std::bad_function_call`。

---

**12. 二叉树中序遍历迭代器**

给定二叉树节点：
```cpp
struct Node { int value; Node* left; Node* right; };
```
请编写一个类 `InorderIterator`，它接受根节点指针，并实现 `operator++`、`operator*`、`operator!=`。再封装一个 `InorderRange` 类，支持基于范围的 `for` 循环：
```cpp
for (int val : InorderRange(root)) { std::cout << val << " "; }
```
要求使用栈模拟递归，时间复杂度 O(1) 平均每次递增。

> **思路点拨**：
> - 中序遍历顺序：左子树 → 根节点 → 右子树。用栈模拟递归：先一直向左深入，将节点压栈。
> - 迭代器的 `current` 指向当前节点。`operator++` 处理右子树：先压入右子树的所有左节点，然后弹出栈顶作为下一个 `current`。
> - `begin()` 应构造一个迭代器，执行到第一个节点（最左节点）。`end()` 应返回一个 `current == nullptr` 的迭代器。
> - 迭代器类型应满足 `std::input_iterator` 的要求，定义 `iterator_category` 等别名。
> - 注意：`operator*` 返回 `const int&` 以确保只读。

---

## 三、异常安全与资源管理（4 题）

---

**13. RAII 文件锁**

假设系统提供了 `lock_file(const char* path)` 和 `unlock_file(const char* path)` 两个 C 函数（成功返回 0，失败返回 -1）。请设计一个 RAII 类 `FileLock`，在构造时加锁，析构时解锁，并处理加锁失败（抛出 `std::runtime_error`）。同时禁止拷贝，支持移动语义。

> **思路点拨**：
> - RAII 的核心是“资源获取即初始化”——构造函数获取资源，析构函数释放资源。
> - 加锁失败时抛出异常，确保不会构造出一个“未锁定”的 FileLock 对象。
> - 禁止拷贝是因为文件锁不能被复制（复制两个持有同一锁的对象会导致重复释放）。
> - 移动语义允许转移所有权：移动后源对象的 `locked` 标志置为 `false`，析构时不再释放锁。
> - 移动赋值需要先释放当前锁，再接管源对象的锁。

---

**14. 异常安全的拷贝赋值**

为 `String` 类（内部 `char*` 动态数组，存储 C 风格字符串）编写一个**异常安全**的拷贝赋值运算符，要求使用 **copy-and-swap** 惯用法，提供强异常安全保证。请解释为什么这种写法能保证赋值失败时原对象状态不变。

> **思路点拨**：
> - “强异常安全保证”意味着：如果操作抛出异常，程序状态保持不变。
> - Copy-and-swap 的核心：用拷贝构造创建临时副本（可能抛异常），然后用不抛异常的 `swap` 交换临时副本和 `this`。
> - 关键点是：如果拷贝构造失败，`this` 完全不受影响（因为还没开始修改 `this`）。
> - `swap` 只交换指针和长度，不分配内存，因此可以标记为 `noexcept`。
> - 移动赋值可以直接实现（不需要 copy-and-swap），因为移动操作通常也是 `noexcept` 的。

---

**15. noexcept 的正确使用**

（a）什么情况下应该给函数标记 `noexcept`？什么情况下不应该？  
（b）移动构造函数通常应标记 `noexcept`，为什么？如果不标记，会对 `std::vector` 的扩容操作产生什么性能影响？

> **思路点拨**：
> - `noexcept` 不仅是一个优化提示，还影响容器的异常安全策略。
> - 析构函数、移动操作、`swap` 通常应该是 `noexcept` 的。
> - 如果函数可能抛出异常（如内存分配、文件操作），标记 `noexcept` 会导致程序在异常发生时直接调用 `std::terminate`。
> - `std::vector` 扩容时，如果元素的移动构造是 `noexcept`，则使用移动；否则使用拷贝——这是性能差异的关键来源。

---

**16. 内存池（Object Pool）**

设计一个简单的 `ObjectPool<T>`，它预先分配一块连续内存（例如 `std::vector` 存储原始字节），并提供 `allocate(Args&&...)` 和 `deallocate(T*)` 来复用对象。要求：
- 避免频繁的 `new/delete`。
- 支持任意类型的 `T`（使用 placement new 和 `std::construct_at`，C++20）。
- 实现自动扩容（当池满时翻倍）。
- （可选加分）使用 `std::shared_mutex` 实现线程安全。

> **思路点拨**：
> - 内存池预先分配一大块内存，用空闲链表（free list）管理可用块。
> - `allocate` 从 free list 取出一块内存，用 placement new 构造对象。C++20 的 `std::construct_at` 是推荐用法。
> - `deallocate` 调用析构函数（`std::destroy_at`，C++17），将块放回 free list。
> - 扩容时分配更大的 `vector`，将新块加入 free list。注意已有对象不受影响（它们在旧内存中）。
> - 线程安全版本用 `std::shared_mutex`：`allocate/deallocate` 使用独占锁，`size()` 等只读操作使用共享锁。

---

## 四、架构与设计模式（4 题）

---

**17. 策略模式（排序算法可切换）**

设计一个 `SortEngine` 类，它拥有一个排序策略接口，可以在运行时切换排序算法（如冒泡、快排、插入排序）。要求：
- 定义 `ISorter` 抽象类，含 `sort(std::vector<int>&)` 纯虚函数。
- 实现至少两个具体策略类。
- `SortEngine` 提供 `setStrategy(std::unique_ptr<ISorter>)` 和 `execute(std::vector<int>&)` 方法。

> **思路点拨**：
> - 策略模式将算法封装在独立的类中，使算法可以相互替换。
> - 定义抽象接口 `ISorter`，具体策略（`QuickSort`、`BubbleSort`）实现该接口。
> - `SortEngine` 持有一个 `std::unique_ptr<ISorter>`，通过 `setStrategy` 切换策略。
> - `execute` 调用当前策略的 `sort` 方法。
> - 思考：如果策略不需要状态，是否可以用函数指针或 `std::function` 替代？

---

**18. 观察者模式（事件系统）**

设计一个简单的事件系统，支持：
- `EventDispatcher` 单例，提供 `register(eventType, callback)` 和 `dispatch(eventType, data)`。
- 多个监听器可注册同一事件类型，回调函数接收 `const std::string& data`。
- 要求使用 `std::unordered_map` 映射事件类型（`std::string`）到回调列表（`std::vector<std::function<void(const std::string&)>>`）。
- **关键难点**：处理回调中可能增删监听器的情况（提示：分发时拷贝回调列表，避免迭代器失效）。

> **思路点拨**：
> - 单例可以用 Meyers 单例实现（C++11 起线程安全）。
> - `register` 需要线程安全（如果多线程环境）。可以用 `std::mutex` 保护 `listeners`。
> - `dispatch` 时拷贝回调列表，然后在新副本上遍历。这样即使回调中修改了 `listeners`，也不会影响正在进行的遍历。
> - 如果不拷贝，而是直接用迭代器遍历，回调中注册新监听器可能导致迭代器失效（`vector` 重新分配）。
> - 考虑：如何支持移除监听器？可以用 `std::weak_ptr` 包装回调，或者返回 token 用于取消注册。

---

**19. 依赖注入 + 工厂模式**

你正在开发一个游戏，需要创建不同种类的敌人（`Goblin`、`Orc`、`Dragon`）。所有敌人都实现 `IEnemy` 接口（含 `attack()` 方法）。
- 设计一个 `EnemyFactory`，它根据字符串参数（如 `"goblin"`）返回对应的 `std::unique_ptr<IEnemy>`。工厂内部使用 `std::unordered_map` 注册创建器。
- 设计一个 `Game` 类，它通过**构造函数注入**接收 `EnemyFactory` 的引用，并在运行时使用工厂生成敌人。
- 说明这种设计对单元测试有什么好处。

> **思路点拨**：
> - 工厂模式将对象的创建与使用分离。`EnemyFactory` 维护一个从类型名到创建函数（lambda）的映射。
> - 依赖注入（构造函数注入）将 `Game` 的依赖（`EnemyFactory`）从外部传入，而不是在 `Game` 内部创建。
> - 好处：`Game` 不直接依赖具体的敌人类（`Goblin`、`Orc`），只依赖工厂接口。
> - 单元测试时，可以注入一个 MockFactory，它返回假敌人或记录调用次数，验证 `Game` 的逻辑是否正确。

---

**20. 编译时多态（CRTP） vs 运行时多态（虚函数）**

在 C++ 中，我们可以通过模板参数传递策略（编译时多态），也可以通过基类指针传递（运行时多态）。
（a）分别说明这两种方式的适用场景和性能差异。
（b）如果你需要实现一个容器，其排序策略在编译期就确定且不会改变，你选哪种方式？如果排序策略需要在运行时由用户通过配置文件选择，你又选哪种？请分别给出两种设计的代码骨架。

> **思路点拨**：
> - 编译时多态（模板/CRTP）在编译期确定类型，无虚函数调用开销，可以内联，但无法在运行时改变策略。
> - 运行时多态（虚函数）有 `vtable` 间接调用开销，可能阻止内联，但策略可以在运行时切换。
> - 编译期确定策略：用模板参数传递排序策略类型，容器内部持有该类型的对象，调用 `sort()` 是静态绑定的。
> - 运行时确定策略：用基类指针指向具体策略对象，通过虚函数调用 `sort()`。可以用配置文件中的字符串决定创建哪个策略对象。
> - 选型决策：如果策略固定且对性能敏感（如计算密集型的核心循环），选编译时多态；如果策略需要由用户配置或插件动态加载，选运行时多态。

---

---

# 参考答案

> **说明**：参考答案提供完整的思路和核心代码实现。对于部分题目，给出的是可编译的核心骨架代码。建议先独立完成后再对照。

---

## 一、语言特性深潜

### 第 1 题

**（a）** `T&&` 在模板参数推导语境下是**转发引用（forwarding reference）**，也称为万能引用。它不是普通的右值引用，而是能够根据传入参数是左值还是右值来调整自身类型的引用。

引用折叠规则：
- `T& &` → `T&`
- `T& &&` → `T&`
- `T&& &` → `T&`
- `T&& &&` → `T&&`

当左值传入时，`T` 被推导为左值引用类型，`T&&` 折叠为 `T&`；当右值传入时，`T` 被推导为非引用类型，`T&&` 保持为 `T&&`。

**（b）** 传入左值 `int a = 5;` 时，`T` 推导为 `int&`，`T&&` 折叠为 `int&`（左值引用）。传入右值 `10` 时，`T` 推导为 `int`，`T&&` 为 `int&&`（右值引用）。

---

### 第 2 题

**（a）** `std::move` 本质上是一个编译时的类型转换工具，它将左值强制转换为右值引用。其核心实现约等于 `static_cast<T&&>(arg)`，不产生任何运行时代码，也不执行任何移动操作。真正的移动操作发生在调用移动构造函数或移动赋值运算符时。

**（b）**
```cpp
class Buffer {
    std::vector<int> data;
public:
    Buffer() = default;
    
    Buffer(Buffer&& other) noexcept 
        : data(std::exchange(other.data, {})) {}
    
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            data = std::exchange(other.data, {});
        }
        return *this;
    }
};
```

移动后应将源对象置于“有效但未指定”的状态。对于 `std::vector`，这意味着将其置为空向量。这样做是为了保证源对象在析构时不会重复释放资源，同时源对象仍然可以被安全地析构或重新赋值。`std::exchange` 是 C++14 引入的工具，它将 `other.data` 置空并返回旧值，一步完成。

---

### 第 3 题

**风险分析**：`makeCounter` 返回的 lambda 以引用方式捕获了局部变量 `count`。当函数返回后，`count` 被销毁，lambda 持有的引用变为悬垂引用。调用 `f()` 时访问已销毁的内存，导致未定义行为。

**修复方案一**（值捕获）：
```cpp
std::function<int()> makeCounter() {
    int count = 0;
    return [count]() mutable { return ++count; };
}
```
`[count]` 按值捕获 `count` 的副本，`mutable` 允许在 lambda 体内修改该副本。

**修复方案二**（`shared_ptr` 动态分配）：
```cpp
std::function<int()> makeCounter() {
    auto count = std::make_shared<int>(0);
    return [count]() { return ++(*count); };
}
```
`count` 在堆上分配，lambda 持有 `shared_ptr`，生命周期与 lambda 绑定。

**补充（C++23）**：如果不需要拷贝语义，可以用 `std::move_only_function` 或直接返回 lambda（类型精确，无需类型擦除）。

---

### 第 4 题

**C++17 版本（返回类型 SFINAE）**：
```cpp
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, std::string>
toString(T val) { 
    return std::to_string(val); 
}

template<typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, std::string>
toString(const T& val) { 
    return std::string(val);  // 假设 T 可转换到 std::string
}
```

**C++20 Concepts 版本**：
```cpp
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic T>
std::string toString(T val) { 
    return std::to_string(val); 
}

template<typename T>
    requires (!Arithmetic<T>)
std::string toString(const T& val) { 
    return std::string(val);
}
```

**为什么不用默认模板参数的 `enable_if`**：`template<typename T, typename = std::enable_if_t<...>>` 方式在重载解析时可能产生歧义，当两个重载都能匹配时编译器无法选择。放在返回类型位置更安全。

---

### 第 5 题

```cpp
#include <type_traits>

template<typename T, typename... Rest>
std::common_type_t<T, Rest...> sum_all(T first, Rest... rest) {
    return (first + ... + rest);  // 二元左折叠，至少一个参数
}
```

**说明**：
- `(first + ... + rest)` 是二元左折叠，展开为 `((first + rest1) + rest2) + ...`
- `std::common_type_t<T, Rest...>` 计算所有参数类型的公共类型，例如 `int` 和 `double` 混合时返回 `double`。
- 模板参数至少包含一个类型 `T`，强制调用时至少传一个参数。

---

### 第 6 题

```cpp
consteval long double operator"" _km(long double km) {
    return km * 1000.0L;
}

// 使用（编译期求值）
constexpr auto meters = 10.0_km;  // 10000.0L
```

**说明**：
- 字面量后缀必须以 `_` 开头（不带下划线的后缀保留给标准库）。
- `consteval`（C++20）强制编译期求值——如果不能在编译期计算，则编译报错。
- `constexpr` 版本（C++11 即可）则允许编译期或运行时求值。

---

## 二、数据结构与算法实现

### 第 7 题

```cpp
#include <vector>
#include <stdexcept>

template<typename T>
class CircularBuffer {
    std::vector<T> buf;
    size_t head = 0;   // 最旧元素位置
    size_t tail = 0;   // 下一个写入位置
    size_t count = 0;  // 当前元素数量（区分空/满）
    
public:
    explicit CircularBuffer(size_t cap) : buf(cap) {}
    
    void push_back(const T& value) {
        if (buf.empty()) throw std::runtime_error("Buffer has zero capacity");
        
        buf[tail] = value;
        tail = (tail + 1) % buf.size();
        
        if (count == buf.size()) {
            head = tail;   // 覆盖最旧元素，head 跟进
        } else {
            ++count;
        }
    }
    
    void pop_front() {
        if (count == 0) throw std::runtime_error("Buffer is empty");
        head = (head + 1) % buf.size();
        --count;
    }
    
    [[nodiscard]] size_t size() const noexcept { return count; }
    [[nodiscard]] size_t capacity() const noexcept { return buf.size(); }
    
    T& operator[](size_t idx) {
        if (idx >= count) throw std::out_of_range("Index out of range");
        return buf[(head + idx) % buf.size()];
    }
    
    const T& operator[](size_t idx) const {
        if (idx >= count) throw std::out_of_range("Index out of range");
        return buf[(head + idx) % buf.size()];
    }
};
```

---

### 第 8 题

```cpp
#include <utility>

template<typename T>
class SharedPtr {
    T* ptr = nullptr;
    size_t* ref_count = nullptr;
    
    void release() noexcept {
        if (ref_count) {
            if (--(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }
    
public:
    explicit SharedPtr(T* p = nullptr) 
        : ptr(p), ref_count(p ? new size_t(1) : nullptr) {}
    
    ~SharedPtr() { release(); }
    
    SharedPtr(const SharedPtr& other) 
        : ptr(other.ptr), ref_count(other.ref_count) {
        if (ref_count) ++(*ref_count);
    }
    
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }
    
    SharedPtr(SharedPtr&& other) noexcept 
        : ptr(std::exchange(other.ptr, nullptr))
        , ref_count(std::exchange(other.ref_count, nullptr)) {}
    
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = std::exchange(other.ptr, nullptr);
            ref_count = std::exchange(other.ref_count, nullptr);
        }
        return *this;
    }
    
    T* get() const noexcept { return ptr; }
    size_t use_count() const noexcept { return ref_count ? *ref_count : 0; }
    
    void reset(T* p = nullptr) {
        release();
        ptr = p;
        ref_count = p ? new size_t(1) : nullptr;
    }
    
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    explicit operator bool() const noexcept { return ptr != nullptr; }
};
```

---

### 第 9 题

```cpp
#include <list>
#include <unordered_map>
#include <stdexcept>

template<typename Key, typename Value>
class LRUCache {
    using ListIter = typename std::list<std::pair<Key, Value>>::iterator;
    
    size_t cap;
    std::list<std::pair<Key, Value>> items;      // 前端=最新，后端=最旧
    std::unordered_map<Key, ListIter> map;
    
public:
    explicit LRUCache(size_t capacity) : cap(capacity) {
        if (cap == 0) throw std::invalid_argument("Capacity must be > 0");
    }
    
    Value* get(const Key& key) {
        auto it = map.find(key);
        if (it == map.end()) return nullptr;
        
        // 移到队首
        items.splice(items.begin(), items, it->second);
        return &it->second->second;
    }
    
    void put(const Key& key, const Value& value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }
        
        if (items.size() >= cap) {
            map.erase(items.back().first);
            items.pop_back();
        }
        
        items.emplace_front(key, value);
        map[key] = items.begin();
    }
};
```

---

### 第 10 题

```cpp
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <optional>

template<typename T>
class BlockingQueue {
    std::queue<T> queue;
    mutable std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    size_t max_size;
    std::atomic<bool> stopped{false};
    
public:
    explicit BlockingQueue(size_t capacity) : max_size(capacity) {}
    
    void push(T value) {
        std::unique_lock lock(mtx);
        not_full.wait(lock, [this] { 
            return queue.size() < max_size || stopped.load(); 
        });
        if (stopped.load()) throw std::runtime_error("Queue is stopped");
        queue.push(std::move(value));
        not_empty.notify_one();
    }
    
    void pop(T& out) {
        std::unique_lock lock(mtx);
        not_empty.wait(lock, [this] { 
            return !queue.empty() || stopped.load(); 
        });
        if (queue.empty()) throw std::runtime_error("Queue is stopped and empty");
        out = std::move(queue.front());
        queue.pop();
        not_full.notify_one();
    }
    
    std::optional<T> try_pop() {
        std::unique_lock lock(mtx);
        if (queue.empty()) return std::nullopt;
        T val = std::move(queue.front());
        queue.pop();
        not_full.notify_one();
        return val;
    }
    
    bool try_push(T value) {
        std::unique_lock lock(mtx);
        if (queue.size() >= max_size || stopped.load()) return false;
        queue.push(std::move(value));
        not_empty.notify_one();
        return true;
    }
    
    void stop() {
        stopped.store(true);
        not_empty.notify_all();
        not_full.notify_all();
    }
    
    [[nodiscard]] bool empty() const {
        std::lock_guard lock(mtx);
        return queue.empty();
    }
    
    [[nodiscard]] size_t size() const {
        std::lock_guard lock(mtx);
        return queue.size();
    }
};
```

---

### 第 11 题

```cpp
#include <memory>
#include <utility>
#include <functional>

template<typename>
class Function;

template<typename R, typename... Args>
class Function<R(Args...)> {
    struct CallableBase {
        virtual ~CallableBase() = default;
        virtual R invoke(Args... args) const = 0;
        virtual std::unique_ptr<CallableBase> clone() const = 0;
    };

    template<typename F>
    struct Callable : CallableBase {
        mutable F func;  

        explicit Callable(F f) : func(std::move(f)) {}

        R invoke(Args... args) const override {
            return invoke_impl(std::move(args)...);
        }

        template<typename... CallArgs>
        R invoke_impl(CallArgs&&... args) const {
            return func(std::forward<CallArgs>(args)...);
        }

        std::unique_ptr<CallableBase> clone() const override {
            return std::make_unique<Callable>(func);
        }
    };

    std::unique_ptr<CallableBase> callable;

public:
    Function() = default;

    template<typename F>
    Function(F f) : callable(std::make_unique<Callable<F>>(std::move(f))) {}

    Function(const Function& other)
        : callable(other.callable ? other.callable->clone() : nullptr) {}

    Function& operator=(const Function& other) {
        if (this != &other) {
            callable = other.callable ? other.callable->clone() : nullptr;
        }
        return *this;
    }

    Function(Function&&) noexcept = default;
    Function& operator=(Function&&) noexcept = default;

    R operator()(Args... args) const {
        if (!callable) throw std::bad_function_call();
        return callable->invoke(std::move(args)...);
    }

    explicit operator bool() const noexcept { return callable != nullptr; }
};
```

使用例：

```cpp
// 1. 函数指针
Function<int(double, double)> f1 = [](double a, double b) { return int(a + b); };
int r1 = f1(1.5, 2.5);  // 4

// 2. 带 mutable 状态的 lambda
int offset = 10;
Function<int(int)> f2 = [offset](int x) mutable { return x + offset; };
int r2 = f2(5);  // 15

// 3. 左值引用参数（修正版可正确转发）
Function<void(std::string&)> f3 = [](std::string& s) { s += "!"; };
std::string msg = "hello";
f3(msg);  // msg == "hello!"
```

---

### 第 12 题

```cpp
#include <stack>
#include <iterator>

struct Node {
    int value;
    Node* left = nullptr;
    Node* right = nullptr;
};

class InorderIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;
    
private:
    std::stack<Node*> stk;
    Node* current = nullptr;
    
    void pushLeft(Node* node) {
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }
    
public:
    explicit InorderIterator(Node* root = nullptr) {
        pushLeft(root);
        if (!stk.empty()) {
            current = stk.top();
            stk.pop();
        }
    }
    
    InorderIterator& operator++() {
        pushLeft(current->right);
        if (!stk.empty()) {
            current = stk.top();
            stk.pop();
        } else {
            current = nullptr;
        }
        return *this;
    }
    
    InorderIterator operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }
    
    reference operator*() const { return current->value; }
    pointer operator->() const { return &current->value; }
    
    bool operator!=(const InorderIterator& other) const {
        return current != other.current;
    }
    bool operator==(const InorderIterator& other) const {
        return current == other.current;
    }
};

class InorderRange {
    Node* root;
public:
    explicit InorderRange(Node* r) : root(r) {}
    InorderIterator begin() const { return InorderIterator(root); }
    InorderIterator end() const { return InorderIterator(); }
};
```

---

## 三、异常安全与资源管理

### 第 13 题

```cpp
#include <string>
#include <stdexcept>
#include <utility>

extern "C" {
    int lock_file(const char* path);
    int unlock_file(const char* path);
}

class FileLock {
    std::string path;
    bool locked = false;
    
public:
    explicit FileLock(const char* p) : path(p) {
        if (lock_file(path.c_str()) != 0) {
            throw std::runtime_error("Failed to lock file: " + path);
        }
        locked = true;
    }
    
    ~FileLock() {
        if (locked) {
            unlock_file(path.c_str());
        }
    }
    
    FileLock(const FileLock&) = delete;
    FileLock& operator=(const FileLock&) = delete;
    
    FileLock(FileLock&& other) noexcept
        : path(std::move(other.path))
        , locked(std::exchange(other.locked, false)) {}
    
    FileLock& operator=(FileLock&& other) noexcept {
        if (this != &other) {
            if (locked) {
                unlock_file(path.c_str());
            }
            path = std::move(other.path);
            locked = std::exchange(other.locked, false);
        }
        return *this;
    }
    
    [[nodiscard]] bool is_locked() const noexcept { return locked; }
};
```

---

### 第 14 题

```cpp
#include <cstring>
#include <algorithm>

class String {
    char* data = nullptr;
    size_t len = 0;
    
public:
    String(const char* str = "") {
        len = std::strlen(str);
        data = new char[len + 1];
        std::memcpy(data, str, len + 1);
    }
    
    String(const String& other) : String(other.data) {}
    
    String(String&& other) noexcept 
        : data(std::exchange(other.data, nullptr))
        , len(std::exchange(other.len, 0)) {}
    
    ~String() { delete[] data; }
    
    String& operator=(const String& other) {
        String temp(other);   // 可能抛异常，但 this 不变
        swap(temp);           // noexcept
        return *this;
    }
    
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = std::exchange(other.data, nullptr);
            len = std::exchange(other.len, 0);
        }
        return *this;
    }
    
    void swap(String& other) noexcept {
        using std::swap;
        swap(data, other.data);
        swap(len, other.len);
    }
    
    friend void swap(String& a, String& b) noexcept { a.swap(b); }
};
```

**强异常安全保证的原理**：
1. `String temp(other)` 在栈上构造临时副本。如果抛出异常（例如内存分配失败），`this` 完全不受影响。
2. `swap(temp)` 只交换指针和长度值，不分配内存，标记为 `noexcept`，不会失败。
3. 因此操作要么完全成功（完成交换），要么完全失败（异常在第一步抛出，状态不变）。

---

### 第 15 题

**（a）** `noexcept` 应在以下情况使用：
- 析构函数（通常默认就是 `noexcept`）
- 移动构造函数和移动赋值运算符
- `swap` 函数
- 不会也不可能抛出异常的函数

不应标记 `noexcept` 的情况：
- 可能抛出异常的函数（如内存分配、文件 I/O、网络操作）
- 不确定未来实现是否会抛异常的函数

**（b）** 移动构造函数标记 `noexcept` 对 `std::vector` 扩容至关重要。当 `vector` 扩容需要重新分配内存时，它会尝试移动元素到新内存。如果移动构造函数是 `noexcept`，`vector` 会使用移动操作，高效且安全。如果移动构造函数未标记 `noexcept`，`vector` 为了保证强异常安全，会退化为使用拷贝操作，当元素数量很大时性能显著下降。

---

### 第 16 题

```cpp
#include <vector>
#include <list>
#include <type_traits>
#include <memory>
#include <new>
#include <shared_mutex>
#include <stdexcept>
#include <cstddef>
#include <cstdint>

template<typename T>
class ObjectPool {
    static_assert(!std::is_array_v<T>, "Array types not supported");
    static_assert(std::is_destructible_v<T>, "T must be destructible");

    struct Block {
        alignas(alignof(T)) unsigned char storage[sizeof(T)];
        bool used = false;
    };

    // 关键设计：list 节点地址稳定，扩容绝不移动已有 Block
    std::list<std::vector<Block>> chunks;
    std::vector<Block*> free_list;   // 空闲块指针栈
    size_t next_chunk_size;
    mutable std::shared_mutex mtx;

    void grow() {
        // emplace_back 到 list 不会使已有 Block* 失效
        chunks.emplace_back(next_chunk_size);
        for (auto& block : chunks.back()) {
            free_list.push_back(&block);
        }
        next_chunk_size *= 2;
    }

public:
    explicit ObjectPool(size_t initial_size = 64) : next_chunk_size(initial_size) {
        if (initial_size == 0) {
            throw std::invalid_argument("Initial pool size must be > 0");
        }
        grow();
    }

    // 资源管理复杂，禁止拷贝/移动
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator=(ObjectPool&&) = delete;

    template<typename... Args>
    T* allocate(Args&&... args) {
        std::unique_lock lock(mtx);

        if (free_list.empty()) {
            grow();
        }

        Block* block = free_list.back();
        free_list.pop_back();

        // RAII guard：若构造异常，自动将 block 归还 free_list
        struct Guard {
            Block* b;
            std::vector<Block*>* fl;
            bool committed = false;
            ~Guard() { if (!committed) fl->push_back(b); }
        } guard{block, &free_list};

        T* obj = reinterpret_cast<T*>(&block->storage);
        std::construct_at(obj, std::forward<Args>(args)...);

        block->used = true;
        guard.committed = true;
        return obj;
    }

    void deallocate(T* ptr) {
        if (!ptr) return;

        // 通过 offsetof 从 T* 反推 Block*（Block 是标准布局，安全）
        Block* block = reinterpret_cast<Block*>(
            reinterpret_cast<unsigned char*>(ptr) - offsetof(Block, storage)
        );

        std::unique_lock lock(mtx);

        if (!block->used) {
            throw std::runtime_error("Double free or invalid pointer");
        }

        std::destroy_at(ptr);
        block->used = false;
        free_list.push_back(block);
    }

    // 只读操作使用共享锁，允许多线程并发查询
    [[nodiscard]] size_t capacity() const {
        std::shared_lock lock(mtx);
        size_t total = 0;
        for (const auto& chunk : chunks) {
            total += chunk.size();
        }
        return total;
    }

    [[nodiscard]] size_t available() const {
        std::shared_lock lock(mtx);
        return free_list.size();
    }

    [[nodiscard]] size_t used() const {
        std::shared_lock lock(mtx);
        size_t total = 0;
        for (const auto& chunk : chunks) {
            total += chunk.size();
        }
        return total - free_list.size();
    }
};
```

使用了`std::list<std::vector<Block>> chunks`，list 节点地址永不移动  
RAII Guard 在构造异常时自动归还 block  
使用了`std::shared_mutex`：写操作（allocate/deallocate）独占锁，读操作（capacity/size）共享锁  
采取了`block->used` 标志 + 运行时检查   

---

## 四、架构与设计模式

### 第 17 题

```cpp
#include <vector>
#include <memory>
#include <algorithm>

struct ISorter {
    virtual ~ISorter() = default;
    virtual void sort(std::vector<int>& data) = 0;
};

struct QuickSort : ISorter {
    void sort(std::vector<int>& data) override {
        std::sort(data.begin(), data.end());
    }
};

struct BubbleSort : ISorter {
    void sort(std::vector<int>& data) override {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};

class SortEngine {
    std::unique_ptr<ISorter> strategy;
    
public:
    void setStrategy(std::unique_ptr<ISorter> s) {
        strategy = std::move(s);
    }
    
    void execute(std::vector<int>& data) {
        if (strategy) strategy->sort(data);
    }
};
```

---

### 第 18 题

```cpp
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <mutex>

class EventDispatcher {
public:
    static EventDispatcher& instance() {
        static EventDispatcher inst;
        return inst;
    }
    
    using Callback = std::function<void(const std::string&)>;
    
    void registerListener(const std::string& event, Callback cb) {
        std::lock_guard lock(mtx);
        listeners[event].push_back(std::move(cb));
    }
    
    void dispatch(const std::string& event, const std::string& data) {
        std::vector<Callback> callbacks;
        {
            std::lock_guard lock(mtx);
            auto it = listeners.find(event);
            if (it == listeners.end()) return;
            callbacks = it->second;  // 拷贝：关键点，避免迭代器失效
        }
        for (auto& cb : callbacks) {
            cb(data);
        }
    }
    
private:
    EventDispatcher() = default;
    ~EventDispatcher() = default;
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
    
    std::unordered_map<std::string, std::vector<Callback>> listeners;
    std::mutex mtx;
};
```

**为什么分发时拷贝回调列表**：如果直接在 `listeners[event]` 上遍历，回调函数中可能调用 `registerListener` 修改同一事件类型的监听器列表，导致 `vector` 重新分配内存，迭代器失效。拷贝列表后再遍历，完全避免了这个问题。

---

### 第 19 题

```cpp
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

struct IEnemy {
    virtual ~IEnemy() = default;
    virtual void attack() = 0;
};

struct Goblin : IEnemy { void attack() override { /* ... */ } };
struct Orc : IEnemy { void attack() override { /* ... */ } };
struct Dragon : IEnemy { void attack() override { /* ... */ } };

class EnemyFactory {
    std::unordered_map<std::string, std::function<std::unique_ptr<IEnemy>()>> creators;
    
public:
    EnemyFactory() {
        registerType("goblin", []{ return std::make_unique<Goblin>(); });
        registerType("orc", []{ return std::make_unique<Orc>(); });
        registerType("dragon", []{ return std::make_unique<Dragon>(); });
    }
    
    void registerType(const std::string& name, 
                      std::function<std::unique_ptr<IEnemy>()> creator) {
        creators[name] = std::move(creator);
    }
    
    std::unique_ptr<IEnemy> create(const std::string& name) const {
        auto it = creators.find(name);
        return it != creators.end() ? it->second() : nullptr;
    }
};

class Game {
    const EnemyFactory& factory;
    
public:
    explicit Game(const EnemyFactory& f) : factory(f) {}
    
    void spawnEnemy(const std::string& type) {
        auto enemy = factory.create(type);
        if (enemy) enemy->attack();
    }
};
```

**对单元测试的好处**：`Game` 不直接依赖具体敌人类，而是通过工厂接口获取敌人。测试时可以注入一个 `MockFactory`，它返回 `MockEnemy` 对象（继承自 `IEnemy`），验证 `spawnEnemy` 是否调用了 `attack()`，以及调用了多少次，而不需要创建真实的 `Goblin`/`Orc` 等复杂对象。

---

### 第 20 题

**（a）适用场景与性能差异**：

| 维度 | 编译时多态（模板/CRTP） | 运行时多态（虚函数） |
|------|------------------------|---------------------|
| 绑定时机 | 编译期 | 运行时 |
| 调用开销 | 零开销，可内联 | vtable 间接调用，难以内联 |
| 灵活性 | 策略固定，编译期确定 | 策略可动态切换 |
| 代码体积 | 可能膨胀（每个实例化一份代码） | 稳定 |
| 适用场景 | 性能敏感、策略明确不变 | 需要动态配置、插件架构 |

**（b）编译期确定策略（模板）**：
```cpp
template<typename SortPolicy>
class ContainerCompileTime {
    std::vector<int> data;
public:
    void sort() {
        SortPolicy::sort(data);  // 静态调用，零开销
    }
};

struct QuickSortPolicy {
    static void sort(std::vector<int>& data) {
        std::sort(data.begin(), data.end());
    }
};

// 使用
ContainerCompileTime<QuickSortPolicy> ct;
ct.sort();  // 编译期确定，无虚函数开销
```

**运行时确定策略（虚函数）**：
```cpp
struct ISortStrategy {
    virtual ~ISortStrategy() = default;
    virtual void sort(std::vector<int>& data) = 0;
};

class ContainerRuntime {
    std::vector<int> data;
    std::unique_ptr<ISortStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<ISortStrategy> s) {
        strategy = std::move(s);
    }
    void sort() {
        if (strategy) strategy->sort(data);
    }
};

// 使用
ContainerRuntime rt;
// 用户可以通过配置文件选择策略
rt.setStrategy(std::make_unique<QuickSortImpl>());
rt.sort();
```

**选型建议**：对性能敏感且策略在编译期确定，选模板；需要用户配置或插件动态加载，选虚函数。