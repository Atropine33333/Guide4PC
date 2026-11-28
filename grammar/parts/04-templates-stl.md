## 13. 模板与泛型

模板（template）把"类型"或"编译期常量"变成参数，让同一份逻辑适配多种类型。编译器在**实例化**（instantiation）时为每组实参生成一份真实代码，所以模板是编译期多态：没有虚表、没有运行时分发，代价是编译变慢、报错变长。这一节按"函数模板 → 类模板 → 特化 → 可变参数 → concepts → 头文件与报错"的顺序梳理。

### 13.1 函数模板与类型推导

`template <class T>` 里的 `class` 与 `typename` 完全等价。`T` 由实参**推导**得到，规则只有三条：

| 形参写法 | 实参 `int x; const int cx; int a[3];` | 推出的 T |
| --- | --- | --- |
| `T`（按值） | `x` / `cx` / `a` | `int` / `int` / `int*`（数组退化、顶层 const 丢弃） |
| `T&` | `x` / `cx` / `a` | `int` / `const int` / `int[3]` |
| `const T&` | `x` / `cx` / `a` | `int` / `int` / `int[3]`（数组不退化） |

```cpp
#include <iostream>
#include <string>

template <class T>
T mymax(T a, T b) {
    return a < b ? b : a;
}

int main() {
    std::cout << mymax(3, 7) << '\n';                     // T = int
    std::cout << mymax(2.5, 1.5) << '\n';                 // T = double
    std::cout << mymax<std::string>("ab", "cd") << '\n';  // T 显式指定
    std::cout << mymax<double>(3, 4.5) << '\n';           // 3 隐式转成 double
    std::cout << mymax('a', 'z') << '\n';                 // T = char
}
```

输出：
```text
7
2.5
cd
4.5
z
```

三件事值得记住：

- 按值传参会对大对象做一次拷贝；只读的大对象用 `const T&`，需要写回用 `T&`，要原样转发才用 `T&&`。
- 显式指定 `mymax<double>` 后，该参数不再参与推导，实参按普通函数规则做隐式转换（`3` → `3.0`）。
- 一个 `T` 必须被推成同一个类型。把 `mymax(3, 4.5)` 交给 clang++，得到的是：

```text
error: no matching function for call to 'mymax'
note: candidate template ignored: deduced conflicting types for parameter 'T' ('int' vs. 'double')
```

第一条 error 说"调用点找不到可行函数"，紧跟的 note 才是真正原因——`T` 同时被推成 `int` 和 `double`。修法：`mymax<double>(3, 4.5)`，或先把实参转成同一类型。

### 13.2 类模板与非类型模板参数

类模板的成员函数定义在类内时隐式 inline；定义在类外要重复写模板头和限定名（`template <class T, std::size_t N> T FixedArray<T, N>::sum() const`）。

非类型模板参数（NTTP）必须是编译期常量：整型、枚举、指针、引用、`nullptr_t`，C++20 起还包括浮点数与满足"结构化类型"的字面量类类型。它参与类型身份——`FixedArray<int, 3>` 和 `FixedArray<int, 4>` 是**两个毫不相干的类型**，不能互相赋值，也不能互相转换。`std::array` 正是这个设计。

```cpp
#include <cstddef>
#include <iostream>
#include <type_traits>

template <class T, std::size_t N>
class FixedArray {
public:
    T& operator[](std::size_t i) { return data_[i]; }
    const T& operator[](std::size_t i) const { return data_[i]; }
    constexpr std::size_t size() const { return N; }
    T sum() const {
        T s{};
        for (std::size_t i = 0; i < N; ++i) s += data_[i];
        return s;
    }
private:
    T data_[N]{};
};

int main() {
    FixedArray<int, 4> a;
    for (std::size_t i = 0; i < a.size(); ++i) a[i] = static_cast<int>(i * i);
    std::cout << a.size() << ' ' << a.sum() << '\n';

    FixedArray<double, 3> b;
    b[0] = 0.5;
    b[1] = 1.5;
    b[2] = 2.0;
    std::cout << b.sum() << '\n';

    std::cout << std::is_same_v<FixedArray<int, 3>, FixedArray<int, 4>> << '\n';
    std::cout << sizeof(FixedArray<int, 4>) << ' ' << sizeof(FixedArray<double, 3>) << '\n';
}
```

输出：
```text
4 14
4
0
16 24
```

`T data_[N]{}` 的 `{}` 让数组零初始化，所以 `sum()` 不用先赋值也不会读到垃圾。`size()` 标了 `constexpr`，因此 `FixedArray<int, 4> a;` 的 `a.size()` 是编译期常量，可以直接拿去做另一个模板的实参。

### 13.3 特化与偏特化（点到为止）

- **全特化**：`template <> struct Printer<bool>`，为某一组具体实参完全重写实现。
- **偏特化**：`template <class T> struct Printer<T*>`，为一族类型（这里是所有指针）重写。只有类模板和变量模板支持偏特化，**函数模板只能全特化**。
- 想让函数模板"对某类型特判"，优先用重载或 `if constexpr`，不要用全特化：特化声明必须在使用点之前可见，漏了不报错，只是静默地用主模板。

```cpp
#include <iostream>
#include <string>

template <class T>
struct Printer {
    static void print(const T& v) { std::cout << "value: " << v << '\n'; }
};

template <>
struct Printer<bool> {                       // 全特化
    static void print(bool v) { std::cout << (v ? "true" : "false") << '\n'; }
};

template <class T>
struct Printer<T*> {                         // 偏特化：匹配所有指针
    static void print(T* p) { std::cout << "pointer -> " << *p << '\n'; }
};

int main() {
    Printer<int>::print(42);
    Printer<bool>::print(true);
    int x = 7;
    Printer<int*>::print(&x);
    Printer<std::string>::print("ok");
}
```

输出：
```text
value: 42
true
pointer -> 7
value: ok
```

`std::is_integral`、`std::hash`、`std::tuple_size` 全是这个套路：主模板给通用定义，偏特化处理指针/数组/引用，全特化兜住关键类型。

### 13.4 可变参数模板

`class... Ts` 是类型包，`const Ts&... ts` 是函数参数包，`sizeof...(Ts)` 取个数。C++17 的折叠表达式把包展开成一条表达式：

| 写法 | 名称 | 展开结果 |
| --- | --- | --- |
| `(ts + ...)` | 一元右折叠 | `t1 + (t2 + t3)` |
| `(... + ts)` | 一元左折叠 | `(t1 + t2) + t3` |
| `(0 + ... + ts)` | 二元左折叠（带初值） | `((0 + t1) + t2) + t3`，空包合法 |
| `((std::cout << ts << ' '), ...)` | 逗号折叠 | 从左到右依次求值 |

```cpp
#include <cstddef>
#include <iostream>

template <class... Ts>
void printAll(const Ts&... ts) {
    ((std::cout << ts << ' '), ...);
    std::cout << "[n=" << sizeof...(Ts) << "]\n";
}

template <class... Ts>
auto sumAll(const Ts&... ts) {
    return (0 + ... + ts);
}

template <class... Ts>
constexpr std::size_t typeCount() { return sizeof...(Ts); }

int main() {
    printAll(1, 2.5, "hi");
    printAll();
    std::cout << sumAll(1, 2, 3) << '\n';
    std::cout << sumAll(1.5, 2.5) << '\n';
    std::cout << typeCount<int, double, char>() << '\n';
}
```

输出：
```text
1 2.5 hi [n=3]
[n=0]
6
4
3
```

空包也合法：逗号折叠对空包得到 `void()`，带初值的二元折叠返回初值，所以 `printAll()` 与 `sumAll()` 都不会编译失败。递归展开的写法（`head, tail...`）在 C++17 之后已经过时了。

这套机制就是 `std::make_unique<T>(args...)`、`emplace_back(args...)` 的底层：它们用 `T&&...` 接收参数，再用 `std::forward<T>(args)...` 原样转发给构造函数。

### 13.5 C++20 concepts

concepts 把"模板对类型的要求"写进签名，让不满足要求的调用在**调用点**被拒绝，而不是在实例化深处喷出几十行模板回溯。三种写法：

```cpp-fragment
template <std::integral T> T half(T x);                 // 约束模板参数
template <class T> requires std::floating_point<T>      // requires 子句
T half(T x);
void show(std::integral auto x);                        // 简写函数模板
```

```cpp
#include <concepts>
#include <iostream>
#include <string>

template <std::integral T>
T half(T x) { return x / 2; }

template <std::floating_point T>
T half(T x) { return x / 2.0; }

template <class T>
    requires requires(const T& a, const T& b) { a + b; }
T twice(const T& v) { return v + v; }

void show(std::integral auto x) { std::cout << "int-like " << x << '\n'; }
void show(std::floating_point auto x) { std::cout << "float-like " << x << '\n'; }

int main() {
    std::cout << half(7) << ' ' << half(8) << '\n';
    std::cout << half(7.5) << '\n';
    std::cout << twice(std::string("ab")) << '\n';
    show(3);
    show(2.5);
    static_assert(std::integral<char> && !std::integral<double>);
    std::cout << std::integral<char> << ' ' << std::integral<double> << '\n';
}
```

输出：
```text
3 4
3.75
abab
int-like 3
float-like 2.5
1 0
```

要点：

- 两个同名模板**只有约束不同**是合法的，约束参与重载决议，更"严"的那个优先；不满足约束的候选直接出局，所以 `half("x")` 报的是 `constraints not satisfied`，不是函数体里的类型错误。
- `requires requires(...)` 里第一个 `requires` 是子句，第二个开始才是 requires 表达式（检查这些表达式是否合法），可用来要求"支持 `a + b`"这类隐式接口。
- 概念可以当布尔常量用（`std::integral<char>` 是 `true_type`），配合 `static_assert` 做编译期自检。

### 13.6 模板与头文件

模板的定义必须对**使用点**可见：编译器要在调用处实例化，只有声明不够。所以模板通常整个放头文件——标准允许模板定义出现在多个翻译单元（隐式 inline 语义），不会重复定义错误。

❌ 头文件只留声明、定义放 `.cpp`：编译能过，链接时报 `undefined reference to 'FixedArray<int, 4>::sum() const'`。

✅ 全放头文件；想隐藏实现，用显式实例化把"哪些类型会被用到"钉死：

```cpp-fragment
// fixed_array.hpp
template <class T, std::size_t N>
class FixedArray { /* 声明和定义都在这里 */ };

extern template class FixedArray<int, 4>;   // 声明：这个实例在别处生成

// fixed_array.cpp
template class FixedArray<int, 4>;          // 显式实例化定义：只在这里生成
```

还有一条常考规则：模板里的名字查找分**两阶段**——不依赖模板参数的普通名字在定义时就查找，依赖参数的名字留到实例化时再查。所以在类模板的派生类里访问基类成员，要写 `this->x` 或 `Base<T>::x`；只写 `x` 时，编译器在定义阶段还没确定基类，会直接报"找不到成员"。

### 13.7 常见编译错误阅读法

模板报错要**从上往下读第一条 error，再看紧跟它的 note**，后面成片的 `candidate ... not viable` 通常是候选枚举，可以跳过。

| 关键词 | 含义 | 处理 |
| --- | --- | --- |
| `no matching function for call to` | 没有可行重载，或推导失败 | 看下面第一条 note |
| `deduced conflicting types for parameter 'T'` | 同一个 T 被推成两种类型 | 显式指定 `f<double>(...)`，或先转换实参 |
| `constraints not satisfied` | concept 不满足 | 继续读 `because ...`，确认缺哪个操作 |
| `required from here` | 实例化调用链：谁触发了它 | 顺着链找到自己写的那行 |
| `undefined reference to ...` | 编译通过，链接找不到定义 | 模板定义没放头文件，或漏了显式实例化 |
| `static_assert failed` | 编译期断言失败 | 断言消息一般直接写了原因 |
| `substitution failure` / `no type named ...` | SFINAE 把候选剔除了 | 说明实参不具备该模板要求的成员/类型 |

经验：真正有用的是前两三行；把第一条 error 对应的实参类型写下来，对照 13.1 的推导表，多数问题当场就能定位。

## 14. STL 容器与迭代器

STL 把"数据结构、迭代器、算法"拆成三层：容器管内存，迭代器提供统一的遍历接口，算法只认迭代器区间 `[first, last)`（左闭右开）。选容器只看三件事：**怎么访问**（随机/顺序）、**在哪插入删除**、**要不要有序**。这一节按序列容器、关联容器、适配器分开讲，再统一讲迭代器与失效规则。

### 14.1 序列容器：vector / list / deque

| 容器 | 内存布局 | 随机访问 | 头部插入 | 尾部插入 | 中间插入/删除 |
| --- | --- | --- | --- | --- | --- |
| `vector` | 一整块连续内存 | O(1) | O(n) | 摊还 O(1) | O(n) |
| `deque` | 分段连续（一块块 buffer + 中控数组） | O(1) | O(1) | O(1) | O(n) |
| `list` | 双向链表，节点分散在堆上 | 不支持 | O(1) | O(1) | O(1)（已定位） |

`vector` 的连续布局是它最大的优势：缓存友好、能传给 C 接口、能 `data()` 取首地址。代价是扩容要整体搬家。内存长这样：

```text
std::vector<int> v;
v.reserve(8);
for (int i = 1; i <= 5; ++i) v.push_back(i);   // size()==5, capacity()>=8
data_ ──► [1][2][3][4][5][ ][ ][ ]
          └── size()=5 ──┘
          └──── capacity()>=8 ────┘
```

`deque` 没有 `reserve`，也没有 `data()`；`list` 没有 `operator[]`，也不能用 `std::sort`（它要求随机访问迭代器），要用成员函数 `l.sort()`。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    v.reserve(4);
    std::cout << (v.capacity() >= 4) << ' ' << v.size() << '\n';

    v.push_back(10);
    int* p = v.data();
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    std::cout << (v.data() == p) << '\n';
    std::cout << v.size() << ' ' << v.front() << ' ' << v.back() << '\n';

    bool first = true;
    for (int x : v) {
        if (!first) std::cout << ' ';
        first = false;
        std::cout << x;
    }
    std::cout << '\n';

    v.pop_back();
    std::cout << v.size() << ' ' << (v.capacity() >= 4) << '\n';
}
```

输出：
```text
1 0
1
4 10 40
10 20 30 40
3 1
```

`reserve(4)` 只把 capacity 抬到至少 4，size 仍是 0；随后 4 次 `push_back` 都没有超过容量，所以 `data()` 不变（第二行输出 1）。注意：第 5 次 `push_back` 会触发扩容，届时所有指针、引用、迭代器**一起失效**——第一行里保存的 `p` 就悬垂了。

```cpp
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>

int main() {
    std::list<int> l{3, 1, 2};
    l.sort();
    l.push_front(0);
    l.push_back(4);
    std::cout << std::distance(l.begin(), l.end()) << ' ';
    for (int x : l) std::cout << x << ',';
    std::cout << '\n';

    std::deque<int> d{1, 2, 3};
    d.push_front(0);
    d.push_back(4);
    std::cout << d.front() << ' ' << d.back() << ' ' << d.size() << ' ' << d[2] << '\n';

    std::vector<int> v{5, 6, 7};
    auto it = v.begin();
    std::advance(it, 2);
    std::cout << *it << ' ' << *std::prev(v.end()) << ' ' << *std::next(v.begin()) << '\n';
    std::cout << std::distance(v.begin(), v.end()) << '\n';
}
```

输出：
```text
5 0,1,2,3,4,
0 4 5 2
7 7 6
3
```

`std::distance` 在 `vector` 上是 O(1)（迭代器相减），在 `list` 上是 O(n)（只能一步步走）；`std::advance` 同理。`std::next`/`std::prev` 返回新迭代器，`std::advance` 修改传入的那个——这是三者最容易混的地方。

### 14.2 容量与 reserve

- `size()`：元素个数；`capacity()`：不重新分配的前提下最多能装多少；`max_size()`：理论极限。
- `reserve(n)`：把 capacity 抬到至少 n，**不改变 size**，不构造元素。
- `resize(n)`：改变 size，新增元素值初始化（`int` 为 0，类类型调默认构造）。
- `clear()`：size 归 0，capacity 不变。
- `shrink_to_fit()`：请求归还多余内存，标准不保证一定生效。

扩容策略是**实现定义**的（libstdc++ 约 2 倍，MSVC 约 1.5 倍），所以只能依赖标准保证的两件事：`capacity() >= size()`，以及"size 不超过 capacity 时不会重新分配"。不要打印或依赖具体的 capacity 数值。

还有一条保证：`vector` 的元素在内存里物理连续，`&v[i] == v.data() + i` 恒成立（C++11 起有了 `data()`），所以 `v.data()` 可以直接传给要求数组的 C 接口，`memcpy` 也不会出事。

❌ `v.reserve(100); int* p = &v[0];` —— size 还是 0，`v[0]` 越界（UB）。reserve 分配的是"未构造的存储"，不是元素。

### 14.3 关联容器：map / unordered_map

| 项目 | `map` | `unordered_map` |
| --- | --- | --- |
| 底层结构 | 红黑树 | 哈希表（桶 + 链表） |
| 迭代顺序 | 按 key 升序 | 未指定（不可依赖） |
| 查找/插入 | O(log n) | 平均 O(1)，最坏 O(n) |
| 对 key 的要求 | 可用 `<` 比较 | 可哈希 + 可判等 |
| 迭代器失效 | 只有被删元素 | rehash 时全部失效（引用不失效） |
| 适用场景 | 要有序、要范围查询 | 纯查找、追求平均速度 |

```cpp
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

int main() {
    std::map<std::string, int> age;
    age["tom"] = 18;
    age["amy"] = 20;
    age["bob"] = 19;
    age["amy"] = 21;
    for (const auto& [k, v] : age) std::cout << k << '=' << v << ';';
    std::cout << '\n';

    std::cout << age.size() << ' ' << age.count("amy") << ' ' << age.count("zed") << '\n';

    auto it = age.find("bob");
    if (it != age.end()) std::cout << it->first << ':' << it->second << '\n';

    std::cout << age["zed"] << '\n';
    std::cout << age.size() << '\n';

    std::unordered_map<std::string, int> h;
    h["a"] = 1;
    h["b"] = 2;
    h["c"] = 3;
    std::cout << h.size() << ' ' << h.count("b") << ' ' << h.at("c") << '\n';
}
```

输出：
```text
amy=21;bob=19;tom=18;
3 1 0
bob:19
0
4
3 1 3
```

最后几行是本节的坑：`age["zed"]` 看起来像查询，实际是**插入**——key 不存在就插入一个值初始化的 `int`（0），size 从 3 变成 4。只想查就别用 `operator[]`：用 `find`、`count`，或 C++20 的 `contains`。`at()` 在 key 不存在时抛 `out_of_range`，`operator[]` 还额外要求 mapped_type 可默认构造。

`unordered_map` 的迭代顺序未指定，同一个程序在不同实现、不同插入顺序下输出可能不同，所以上面只打印 `size`、`count`、`at` 这类与顺序无关的量。

插入接口的选择也有讲究：`m.insert_or_assign(k, v)` 是"有则覆盖"，`m.try_emplace(k, args...)` 是"没有才构造"（C++17），两者都比 `operator[]` 少一次默认构造，性能敏感时优先用它们。

### 14.4 set / unordered_set

`set` 可以看成"只有 key 的 map"：自动去重 + 有序遍历。`insert` 返回 `pair<iterator, bool>`，`bool` 表示是否真的插入了。元素是 **const** 的，不能 `*it = 5`，要改只能 erase 再 insert。

```cpp
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

int main() {
    std::vector<int> raw{5, 3, 5, 1, 3, 9};
    std::set<int> s(raw.begin(), raw.end());
    std::cout << s.size() << ' ';
    for (int x : s) std::cout << x << ',';
    std::cout << '\n';

    std::unordered_set<int> us(raw.begin(), raw.end());
    std::cout << us.size() << ' ' << us.count(3) << ' ' << us.count(4) << '\n';

    auto [it, ok] = s.insert(7);
    std::cout << *it << ' ' << ok << '\n';
    auto [it2, ok2] = s.insert(9);
    std::cout << *it2 << ' ' << ok2 << '\n';

    std::cout << *s.lower_bound(4) << ' ' << *s.upper_bound(5) << '\n';
}
```

输出：
```text
4 1,3,5,9,
4 1 0
7 1
9 0
5 7
```

`set` 的 `lower_bound` / `upper_bound` 是成员函数，O(log n)；对 `set` 用 `std::lower_bound` 会退化成 O(n)（`std::lower_bound` 只会用 `std::advance` 走迭代器），这是一个很隐蔽的性能坑。

### 14.5 容器适配器：stack / queue / priority_queue

| 适配器 | 默认底层容器 | 主要操作 | 语义 |
| --- | --- | --- | --- |
| `stack` | `deque` | `push` / `pop` / `top` | 后进先出 |
| `queue` | `deque` | `push` / `pop` / `front` / `back` | 先进先出 |
| `priority_queue` | `vector` | `push` / `pop` / `top` | 堆，默认大根堆 |

适配器**不提供迭代器**，不能遍历、不能套 STL 算法；要看全部元素只能逐个 `pop`（会清空容器）。`priority_queue` 默认用 `std::less` 比较，堆顶是最大值；要小根堆就换 `std::greater<int>`。

```cpp
#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

bool balanced(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;
            char t = st.top();
            st.pop();
            if ((c == ')' && t != '(') || (c == ']' && t != '[') || (c == '}' && t != '{'))
                return false;
        }
    }
    return st.empty();
}

int main() {
    std::cout << balanced("([]{})") << ' ' << balanced("(]") << ' ' << balanced("((") << '\n';

    std::queue<int> q;
    for (int i = 1; i <= 3; ++i) q.push(i * 10);
    while (!q.empty()) {
        std::cout << q.front() << ',';
        q.pop();
    }
    std::cout << '\n';

    std::priority_queue<int> pq;
    for (int x : {5, 1, 9, 3}) pq.push(x);
    while (!pq.empty()) {
        std::cout << pq.top() << ',';
        pq.pop();
    }
    std::cout << '\n';

    std::priority_queue<int, std::vector<int>, std::greater<int>> minq;
    for (int x : {5, 1, 9, 3}) minq.push(x);
    std::cout << minq.top() << ' ' << minq.size() << '\n';
}
```

输出：
```text
1 0 0
10,20,30,
9,5,3,1,
1 4
```

括号匹配是栈的原型题：遇到左括号入栈，遇到右括号就和栈顶比对，最后栈必须为空。`priority_queue` 的 `top()` 是 O(1)，`push`/`pop` 是 O(log n)，底层就是 `std::push_heap` / `std::pop_heap`。它没有 `clear()`：清空只能循环 `pop()`，或直接 `pq = {};` 赋一个空堆。

### 14.6 迭代器类别与遍历

| 类别 | 能力 | 典型容器/来源 | 可用的算法举例 |
| --- | --- | --- | --- |
| 输入 / 输出 | 单向、单遍 | `istream_iterator` / `ostream_iterator` | `copy` |
| 前向 forward | 可多遍、只能 `++` | `forward_list`、`unordered_*` | `find`、`for_each` |
| 双向 bidirectional | 还能 `--` | `list`、`map`、`set` | `reverse`、`prev` |
| 随机访问 random access | `+n`、`-`、下标、比大小 | `vector`、`deque`、`array`、`string` | `sort`、`binary_search` |
| 连续 contiguous（C++20） | 内存物理连续，可 `data()` | `vector`、`array`、`string` | 传给 C 接口 |

遍历有三种写法：下标（只有随机访问容器能用）、迭代器、range-for。range-for 展开后就是 `begin()` / `end()` 那一套，只是不用手写类型。只读遍历用 `cbegin()` / `cend()`，拿到的是 `const_iterator`，能防止误改。`std::distance` 对随机访问迭代器是 O(1)，对链表是 O(n)——在 list 上循环调用 `std::distance` 会悄悄变成 O(n²)。

### 14.7 迭代器失效

| 容器 | 什么操作让迭代器失效 |
| --- | --- |
| `vector` | 扩容（`push_back`/`insert`/`reserve` 不够）→ 全部失效；`erase` → 被删位置及之后全部失效 |
| `deque` | 头/尾插入 → 迭代器全部失效（**引用仍有效**）；中间插入或删除 → 全部失效 |
| `list` | 只有指向被删元素的那个失效 |
| `map` / `set` | 只有指向被删元素的那个失效 |
| `unordered_*` | rehash（插入导致扩容）→ 全部失效（引用不失效）；否则只有被删的失效 |

```cpp
#include <iostream>
#include <map>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 == 0) it = v.erase(it);
        else ++it;
    }
    for (int x : v) std::cout << x << ',';
    std::cout << '\n';

    std::map<int, int> m{{1, 10}, {2, 20}, {3, 30}};
    for (auto it = m.begin(); it != m.end();) {
        if (it->first == 2) it = m.erase(it);
        else ++it;
    }
    std::cout << m.size() << ' ' << m.begin()->first << ' ' << m.rbegin()->first << '\n';
}
```

输出：
```text
1,3,5,
2 1 3
```

❌ 常见错误写法：

```cpp-fragment
for (auto it = v.begin(); it != v.end(); ++it)
    if (*it % 2 == 0) v.erase(it);   // erase 后 it 已失效，++it 是 UB
```

✅ 两种正确写法：`it = v.erase(it)`（erase 返回下一个有效位置），或 erase-remove 惯用法（见 15.4）。另外，循环里缓存的 `v.end()` 在 `push_back` 之后也可能失效，别把它存到循环外。

### 14.8 选型建议

| 需求 | 选谁 | 理由 |
| --- | --- | --- |
| 默认的"一组元素" | `vector` | 连续内存、随机访问、摊还 O(1) 尾插 |
| 频繁头尾插入 | `deque` | 两端 O(1)，仍支持随机访问 |
| 频繁在中间插入删除，且已持有位置 | `list` | O(1) 接链，代价是缓存不友好 |
| 按 key 有序、要范围查询 | `map` / `set` | 红黑树，O(log n)，天然有序 |
| 只要平均 O(1) 查找，不关心顺序 | `unordered_map` / `unordered_set` | 哈希表 |
| 后进先出 / 先进先出 / 取最值 | `stack` / `queue` / `priority_queue` | 语义清晰，封装好 |
| 定长、要栈上分配 | `std::array` | 无堆分配，size 是编译期常量 |

### 14.9 常见坑

- `vector<bool>` 是特化：`v[0]` 返回代理对象，`&v[0]` 不合法，`auto x = v[0]` 拿到的是代理而不是 `bool&`。需要真 bool 数组就用 `vector<char>` 或 `array<bool, N>`。
- `size()` 返回**无符号**：`for (int i = 0; i < v.size() - 1; ++i)` 在空容器上 `v.size() - 1` 会回绕成 `SIZE_MAX`，循环直接爆掉。用 `std::size_t`，或先判空。
- `v[i]` 不检查越界；要检查用 `v.at(i)`（越界抛 `out_of_range`）。
- `map[key]` 会插入（14.3）；查询请用 `find` / `contains`。
- `reserve` 之后保存的指针/引用，在扩容后悬垂（14.2）。
- `list` 没有 `operator[]`，`std::sort(l.begin(), l.end())` 编译错误，用 `l.sort()`。
- `priority_queue` 的 `pop()` 返回 `void`，取值要先 `top()`。

## 15. 算法与常用技巧

`<algorithm>` 与 `<numeric>` 用"迭代器区间 + 谓词"描述操作，一切区间都是左闭右开 `[first, last)`。这一节按"排序 → 查找 → 统计 → 去重 → 二分答案 → 双指针 → 位运算 → 字符串"排列，最后补输入输出加速。这些都是作业和比赛里反复出现的套路。

### 15.1 排序：sort 与 stable_sort

- `std::sort`：平均 O(n log n)，**不稳定**，需要随机访问迭代器。
- `std::stable_sort`：稳定；内存足够时 O(n log n)，申请不到额外内存时退化为 O(n log² n)。
- 比较器必须满足**严格弱序**：`comp(a, a)` 必须为 `false`，且传递。写成 `<=` / `>=` 会破坏它，后果是 UB——常见表现是越界崩溃或死循环，而且换个编译器/换个数据就可能"看起来正常"。

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

int main() {
    std::vector<int> v{5, 1, 4, 1, 9, 2};
    std::sort(v.begin(), v.end());
    for (int x : v) std::cout << x << ',';
    std::cout << '\n';

    std::sort(v.begin(), v.end(), std::greater<int>());
    for (int x : v) std::cout << x << ',';
    std::cout << '\n';

    std::vector<Student> st{{"amy", 90}, {"bob", 90}, {"cid", 80}};
    std::stable_sort(st.begin(), st.end(),
                     [](const Student& a, const Student& b) { return a.score > b.score; });
    for (const auto& s : st) std::cout << s.name << ':' << s.score << ',';
    std::cout << '\n';

    std::sort(st.begin(), st.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.name < b.name;          // 第二关键字，顺序才确定
    });
    std::cout << st.front().name << ' ' << st.back().name << '\n';
}
```

输出：
```text
1,1,2,4,5,9,
9,5,4,2,1,1,
amy:90,bob:90,cid:80,
amy cid
```

第三行体现 `stable_sort` 的价值：amy 和 bob 同分，稳定排序保留输入顺序。`sort` 不保证这一点，所以最后一段显式加了第二关键字——**要确定的顺序，就把比较规则写全**，不要指望算法的实现细节。

如果只要前 k 小（或第 k 大）而不需要整体有序，用 `std::nth_element`（平均 O(n)）或 `std::partial_sort`（O(n log k)），比完整 `sort` 快一个量级；这两个函数同样需要随机访问迭代器。

### 15.2 查找：find / binary_search / lower_bound

| 函数 | 返回值 | 前提 | 复杂度 |
| --- | --- | --- | --- |
| `find` | 迭代器（找不到返回 `end()`） | 无 | O(n) |
| `binary_search` | `bool` | **已排序** | O(log n) |
| `lower_bound` | 第一个 `>= x` 的位置 | 已排序 | O(log n) |
| `upper_bound` | 第一个 `> x` 的位置 | 已排序 | O(log n) |
| `equal_range` | `pair(lower, upper)` | 已排序 | O(log n) |

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 3, 3, 5, 7, 9};

    std::cout << std::binary_search(v.begin(), v.end(), 5) << ' '
              << std::binary_search(v.begin(), v.end(), 4) << '\n';

    auto lo = std::lower_bound(v.begin(), v.end(), 3);
    auto hi = std::upper_bound(v.begin(), v.end(), 3);
    std::cout << (lo - v.begin()) << ' ' << (hi - v.begin()) << ' ' << (hi - lo) << '\n';

    auto r = std::equal_range(v.begin(), v.end(), 3);
    std::cout << (r.second - r.first) << '\n';

    std::cout << *std::lower_bound(v.begin(), v.end(), 4) << '\n';
    std::cout << (std::find(v.begin(), v.end(), 4) == v.end()) << '\n';
    std::cout << *std::max_element(v.begin(), v.end()) << ' '
              << *std::min_element(v.begin(), v.end()) << '\n';
}
```

输出：
```text
1 0
1 3 2
2
5
1
9 1
```

`it - v.begin()` 是下标（随机访问容器）；`upper_bound - lower_bound` 就是 x 出现的次数，`equal_range` 只是把两者一次返回。三个坑：

- ❌ 对未排序区间调用 `binary_search` 不报错，只是答案随机错误。
- ❌ 拿 `binary_search` 当下标用——它只返回 `bool`，要位置就用 `lower_bound`。
- 降序数组要传比较器：`std::lower_bound(v.begin(), v.end(), x, std::greater<int>())`，语义变成"第一个不满足 `greater` 的位置"。

### 15.3 计数、求和与前缀和

- `count` / `count_if`：O(n)，返回个数。
- `accumulate(first, last, init)`：**返回类型由 init 决定**。初值写 `0` 是 `int`，写 `0LL` 才是 `long long`。
- `partial_sum`：直接生成前缀和数组。
- 手写前缀和：`pref[0] = 0; pref[i+1] = pref[i] + a[i];`，区间 `[l, r)` 的和 = `pref[r] - pref[l]`，预处理 O(n)、查询 O(1)。

```cpp
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> v{4, 7, 2, 9, 5};
    std::cout << std::count(v.begin(), v.end(), 9) << ' '
              << std::count_if(v.begin(), v.end(), [](int x) { return x % 2 != 0; }) << '\n';
    std::cout << std::accumulate(v.begin(), v.end(), 0) << ' '
              << std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>()) << '\n';

    std::vector<long long> pref(v.size() + 1, 0);
    for (std::size_t i = 0; i < v.size(); ++i) pref[i + 1] = pref[i] + v[i];
    auto sum = [&pref](std::size_t l, std::size_t r) { return pref[r] - pref[l]; };
    std::cout << sum(1, 3) << ' ' << sum(0, 5) << ' ' << sum(2, 2) << '\n';

    std::vector<unsigned> big{3000000000u, 3000000000u};
    std::cout << std::accumulate(big.begin(), big.end(), 0u) << '\n';
    std::cout << std::accumulate(big.begin(), big.end(), 0ULL) << '\n';
}
```

输出：
```text
1 3
27 2520
9 27 0
1705032704
6000000000
```

最后两行是"初值类型决定一切"的实证：两个 30 亿相加，`0u` 的初值让结果在 `unsigned` 里回绕（定义行为，1705032704），`0ULL` 才得到正确的 60 亿。把 `unsigned` 换成 `int`，同样的写法就是**有符号溢出（UB）**，所以累加和一律用 `long long` 初值。

### 15.4 去重与删除：unique / erase-remove

- `unique` 只压紧**相邻**的重复元素，返回新的逻辑结尾，容器 size 不变；末尾剩下的元素"有效但值未指定"，不能读。
- 所以标准写法是两步：`v.erase(std::unique(v.begin(), v.end()), v.end())`；要去重前先 `sort`。
- 删除满足条件的元素：`v.erase(std::remove_if(v.begin(), v.end(), pred), v.end())`。
- C++20 起有一行版本：`std::erase(v, x)` / `std::erase_if(v, pred)`。

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 1, 2, 2, 2, 3, 4, 4};
    auto last = std::unique(v.begin(), v.end());
    std::cout << v.size() << ' ' << (last - v.begin()) << '\n';

    v.erase(last, v.end());
    for (int x : v) std::cout << x << ',';
    std::cout << '\n';

    std::vector<int> w{1, 2, 3, 4, 5, 6};
    w.erase(std::remove_if(w.begin(), w.end(), [](int x) { return x % 2 == 0; }), w.end());
    for (int x : w) std::cout << x << ',';
    std::cout << '\n';

    std::vector<int> u{5, 3, 5, 1, 3};
    std::sort(u.begin(), u.end());
    u.erase(std::unique(u.begin(), u.end()), u.end());
    std::cout << u.size() << ' ' << u.front() << ' ' << u.back() << '\n';

    std::reverse(u.begin(), u.end());
    std::cout << u.front() << ' ' << u.back() << '\n';
}
```

输出：
```text
8 4
1,2,3,4,
1,3,5,
3 1 5
5 1
```

第一行 `8 4`：`unique` 没有删元素，size 仍是 8，只是返回了"新结尾"的位置 4。`remove_if` 同理——它只是把要保留的元素往前挪，所以必须再 `erase` 一次，这就是 erase-remove 惯用法的由来。

### 15.5 二分答案

适用条件：答案是单调的——`check(mid)` 为真时，所有更大的 mid 也为真（或反过来）。框架固定：

```cpp-fragment
long long lo = 下界, hi = 上界;          // 答案落在 [lo, hi]
while (lo < hi) {
    long long mid = lo + (hi - lo) / 2;  // 防溢出
    if (check(mid)) hi = mid;            // mid 可行，答案 <= mid
    else lo = mid + 1;
}
return lo;
```

`mid = (lo + hi) / 2` 在 lo、hi 都接近 `LLONG_MAX` 时会溢出，所以写成 `lo + (hi - lo) / 2`。

```cpp
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

bool canSplit(const std::vector<int>& a, int k, long long limit) {
    int parts = 1;
    long long cur = 0;
    for (int x : a) {
        if (x > limit) return false;
        if (cur + x > limit) {
            ++parts;
            cur = x;
        } else {
            cur += x;
        }
    }
    return parts <= k;
}

long long minMaxSum(const std::vector<int>& a, int k) {
    long long lo = *std::max_element(a.begin(), a.end());
    long long hi = std::accumulate(a.begin(), a.end(), 0LL);
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (canSplit(a, k, mid)) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

int main() {
    std::vector<int> a{7, 2, 5, 10, 8};
    std::cout << minMaxSum(a, 2) << '\n';
    std::cout << minMaxSum(a, 1) << ' ' << minMaxSum(a, 5) << '\n';
    std::cout << minMaxSum(a, 3) << '\n';
}
```

输出：
```text
18
32 10
14
```

`check` 用贪心：能塞进当前段就塞，塞不下就新开一段，段数超过 k 即不可行。`k=2` 的最优切法是 `[7,2,5]` 与 `[10,8]`，最大段和 18；`k=1` 只能整段（32）；`k=5` 每段一个元素，最大 10。答案对 `limit` 单调，二分才成立。

### 15.6 双指针

三类套路：

- **对撞**：有序数组两数之和，`i` 从左、`j` 从右，和大了 `--j`，小了 `++i`，O(n) 代替 O(n²)。
- **快慢**：原地去重、判环、找中点，慢指针写、快指针读。
- **滑动窗口**：窗口内维护和/计数，右端扩张、左端收缩，求最短/最长满足条件的区间。

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int twoSumPairs(const std::vector<int>& a, int target) {
    int i = 0;
    int j = static_cast<int>(a.size()) - 1;
    int cnt = 0;
    while (i < j) {
        int s = a[i] + a[j];
        if (s == target) {
            ++cnt;
            ++i;
            --j;
        } else if (s < target) {
            ++i;
        } else {
            --j;
        }
    }
    return cnt;
}

int shortestWindow(const std::vector<int>& a, int s) {
    int n = static_cast<int>(a.size());
    int best = n + 1;
    int sum = 0;
    int l = 0;
    for (int r = 0; r < n; ++r) {
        sum += a[r];
        while (sum >= s) {
            best = std::min(best, r - l + 1);
            sum -= a[l++];
        }
    }
    return best == n + 1 ? 0 : best;
}

int main() {
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    std::cout << twoSumPairs(a, 7) << '\n';

    std::vector<int> b{2, 3, 1, 2, 4, 3};
    std::cout << shortestWindow(b, 7) << '\n';
    std::cout << shortestWindow(b, 100) << '\n';
    std::cout << shortestWindow(b, 1) << '\n';
}
```

输出：
```text
3
2
0
1
```

`twoSumPairs` 的正确性靠有序性：`a[i] + a[j]` 太大时，`j` 左边的任何数与 `a[i]` 配对只会更小，所以 `--j` 不会漏解。滑动窗口里每个元素最多进出窗口各一次，总 O(n)。

### 15.7 位运算技巧

| 技巧 | 写法 | 用途 |
| --- | --- | --- |
| lowbit | `x & -x` | 取最低位的 1（树状数组） |
| 去掉最低位的 1 | `x & (x - 1)` | 数 1 的个数、判断 2 的幂 |
| 取第 k 位 | `(x >> k) & 1` | 状态压缩 |
| 置第 k 位 | `x \| (1u << k)` | 状态压缩 |
| 取反某位 | `x ^ (1u << k)` | 翻转 |

异或的三条性质：`a ^ a = 0`、`a ^ 0 = a`、满足交换律与结合律。所以把一堆数全部异或起来，成对出现的会互相抵消，剩下的就是只出现一次的那个。

```cpp
#include <bit>
#include <cstdint>
#include <iostream>

int main() {
    unsigned x = 0b10110100u;
    std::cout << (x & -x) << '\n';
    std::cout << (x & (x - 1)) << '\n';
    std::cout << std::popcount(x) << '\n';
    std::cout << std::countr_zero(x) << '\n';
    std::cout << (1u << 5) << '\n';
    std::cout << (x >> 2) << '\n';

    int a[] = {4, 1, 2, 1, 2};
    int r = 0;
    for (int v : a) r ^= v;
    std::cout << r << '\n';
    std::cout << (5 ^ 5) << ' ' << (7 ^ 0) << '\n';
    std::cout << (3 ^ 5 ^ 3) << '\n';

    int p = 3, q = 8;
    p ^= q;
    q ^= p;
    p ^= q;
    std::cout << p << ' ' << q << '\n';
}
```

输出：
```text
4
176
4
2
32
45
4
0 7
5
8 3
```

注意事项：

- 左移位数必须小于类型宽度，`1 << 32` 是 UB；左移负数是 UB。C++20 起 `1 << 31` 本身已经合法（得到 `INT_MIN`），但把最高位当位掩码用时仍建议写 `1u << 31`，免得掉进有符号解释。
- 有符号右移是实现定义的（通常是算术右移，补符号位），无符号才是逻辑右移。
- 异或交换 `a ^= b; b ^= a; a ^= b;` 在 `a`、`b` 是同一个变量时会把它清零，别用；可读性也不如 `std::swap`。
- `std::popcount` / `std::countr_zero` 在 C++20 `<bit>` 里；老环境用 `__builtin_popcount` / `__builtin_ctz`。

### 15.8 字符串处理技巧

- 拼接用 `s += t`（摊还 O(1)）；`s = s + t` 每次都会整串拷贝。
- 子串 `s.substr(pos, len)`；`pos > size()` 会抛 `out_of_range`。
- 查找 `s.find(t)` 找不到返回 `std::string::npos`（`size_t` 的最大值），必须和 `npos` 比，不要和 `-1` 比。
- 数值互转：`std::stoi` / `std::stoll` / `std::stod`（输入非法会抛异常），`std::to_string`（浮点固定 6 位小数）。它们还能带两个参数，`std::stoi(s, &pos, 16)` 表示按 16 进制解析，并把第一个未解析字符的下标写进 `pos`。
- 标准库没有 `split`，按分隔符切要自己写循环。
- 字符分类函数 `std::isdigit` / `std::isupper` 接收 `int`，**直接传负的 `char` 是 UB**，先转 `unsigned char`。
- `cin >> s` 遇到空白就停；读整行用 `getline(cin, line)`；两者混用时先 `cin.ignore()` 吃掉残留换行。

```cpp
#include <cctype>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> out;
    std::string cur;
    for (char c : s) {
        if (c == sep) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    out.push_back(cur);
    return out;
}

int main() {
    std::string line = "age=21;name=tom;city=hz";
    for (const auto& kv : split(line, ';')) {
        auto pos = kv.find('=');
        std::cout << kv.substr(0, pos) << "->" << kv.substr(pos + 1) << ',';
    }
    std::cout << '\n';

    std::string s = "  Hello C++  ";
    std::size_t b = s.find_first_not_of(' ');
    std::size_t e = s.find_last_not_of(' ');
    std::cout << '[' << s.substr(b, e - b + 1) << "]\n";

    std::cout << std::stoi("123") + 1 << ' ' << std::to_string(3.14) << '\n';

    int up = 0, low = 0, dig = 0;
    for (unsigned char c : std::string("Ab3 xY9")) {
        if (std::isupper(c)) ++up;
        else if (std::islower(c)) ++low;
        else if (std::isdigit(c)) ++dig;
    }
    std::cout << up << ' ' << low << ' ' << dig << '\n';

    std::vector<int> nums;
    std::istringstream in("3 14 15");
    int t;
    while (in >> t) nums.push_back(t);
    std::cout << nums.size() << ' ' << nums[1] << '\n';
}
```

输出：
```text
age->21,name->tom,city->hz,
[Hello C++]
124 3.140000
2 2 2
3 14
```

`find_first_not_of` / `find_last_not_of` 是"去首尾空白"的标准做法；`substr` 的第二个参数是长度，不是结束下标。`istringstream` 把字符串当输入流，适合解析一行用空格分隔的数字——它也是本讲义里演示 `cin` 行为的常用替身（不依赖真实 stdin）。

### 15.9 输入输出加速

`cin`/`cout` 默认与 C 的 stdio 保持同步，每次 IO 都有额外开销。数据量大（十万级以上）时，在 `main` 开头加两行：

```cpp-fragment
std::ios::sync_with_stdio(false);   // 解除与 printf/scanf 的同步
std::cin.tie(nullptr);              // 解除 cin 与 cout 的绑定
```

- 第一行之后**不要**再混用 `printf` / `scanf`，否则输出顺序可能错乱。
- 第二行让 `cin` 不再自动 flush `cout`；交互题需要手动 `std::cout.flush()`。
- 两行必须写在任何 IO 之前。
- 换行用 `'\n'`，不要用 `std::endl`：`endl` 每次都 flush，是大数据 TLE 的常见原因。
- 还不够快就用 `scanf` / `printf`，或手写 `getchar` 快读（比赛模板，这里不展开）。

## 16. 常见陷阱清单与速查表

前面各节把坑分散讲了，这一节集中成表：先给"现象 / 原因 / 正确做法"三列清单，再给类型大小、运算符优先级、容器复杂度、常用头文件四张速查表，最后配三个小例子。

### 16.1 陷阱清单

| 现象 | 原因 | 正确做法 |
| --- | --- | --- |
| 局部变量、数组元素是随机值 | `int x;`、`int a[10];` 不初始化，栈上内容是上次留下的 | 定义时就初始化：`int x = 0;`、`int a[10]{};` |
| 两个正数相加得到负数 | 有符号溢出是 UB，无符号才回绕 | 用 `long long`；加之前判断 `a > INT_MAX - b` |
| `7 / 2` 得 3、`-7 / 2` 得 -3 | 整数除法向零取整，不是向下取整 | 要精确值用 `double`；要向下取整用 `std::floor` |
| `0.1 + 0.2 == 0.3` 为 `false` | 二进制浮点无法精确表示 0.1 | 用 `fabs(a - b) < eps` 比较，或改用整数运算 |
| 程序偶发崩溃、结果自己会变 | 数组越界写坏了别的变量（UB） | 用 `at()` 或 `-fsanitize=address` 定位；下标用 `size_t` |
| 指针/引用指向已销毁的对象 | 返回局部变量的引用、`delete` 后继续用、vector 扩容后旧指针 | 返回值类型；用智能指针；扩容后重新取地址 |
| 两个对象改一个崩、改一个动俩 | 默认拷贝是逐成员浅拷贝，两个指针指向同一块内存 | 用 `string`/`vector`/智能指针；自己管资源就遵守三/五法则 |
| 多态调用总是调基类版本 | 对象切片：按值传参、存进 `vector<Base>` 只复制基类部分 | 传 `Base&` / `Base*`，容器存 `unique_ptr<Base>` |
| `erase` 之后崩溃或死循环 | 迭代器失效 | `it = v.erase(it)`，或 erase-remove |
| `if (x = 1)` 永远为真 | 赋值表达式的结果就是赋进去的值 | 常量放左边 `1 == x`；开 `-Wall`（`-Wparentheses` 会警告） |
| 少写一个分号，错误却指向下一行 | 编译器在下一行才发现上一条语句没结束 | 看报错行的**上一行**；`if (...)` 后多写 `;` 会让分支体变成空语句 |
| 该改的没改、不该改的改了 | const 正确性缺失：该 const 的没 const，或 const 成员函数里改成员 | 只读参数用 `const T&`；不修改状态的成员函数加 `const` |
| `#define SQUARE(x) x*x` 展开出错 | 宏是文本替换，没有类型、没有求值顺序、没有作用域 | 用 `inline` 函数或 `constexpr` 函数；宏参数加括号只是缓解 |
| 函数里 `sizeof` 数组变成指针大小 | 数组形参退化为指针 | 传 `std::array` / `std::vector`，或额外传长度 |
| `char c = 200;` 之后 `c < 0` 成立 | `char` 是否有符号是实现定义的 | 处理字节用 `unsigned char` / `std::uint8_t` |

### 16.2 类型大小与取值范围

标准只保证"至少多少位"，具体宽度由实现决定。下表"本机"一列是在 **x86-64 Windows + MinGW clang 22** 上实测的结果：

| 类型 | 标准最小 | 本机 sizeof | 本机取值范围 |
| --- | --- | --- | --- |
| `bool` | — | 1 | `true` / `false` |
| `char` | 8 位 | 1 | -128 ~ 127（本机有符号） |
| `unsigned char` | 8 位 | 1 | 0 ~ 255 |
| `short` | 16 位 | 2 | -32768 ~ 32767 |
| `int` | 16 位 | 4 | -2147483648 ~ 2147483647 |
| `unsigned` | 16 位 | 4 | 0 ~ 4294967295 |
| `long` | 32 位 | **4** | 同 `int`（Linux x86-64 上是 8 字节！） |
| `long long` | 64 位 | 8 | 约 ±9.2×10^18 |
| `float` | 6 位有效数字 | 4 | 约 1.2×10^-38 ~ 3.4×10^38 |
| `double` | 10 位有效数字 | 8 | 约 2.2×10^-308 ~ 1.8×10^308 |
| 指针 `T*` | — | 8 | 64 位地址 |
| `std::size_t` | — | 8 | 无符号，`sizeof` 的返回类型 |

`long` 那一行是最容易踩的跨平台坑：Windows 是 LLP64（`long` 4 字节），Linux/macOS 是 LP64（`long` 8 字节）。需要 64 位整数就写 `long long` 或 `std::int64_t`，不要写 `long`。

```cpp
#include <climits>
#include <cstddef>
#include <iostream>

int main() {
    unsigned u = 0;
    u -= 1;
    std::cout << u << ' ' << UINT_MAX << '\n';

    int a = -7, b = 2;
    std::cout << a / b << ' ' << a % b << '\n';
    std::cout << 1 / 2 << ' ' << 1.0 / 2 << '\n';

    int x = 100000, y = 100000;
    std::cout << 1LL * x * y << '\n';

    int big = INT_MAX;
    std::cout << (big > INT_MAX - 1) << '\n';

    std::cout << sizeof(char) << ' ' << sizeof(short) << ' ' << sizeof(int) << ' '
              << sizeof(long) << ' ' << sizeof(long long) << '\n';
    std::cout << sizeof(float) << ' ' << sizeof(double) << ' ' << sizeof(void*) << ' '
              << sizeof(std::size_t) << '\n';
}
```

输出：
```text
4294967295 4294967295
-3 -1
0 0.5
10000000000
1
1 2 4 4 8
4 8 8 8
```

`1LL * x * y` 里的 `1LL` 把整个乘法提升到 64 位；如果写 `x * y`，两个 `int` 先乘、溢出（UB），再赋值给 `long long` 已经晚了——**提升要发生在乘法之前**。

### 16.3 运算符优先级（常用部分）

| 优先级 | 运算符 | 结合性 |
| --- | --- | --- |
| 1（最高） | `::` | 左 |
| 2 | `a++` `a--` `f()` `a[]` `.` `->`（后缀） | 左 |
| 3 | `!` `~` `++a` `--a` `+a` `-a` `(T)` `*p` `&x` `sizeof`（一元） | 右 |
| 5 | `*` `/` `%` | 左 |
| 6 | `+` `-` | 左 |
| 7 | `<<` `>>` | 左 |
| 8 | `<=>` | 左 |
| 9 | `<` `<=` `>` `>=` | 左 |
| 10 | `==` `!=` | 左 |
| 11 | `&`（按位与） | 左 |
| 12 | `^` | 左 |
| 13 | `\|` | 左 |
| 14 | `&&` | 左 |
| 15 | `\|\|` | 左 |
| 16 | `?:` `=` `+=` `-=` 等 | 右 |
| 17（最低） | `,` | 左 |

最容易错的两条：

- **按位运算低于比较**：`x & 1 == 0` 实际是 `x & (1 == 0)`，永远得 0；要写 `(x & 1) == 0`。
- **赋值右结合**：`a = b = c` 是 `a = (b = c)`；`*p++` 是 `*(p++)` 而不是 `(*p)++`。

### 16.4 容器与算法复杂度

| 操作 | `vector` | `deque` | `list` | `map`/`set` | `unordered_*` |
| --- | --- | --- | --- | --- | --- |
| 随机访问 `v[i]` | O(1) | O(1) | 不支持 | 不支持 | 不支持 |
| 查找某值 | O(n) | O(n) | O(n) | O(log n) | 平均 O(1) |
| 头部插入 | O(n) | O(1) | O(1) | — | — |
| 尾部插入 | 摊还 O(1) | O(1) | O(1) | — | — |
| 中间插入 | O(n) | O(n) | O(1)※ | O(log n) | 平均 O(1) |
| 按 key 有序遍历 | 需先排序 | 需先排序 | 需先排序 | 天然有序 | 无序 |

※ 前提是已经拿到位置的迭代器；找位置本身仍是 O(n)。

常用算法：`sort` O(n log n)；`stable_sort` O(n log n)（内存不足时 O(n log² n)）；`lower_bound` / `upper_bound` O(log n)；`find` / `count` / `accumulate` O(n)；`unique` O(n)（要求相邻重复已排好）。

### 16.5 常用头文件

| 头文件 | 内容 |
| --- | --- |
| `<iostream>` | `cin` / `cout` / `cerr` |
| `<iomanip>` | `setprecision` / `setw` / `fixed` |
| `<string>` | `std::string`、`stoi`、`to_string` |
| `<vector>` `<deque>` `<list>` `<array>` | 序列容器 |
| `<map>` `<set>` `<unordered_map>` `<unordered_set>` | 关联容器 |
| `<stack>` `<queue>` | 适配器（`priority_queue` 在 `<queue>` 里） |
| `<algorithm>` | `sort` / `find` / `lower_bound` / `unique` / `remove_if` / `reverse` / `min` / `max` / `clamp` |
| `<numeric>` | `accumulate` / `partial_sum` / `iota` / `gcd` / `lcm` |
| `<functional>` | `greater` / `less` / `hash` / `function` |
| `<utility>` | `pair` / `swap` / `move` / `forward` |
| `<tuple>` | `tuple` / `tie` / `get` |
| `<cmath>` | `sqrt` / `fabs` / `pow` / `floor` / `ceil` |
| `<climits>` `<cstdint>` | `INT_MAX` / `int64_t` / `uint32_t` |
| `<cstddef>` | `size_t` / `ptrdiff_t` |
| `<sstream>` | `istringstream` / `ostringstream`（字符串当流） |
| `<bit>` | `popcount` / `countr_zero`（C++20） |
| `<concepts>` | `integral` / `floating_point` 等标准概念 |

### 16.6 浮点比较与对象切片

```cpp
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

bool nearlyEqual(double a, double b) {
    double scale = std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
    return std::fabs(a - b) <= 1e-9 * scale;
}

int main() {
    double s = 0.0;
    for (int i = 0; i < 10; ++i) s += 0.1;
    std::cout << std::setprecision(17) << s << '\n';
    std::cout << (s == 1.0) << ' ' << nearlyEqual(s, 1.0) << '\n';

    std::cout << std::setprecision(17) << (0.1 + 0.2) << ' '
              << (0.1 + 0.2 == 0.3) << '\n';
    std::cout << std::setprecision(6) << 1.0 / 3.0 << '\n';

    double nan = std::sqrt(-1.0);
    std::cout << std::isnan(nan) << ' ' << (nan == nan) << ' ' << (nan < 1.0) << '\n';

    std::cout << std::fixed << std::setprecision(2) << 2.0 / 3.0 << '\n';
    std::cout << (0.0 == -0.0) << '\n';
}
```

输出：
```text
0.99999999999999989
0 1
0.30000000000000004 0
0.333333
1 0 0
0.67
1
```

十次累加 0.1 并不等于 1.0，精确值差 1.1×10^-16；默认精度 6 位会把 0.30000000000000004 打印成 0.3，所以**打印看着对不代表它等于**。用相对误差比较（`nearlyEqual`）而不是 `==`。另外 `NaN` 与任何值比较都为 `false`，包括它自己，所以判断要用 `std::isnan`。

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct Base {
    virtual ~Base() = default;
    virtual std::string name() const { return "Base"; }
};

struct Derived : Base {
    std::string name() const override { return "Derived"; }
};

void byValue(Base b) { std::cout << "byValue:" << b.name() << '\n'; }
void byRef(const Base& b) { std::cout << "byRef:" << b.name() << '\n'; }

int main() {
    Derived d;
    byValue(d);
    byRef(d);

    std::vector<Base> v;
    v.push_back(d);
    std::cout << "vector<Base>:" << v[0].name() << '\n';

    std::vector<std::unique_ptr<Base>> p;
    p.push_back(std::make_unique<Derived>());
    std::cout << "vector<unique_ptr>:" << p[0]->name() << '\n';

    std::string a = "hello";
    std::string b = a;
    b[0] = 'H';
    std::cout << a << ' ' << b << '\n';
}
```

输出：
```text
byValue:Base
byRef:Derived
vector<Base>:Base
vector<unique_ptr>:Derived
hello Hello
```

`byValue(d)` 和 `v.push_back(d)` 都发生了**对象切片**：`Derived` 多出来的部分被切掉，只剩 `Base` 子对象，虚调用自然走 `Base::name()`。多态必须通过引用或指针（`Base&`、`Base*`、`unique_ptr<Base>`）。最后两行是值语义的对照：`std::string` 的拷贝是深拷贝，改 `b` 不影响 `a`——自己写类时如果成员是裸指针，默认拷贝就变成浅拷贝，两个对象共享一块内存，析构时二次释放。
