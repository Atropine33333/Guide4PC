## 5. 函数

函数是把一段逻辑命名、参数化、可复用的最小单位。这一节的重点不在"怎么写函数"，而在三件容易考、容易错的事：参数到底是拷贝还是别名、重载到底选了哪个、返回值到底是值还是引用。

### 5.1 声明、定义与 inline

- **声明**只给签名，可以出现任意多次：`int add(int, int);`（参数名可省）。
- **定义**给函数体，整个程序只能有一份（ODR，One Definition Rule）。
- 调用点之前必须已经见过声明，否则编译错。
- `inline` 的现代语义是"允许多个翻译单元各持一份相同定义，链接器只保留一份"，而不是"一定内联"。类内定义的成员函数、`constexpr` 函数隐式 inline。

❌ 在头文件里写普通函数定义，被两个 `.cpp` 包含 → `multiple definition` 链接错误。
✅ 头文件放声明（或 `inline` 定义），`.cpp` 放定义。

### 5.2 参数传递：值、指针、引用

| 方式 | 形参写法 | 函数内能否改实参 | 开销 | 能否为空 | 调用点写法 |
| --- | --- | --- | --- | --- | --- |
| 值 | `T x` | 不能（改的是副本） | 拷贝一份 T | 不涉及 | `f(a)` |
| 指针 | `T* p` | 能（`*p = ...`） | 拷一个地址 | 能（`nullptr`） | `f(&a)` |
| 引用 | `T& r` | 能（`r = ...`） | 无拷贝 | 不能 | `f(a)` |
| const 引用 | `const T& r` | 不能 | 无拷贝 | 不能 | `f(a)` |

选择原则：内置类型和小结构按值；大对象只读用 `const T&`；需要修改用 `T&`；"可能没有"用 `T*`。

```cpp-fragment
void f(int a[10]);   // 形参里的 10 被忽略，等价于 void f(int* a)
```

数组作参数永远退化成指针，长度信息在形参里丢失，这是第 6 节反复出现的主题。

```cpp
#include <iostream>

void byValue(int x)   { x += 1; }        // 副本，改不到调用者
void byPointer(int* p) { *p += 1; }      // 通过地址改
void byRef(int& r)     { r += 1; }       // 别名，直接改

void swapPtr(int* a, int* b) { int t = *a; *a = *b; *b = t; }
void swapRef(int& a, int& b) { int t = a; a = b; b = t; }

int main() {
    int a = 10;
    byValue(a);
    std::cout << a << '\n';       // 10
    byPointer(&a);
    std::cout << a << '\n';       // 11
    byRef(a);
    std::cout << a << '\n';       // 12

    int x = 1, y = 2;
    swapPtr(&x, &y);
    std::cout << x << ' ' << y << '\n';
    swapRef(x, y);
    std::cout << x << ' ' << y << '\n';
}
```

输出：
```text
10
11
12
2 1
1 2
```

### 5.3 默认参数与重载决议

默认参数的规则：

- 必须从右往左连续给：`void f(int a, int b = 1);` 合法，`void f(int a = 1, int b);` 非法。
- 同一个作用域里只能指定一次（通常写在声明处，定义处不要再写）。
- 默认实参在**调用点**求值，每次调用都重新计算。
- 默认参数不改变函数类型，也不能用来区分重载。

重载的规则：

- 同名、参数列表不同（个数、类型、顺序）。**返回类型不同不能构成重载**。
- 决议顺序：精确匹配 > 提升（`char`→`int`、`float`→`double`）> 标准转换（`int`→`double`）> 用户定义转换 > 省略号。
- 两个候选一样好就是二义性，编译错，不会"随便挑一个"。

注意：`f(0)` 在 `f(int)` 与 `f(char*)` 之间一定选 `int`；要选指针版本必须写 `nullptr`。这也是 C++11 之后不推荐 `NULL` 的原因之一。

```cpp
#include <iostream>

void f(int x)        { std::cout << "f(int) " << x << '\n'; }
void f(unsigned x)   { std::cout << "f(unsigned) " << x << '\n'; }
void f(double x)     { std::cout << "f(double) " << x << '\n'; }
void f(int a, int b) { std::cout << "f(int,int) " << a << ',' << b << '\n'; }

void g(int a, int b = 10, int c = 20) {
    std::cout << "g " << a << ' ' << b << ' ' << c << '\n';
}

int main() {
    f(3);      // 精确匹配 int
    f(3u);     // 精确匹配 unsigned
    f(3.0);    // 精确匹配 double
    f('a');    // char -> int 是“提升”，优先于换成 double/unsigned 的“转换”
    f(3.0f);   // float -> double 是浮点提升
    f(3, 4);   // 实参个数也参与决议

    g(1);      // 后面的参数取默认值
    g(1, 2);
    g(1, 2, 3); // 默认值可以被逐个覆盖
}
```

输出：
```text
f(int) 3
f(unsigned) 3
f(double) 3
f(int) 97
f(double) 3
f(int,int) 3,4
g 1 10 20
g 1 2 20
g 1 2 3
```

### 5.4 递归

递归只需要两件事：**出口**和**每层规模严格变小**。代价是调用栈：深度为 n 的递归会占用 n 个栈帧，每帧几十到几百字节，深度到十万级就可能栈溢出。

- 朴素 `fib(n)` 是指数级（约 1.6^n 次调用），`fib(40)` 已经上亿次，必须记忆化或改迭代。
- 尾递归 C++ 标准**不保证**优化，别指望编译器帮你把栈消掉。
- 递归深度大时改成迭代或显式栈。

下面的汉诺塔把移动次数用引用参数累加，顺带复习"引用传参"和"递归 + 引用"的组合。

```cpp
#include <iostream>

// 汉诺塔：把 n 个盘子从 from 移到 to，via 作中转
void hanoi(int n, char from, char to, char via, int& moves) {
    if (n == 0) return;                       // 递归出口
    hanoi(n - 1, from, via, to, moves);
    ++moves;
    std::cout << n << ": " << from << " -> " << to << '\n';
    hanoi(n - 1, via, to, from, moves);
}

int main() {
    int moves = 0;
    hanoi(3, 'A', 'C', 'B', moves);
    std::cout << "moves = " << moves << '\n';
}
```

输出：
```text
1: A -> C
2: A -> B
1: C -> B
3: A -> C
1: B -> A
2: B -> C
1: A -> C
moves = 7
```

### 5.5 返回引用与返回指针的陷阱

返回引用能避免拷贝、支持 `a[i] = x` 这种写法，但**返回谁**决定了它是否安全：

| 返回的东西 | 是否安全 | 说明 |
| --- | --- | --- |
| 静态/全局对象 | 安全 | 生命周期到程序结束 |
| 类的成员 | 安全（对象还活着时） | 注意对象本身何时销毁 |
| 容器元素 | 安全（容器不重新分配时） | `push_back`/`resize` 后可能失效 |
| 引用形参 | 安全 | 生命周期由调用者保证 |
| 局部变量 | **悬垂** | 函数返回后栈帧已销毁，UB |
| 值形参 | **悬垂** | 形参本身也是局部对象 |
| 临时量 | **悬垂** | 表达式结束即销毁 |

❌ 三种典型错误：

```cpp-fragment
int& bad1() { int x = 0; return x; }              // 返回局部变量：UB
int& bad2(int x) { return x; }                    // 返回值形参：UB
const std::string& bad3() { return std::string("tmp"); }  // 返回临时量：UB
int* bad4() { int x = 0; return &x; }             // 指针版同样悬垂
```

✅ 正确做法：返回容器元素/成员（并文档化失效条件）、返回值、或让调用者传入引用/指针作为输出参数。

```cpp
#include <iostream>
#include <string>
#include <vector>

// 返回引用：调用者拿到的是容器里的那个元素本身
int& element(std::vector<int>& v, std::size_t i) { return v[i]; }

// 返回 const 引用：形参是左值，生命周期比函数长，安全
const std::string& longer(const std::string& a, const std::string& b) {
    return a.size() >= b.size() ? a : b;
}

int main() {
    std::vector<int> v{10, 20, 30};
    element(v, 1) = 99;                     // 返回引用才可以放在赋值左边
    std::cout << v[1] << '\n';

    element(v, 0) += 5;
    std::cout << v[0] << '\n';

    std::string s1 = "hi";
    std::string s2 = "hello";
    std::cout << longer(s1, s2) << '\n';

    int& alias = element(v, 2);             // 引用是元素的别名
    alias = 7;
    std::cout << v[2] << '\n';
}
```

输出：
```text
99
15
hello
7
```

### 5.6 lambda、函数指针与 std::function

```cpp-fragment
[capture](params) -> ret { body }   // 返回类型通常可省
```

- 捕获列表在 **lambda 创建时**求值，不是调用时。`[=]` 拷贝当时的值，`[&]` 保存引用（被引用对象先死就悬垂）。
- 按值捕获的副本在 lambda 体内默认是 `const`，要改副本得写 `mutable`。
- 无捕获 lambda 可以隐式转换成函数指针；有捕获的不行。
- `std::function<R(Args...)>` 是类型擦除的容器，能装捕获 lambda、函数对象、成员函数包装，代价是一次间接调用（可能还有堆分配）。
- 函数指针只能指向普通函数或无捕获 lambda；模板参数（如 `std::sort` 的比较器）最快，`std::function` 最灵活。

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int addOne(int x) { return x + 1; }        // 普通函数

int main() {
    int base = 10;
    auto lam = [base](int x) { return x + base; };   // 按值捕获
    std::cout << lam(5) << '\n';

    int (*fp)(int) = addOne;                        // 函数指针
    std::cout << fp(5) << '\n';

    std::function<int(int)> f = lam;                // 可存放任意可调用体
    std::cout << f(5) << '\n';
    f = addOne;
    std::cout << f(5) << '\n';

    int outside = 100;
    auto byRef = [&outside](int x) { return x + outside; };
    std::cout << byRef(1) << ' ' << outside << '\n';

    std::vector<int> v{3, 1, 2};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    for (std::size_t i = 0; i < v.size(); ++i)
        std::cout << (i ? " " : "") << v[i];
    std::cout << '\n';
}
```

输出：
```text
15
6
15
6
101 100
3 2 1
```

### 5.7 main 的参数

```cpp-fragment
int main(int argc, char** argv) { ... }   // 等价于 char* argv[]
```

- `argc` 至少为 1，`argv[0]` 是程序名（可能是空串），`argv[argc]` 保证是 `nullptr`。
- 命令行参数全是字符串，要数字得自己 `std::stoi` / `std::strtol`。
- 标准只保证 `int main()` 和 `int main(int, char**)` 两种形式；`main` 不写 `return` 等价于 `return 0`。

### 5.8 本节坑清单

| 写法 | 后果 |
| --- | --- |
| 头文件里写非 inline 函数定义 | 链接期 multiple definition |
| 值形参里改参数，指望影响实参 | 静默错误，实参不变 |
| `void f(int a[10])` 后用 `sizeof(a)` 求长度 | 得到指针大小，静默错误 |
| 返回局部变量的引用/指针 | 悬垂，UB |
| `[&]` 捕获局部变量后把 lambda 存起来晚点调用 | 悬垂引用，UB |
| 用 `0`/`NULL` 调用指针重载 | 选中 `int` 版本，静默错误 |

## 6. 数组与多维数组

C 数组是"一块连续内存 + 编译期固定的长度"，不做任何边界检查；`std::array` 是同样布局的值语义包装；`std::vector` 是堆上的可增长数组。二维数组不是"矩阵对象"，而是"数组的数组"——这一句话决定了它的内存布局、传参写法和遍历性能。

### 6.1 定义、初始化与长度

```cpp-fragment
int a[5];              // 未初始化：函数内是垃圾值
int b[5] = {1, 2, 3};  // 后两个元素补 0
int c[5] = {};         // 全部清零
int d[] = {1, 2, 3};   // 长度由初始化列表推出：3
char s[] = "hi";       // 3 个字节：'h' 'i' '\0'
char t[3] = "hi";      // 恰好放得下，合法
char u[2] = "hi";      // 编译错：放不下 '\0'
constexpr int N = 5;
int e[N];              // 长度必须是编译期常量
```

要点：

- 长度必须是常量表达式。`int n; int a[n];` 是 VLA，标准 C++ 没有；clang 作为扩展接受并给出警告。
- 数组**不能拷贝、不能赋值**：`int x[3] = y;` 和 `x = y;` 都编译错。要拷贝用 `std::array`、`std::copy` 或 `memcpy`。
- 内置类型数组默认不初始化，"忘了 `{}`"是垃圾值的头号来源。

还有两件事 C 数组做不到：不能作为函数返回值（`int f()[3]` 非法，函数不能返回数组），也不能整体比较（`a == b` 比较的是两个退化后的地址，几乎永远为假）。需要"能返回、能比较、能拷贝"的数组，就该换成 `std::array`。

```cpp
#include <cstddef>
#include <iostream>
#include <iterator>

void showFirst(int arr[], int n) {   // 形参里的 int arr[] 会被调整成 int*
    std::cout << "n=" << n << " first=" << arr[0] << '\n';
}

void sizeOfPtr(int* p) {             // 到了函数里，只剩指针
    std::cout << (sizeof(p) == sizeof(int*)) << '\n';
}

int main() {
    int a[5] = {1, 2, 3};        // 后面两个元素补 0
    int b[] = {4, 5, 6, 7};      // 长度由初始化列表决定
    char s[] = "hi";             // 3 个字节：'h' 'i' '\0'

    std::cout << sizeof a / sizeof a[0] << '\n';
    std::cout << sizeof b / sizeof b[0] << '\n';
    std::cout << sizeof s << '\n';
    std::cout << a[0] << a[1] << a[2] << a[3] << a[4] << '\n';
    std::cout << std::size(a) << ' ' << std::size(b) << '\n';

    showFirst(a, static_cast<int>(std::size(a)));
    sizeOfPtr(a);
}
```

输出：
```text
5
4
3
12300
5 4
n=5 first=1
1
```

### 6.2 sizeof、std::size 与数组名退化

数组名在**大多数**表达式里会退化成指向首元素的指针，只有几种情况保留数组类型：`sizeof`、`&`、`decltype`、绑定到数组引用、以及 `std::size`/模板推导 `T(&)[N]`。

ASCII 布局（`sizeof(int) == 4`）：

```text
main 里：a 的类型是 int[5]，整块 20 字节
        +----+----+----+----+----+
        | 1  | 2  | 3  | 0  | 0  |
        +----+----+----+----+----+
        sizeof a      = 20
        sizeof a[0]   = 4
        sizeof a / sizeof a[0] = 5

被调函数里：形参 arr 的类型是 int*，只占 8 字节
        +--------------+
        |  &a[0]       | ---------> 上面那块内存
        +--------------+
        sizeof arr    = 8
```

所以"在函数里用 `sizeof` 算数组长度"永远错，而且不报错。要么把长度一起传，要么用 `template <std::size_t N> void f(int (&a)[N])` 保留维度，要么用 `std::span`。

### 6.3 越界

`a[i]` 不做任何检查，`i` 越界是未定义行为：可能读到垃圾、可能写坏相邻变量、也可能"看起来正常"，换个编译器就崩。

```cpp-fragment
int a[3] = {1, 2, 3};
a[3] = 4;                 // UB：越界写
int i = 3;
std::cout << a[i];        // UB：越界读
int* end = a + 3;         // 合法：one-past-end 只能取地址、比较，不能解引用
std::array<int, 3> s{};
s.at(3);                  // 抛 std::out_of_range（本讲义不演示异常）
```

❌ `for (int i = 0; i <= n; ++i)` 多走一格，是最常见的越界来源。
✅ 统一用 `i < n`；容器用范围 for 或 `size()`。

### 6.4 std::array

- 长度编译期固定，对象在栈上，布局与 `int[N]` 相同（`data()` 能拿到裸指针）。
- 值语义：可以拷贝、赋值、比较、结构化绑定，这一点 C 数组完全没有。
- **不会退化**，传参时 `sizeof` 还是整块大小。
- `size()` 是 `constexpr`，`at()` 带检查，`front()`/`back()` 方便。

传参写成模板才能保住长度：

```cpp-fragment
template <std::size_t N>
void show(const std::array<int, N>& a);   // N 被推导出来，不丢长度
```

### 6.5 std::vector

- 堆上**连续**存储，可增长；`size()` 是元素个数，`capacity()` 是已分配容量。
- `push_back` 摊还 O(1)，但扩容会重新分配并搬移所有元素，**此前取得的所有指针、引用、迭代器全部失效**。
- `reserve(n)` 只改容量，`resize(n)` 改元素个数并值初始化新元素。
- `operator[]` 不检查，`at()` 检查；`data()` 拿连续内存首地址。

```cpp
#include <array>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::array<int, 4> ax{1, 2, 3, 4};
    std::cout << ax.size() << ' ' << ax.front() << ' ' << ax.back() << '\n';
    std::cout << (sizeof(ax) == ax.size() * sizeof(int)) << '\n';

    std::vector<int> v{5, 6, 7};
    v.push_back(8);
    std::cout << v.size() << ' ' << std::accumulate(v.begin(), v.end(), 0) << '\n';

    std::vector<int> rows(3, -1);
    rows.resize(5);
    std::cout << rows.size() << ' ' << rows[4] << '\n';
}
```

输出：
```text
4 1 4
1
4 26
5 0
```

### 6.6 二维数组的内存布局：行主序

`int a[R][C]` 的类型链条是：`a` 是 `int[R][C]`，`a[r]` 是 `int[C]`（一个一维数组），`a[r][c]` 才是 `int`。整块内存按**行主序**（row-major）排列，最后一维连续：

```text
int a[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};

         c=0    c=1    c=2    c=3
      +------+------+------+------+
r=0   |  1   |  2   |  3   |  4   |   字节偏移 0 .. 15
      +------+------+------+------+
r=1   |  5   |  6   |  7   |  8   |   字节偏移 16 .. 31
      +------+------+------+------+
r=2   |  9   | 10   | 11   | 12   |   字节偏移 32 .. 47
      +------+------+------+------+

线性视图（一个 48 字节的连续块）：
  [1][2][3][4][5][6][7][8][9][10][11][12]
   ^a[0][0]              ^a[1][1]
```

地址公式（单位是元素，不是字节）：

```text
&a[r][c] == &a[0][0] + (r * C + c)
字节地址  == 基址 + (r * C + c) * sizeof(int)
sizeof a        = R * C * sizeof(int) = 48
sizeof a[0]     = C * sizeof(int)     = 16   <- 一行的字节数
sizeof a[0][0]  = sizeof(int)         = 4
```

三维同理，`int b[2][3][4]`：`&b[i][j][k] = 基址 + ((i * 3 + j) * 4 + k) * 4`。规律是"最右边一维连续，往左依次是更高层的步长"。

举个例子：`a[1][2]` 的扁平下标是 `1 * 4 + 2 = 6`，字节偏移是 `6 * 4 = 24`；`a[2][3]` 的下标是 `2 * 4 + 3 = 11`，正好是整块内存的最后一个元素。记住"行号乘列数再加列号"这一个公式，二维数组的地址计算题就都能算。

顺带记住两个类型事实。第一，`a[i]` 的类型是 `int[C]`（一个数组），所以 `sizeof a[i]` 得到的是一行的字节数，`a[i]` 也能整体绑定到 `int (&)[C]` 形参；`a[i][j]` 才是单个 `int`。第二，`a` 里**没有存任何指针**——所谓"行"是编译器用步长 `C * sizeof(int)` 算出来的偏移，不是查表得来的，所以 `int[3][4]` 不可能变成 `int**`。

再对比一种语言差异：C/C++ 是行主序（row-major），Fortran 和 MATLAB 是列主序（column-major），同一个矩阵存成线性字节的顺序正好相反。跨语言交换二进制数据时必须显式约定顺序，否则会出现"数字全对、行列颠倒"的经典 bug；转置的代价也因此不同。

```cpp
#include <cstddef>
#include <iostream>

int main() {
    int a[3][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
    };

    std::cout << "rows=" << sizeof a / sizeof a[0];
    std::cout << " cols=" << sizeof a[0] / sizeof a[0][0] << '\n';

    // 行主序：a[r][c] 的“扁平下标”是 r*4 + c
    std::cout << "flat(1,2)=" << 1 * 4 + 2 << '\n';
    std::cout << "flat(2,3)=" << 2 * 4 + 3 << '\n';

    // 真实字节偏移，用 sizeof(int) 换算成元素个数，避免输出真实地址
    std::ptrdiff_t bytes =
        reinterpret_cast<const char*>(&a[1][2]) - reinterpret_cast<const char*>(&a[0][0]);
    std::cout << "offset=" << bytes / static_cast<std::ptrdiff_t>(sizeof(int)) << '\n';
    std::cout << "a[1][2]=" << a[1][2] << '\n';
}
```

输出：
```text
rows=3 cols=4
flat(1,2)=6
flat(2,3)=11
offset=6
a[1][2]=7
```

### 6.7 按行遍历与按列遍历

两种顺序算出的和一样，但**访问模式**差很多。缓存行一般 64 字节，能装 16 个 `int`：按行遍历时地址连续，硬件预取器有效；按列遍历时每走一步跳过 `C * sizeof(int)` 字节，可能每一步都落在新的缓存行上。

对 3×4 这种小数组差异只体现在理论上，但对 1000×1000 的矩阵，按列遍历可能慢几倍。这就是"同样的算法，写法决定常数"。

以本节的 3×4 数组为例：整块只有 48 字节，小到几乎一定落在 1~2 个缓存行里，所以两种顺序看不出差别。但把规模放大到 1000×1000（4 MB，远超 L2），按行遍历是顺序扫描，载入一个缓存行之后紧跟的十几次访问全部命中；按列遍历每走一步要跳过 4000 字节，几乎每次访问都要新的缓存行——这才是"数量级"差距的来源。

```cpp
#include <iostream>

int main() {
    constexpr int R = 3;
    constexpr int C = 4;
    int a[R][C] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};

    long rowSum = 0;
    std::cout << "row-major:";
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            std::cout << ' ' << a[r][c];
            rowSum += a[r][c];
        }
    std::cout << "  sum=" << rowSum << '\n';

    long colSum = 0;
    std::cout << "col-major:";
    for (int c = 0; c < C; ++c)
        for (int r = 0; r < R; ++r) {
            std::cout << ' ' << a[r][c];
            colSum += a[r][c];
        }
    std::cout << "  sum=" << colSum << '\n';
}
```

输出：
```text
row-major: 1 2 3 4 5 6 7 8 9 10 11 12  sum=78
col-major: 1 5 9 2 6 10 3 7 11 4 8 12  sum=78
```

### 6.8 二维数组怎么传参

`int a[2][4]` 传参时第一维退化，**第二维必须写死**，因为它决定了 `a[r]` 一步跨多少：

```cpp-fragment
void f(int m[][4], int rows);      // 人话写法
void f(int (*m)[4], int rows);     // 真实类型：指向"4 个 int 的数组"的指针
void f(int** m, int rows);         // ❌ 类型不匹配，int[2][4] 不能转 int**
void f(int m[][], int rows);       // ❌ 第二维不能省，编译错
```

`int**` 和 `int[2][4]` 是两种完全不同的东西：前者是"指针数组，每行各自分配"，后者是"一整块连续内存"。把二维数组强转成 `int**` 再按下标访问，是典型的 UB。

`std::vector<std::vector<int>>` 每行都是独立分配，行与行之间不保证相邻，也不是 `int**`。想要连续二维内存，用一维 `std::vector<int> flat(R * C)` 加 `flat[r * C + c]`。

如果想同时做到"保留长度、不退化、又不绑定某一种容器"，C++20 的 `std::span<int>` 是标准答案：它内部只有"指针 + 长度"两样东西，能接 C 数组、`std::array`、`std::vector`，`size()` 随取随用，传参代价只有两个机器字。期中一般不考它，但它能解释"为什么现代 C++ 传数组不再写 `int a[]`"。

```cpp
#include <iostream>
#include <vector>

// 第一维可以退化，后面的维度必须写死：参数类型是 int (*)[4]
void sumRows(int (*m)[4], int rows) {
    for (int r = 0; r < rows; ++r) {
        int s = 0;
        for (int c = 0; c < 4; ++c) s += m[r][c];
        std::cout << "row " << r << " = " << s << '\n';
    }
}

// 传数组引用，维度由模板推导，不退化
template <int R, int C>
int total(const int (&m)[R][C]) {
    int s = 0;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) s += m[r][c];
    return s;
}

int main() {
    int a[2][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
    sumRows(a, 2);
    std::cout << "total=" << total(a) << '\n';

    // vector<vector> 是“指针的指针 + 每行独立分配”，不是一整块内存
    std::vector<std::vector<int>> v{{1, 2}, {3, 4, 5}};
    int s = 0;
    for (const auto& row : v)
        for (int x : row) s += x;
    std::cout << "rows=" << v.size() << " sum=" << s << '\n';
}
```

输出：
```text
row 0 = 10
row 1 = 26
total=36
rows=2 sum=15
```

### 6.9 本节坑清单

| 写法 | 后果 |
| --- | --- |
| 函数内 `sizeof(arr)` 求长度 | 得到 8（指针大小），静默错误 |
| `void f(int** m)` 接收 `int a[2][4]` | 编译错；强转则 UB |
| `char s[2] = "hi";` | 编译错，放不下 `'\0'` |
| `v.push_back(x)` 后继续用旧的 `&v[0]` | 悬垂指针，UB |
| `for (int i = 0; i <= n; ++i)` | 越界，UB |
| `int a[n];`（n 是变量） | 非标准 VLA，clang 给扩展警告 |
| `a = b;`（两个 C 数组） | 编译错，数组不可赋值 |

### 6.10 小结：三种"数组"怎么选

| 需求 | 选择 | 理由 |
| --- | --- | --- |
| 长度编译期固定，要和 C 接口互操作 | `int a[N]` | 零开销，布局最直接 |
| 长度编译期固定，要值语义（拷贝、比较、返回） | `std::array<int, N>` | 不退化，有 `size()`/`at()` |
| 长度运行期才知道，还要增长 | `std::vector<int>` | 连续存储，内存自动管理 |
| 二维且要连续内存 | `std::vector<int> flat(R * C)` | 缓存友好，传参只传一个指针 |
| 二维且要 `v[i][j]` 的写法 | `vector<vector<int>>` | 每行独立分配，缓存不友好 |

一句话：**能用 `vector`/`array` 就不要手写 C 数组**，只有对接 C 接口或需要精确控制布局时才用裸数组；无论用哪种，长度都必须自己管住。

## 7. 指针

指针是"存放地址的变量"。它的全部难点只有一句话：指针的值是一个地址，而地址是否有效、指向什么类型、能走多远，编译器几乎都不检查。这一节把"语法"和"什么时候会炸"分开讲清楚。

### 7.1 地址、解引用与 `&` 的三重身份

```text
64 位进程里的示意布局（地址仅示意，不作输出）
        +------------------+
   x    |        42        |   int x = 42;        假设 x 在 0x1000
        +------------------+
        +------------------+
   p    |      0x1000      |   int* p = &x;       p 在 0x2000
        +------------------+
                 |
                 +-------> x
```

- `&x` 取地址，类型 `int*`；`*p` 解引用，结果是**左值**，可以 `*p = 7`。
- `p->m` 等价于 `(*p).m`。
- `&` 有三个身份：取地址运算符、引用声明符、按位与。看上下文区分。

### 7.2 nullptr

- `nullptr` 的类型是 `std::nullptr_t`，可以隐式转成任意指针类型，且不会和整数混淆。
- `NULL` 通常就是 `0`，在重载里会选中 `int` 版本（见 5.3）。
- `if (p)` / `if (!p)` / `p != nullptr` 三种写法等价。
- `delete nullptr;` 是安全的空操作；但**解引用**空指针是 UB（多数平台直接段错误）。
- 未初始化的指针既不是空指针也不是有效指针，它是"野指针"，读它的值本身就是 UB。
- `nullptr` 和 `bool` 的转换是安全的：`if (p)` 判断的是"指针是否为空"，不是"它指向的值是否为真"。

### 7.3 指针类型与指针算术

指针的类型决定"一步走多远"：

| 表达式 | 含义 | 前进的字节数 |
| --- | --- | --- |
| `p + 1`（`int*`） | 下一个 int | `sizeof(int)` = 4 |
| `p + 1`（`char*`） | 下一个字节 | 1 |
| `p + 1`（`double*`） | 下一个 double | 8 |
| `&a + 1`（`int(*)[4]`） | 跳过整个数组 | 16 |

规则：

- 指针算术只在**同一个数组**（含 one-past-end）内有定义；两个不同数组的指针相减、比较都是 UB。
- `p - q` 的类型是 `std::ptrdiff_t`，单位是元素个数，不是字节。
- `void*` 不能解引用、不能算术；要按字节走就转 `char*`。
- 64 位下所有对象指针都是 8 字节，但"指针大小相同"不代表可以随便互相强转。

```cpp
#include <iostream>

int main() {
    int a[5] = {10, 20, 30, 40, 50};
    int* p = a;                 // 数组名退化成 &a[0]

    std::cout << *p << ' ' << *(p + 2) << ' ' << p[3] << '\n';
    std::cout << (&a[0] == p) << '\n';

    p += 2;                     // 指针加法按“元素”步进，不是按字节
    std::cout << *p << ' ' << p[-1] << '\n';
    std::cout << (p - a) << '\n';

    --p;
    std::cout << *p << ' ' << (p < &a[3]) << '\n';
}
```

输出：
```text
10 30 40
1
30 20
2
20 1
```

### 7.4 指针与数组：关系与区别

关系：数组名在大多数表达式里退化成首元素地址，所以 `a[i] == *(a + i) == *(i + a) == i[a]`。

区别：

| 维度 | 数组 `int a[4]` | 指针 `int* p` |
| --- | --- | --- |
| `sizeof` | 整块 16 | 8（地址大小） |
| 能否赋值 | 不能 | 能 |
| `&` 得到的类型 | `int(*)[4]`，指向整个数组 | `int**`，指向指针变量 |
| 是否有独立存储 | 没有，就是那块内存 | 有，存的是一个地址 |
| `+1` 的效果 | 退化成指针后走一个元素 | 走一个元素 |
| 能否用 `sizeof` 求长度 | 能（未退化时） | 不能 |

关键的一句：`a` 和 `&a` 的值相同，但**类型不同**，所以 `a + 1` 和 `&a + 1` 走的距离不同。

顺带说一句：标准库的迭代器就是"指针的泛化"。`begin()`/`end()` 构成半开区间 `[first, last)`，`end()` 指向最后一个元素的下一个位置，正好就是 `a + n` 这个 one-past-end 指针——这解释了为什么 `end()` 永远不能解引用，也解释了为什么 `for (auto it = v.begin(); it != v.end(); ++it)` 用 `!=` 而不是 `<`。

```cpp
#include <iostream>

int main() {
    int a[4] = {1, 2, 3, 4};

    std::cout << (sizeof a == 4 * sizeof(int)) << '\n';   // 1：数组名不退化时是整块
    int* p = a + 0;                                      // 一参与运算就退化成 int*
    std::cout << (sizeof(p) == sizeof(int*)) << '\n';     // 1

    const char* base = reinterpret_cast<const char*>(a);
    std::cout << (reinterpret_cast<const char*>(a + 1) - base) << '\n';   // 4：走一个 int
    std::cout << (reinterpret_cast<const char*>(&a + 1) - base) << '\n';  // 16：走整个数组

    int (*pa)[4] = &a;          // 指向“含 4 个 int 的数组”的指针
    std::cout << (*pa)[2] << ' ' << sizeof *pa / sizeof(int) << '\n';
}
```

输出：
```text
1
1
4
16
3 4
```

### 7.5 const 与指针的三种组合

| 写法 | 能否改 `*p` | 能否改 `p` | 读法 |
| --- | --- | --- | --- |
| `const int* p` | 否 | 能 | 指向 const int 的指针（底层 const） |
| `int* const p` | 能 | 否 | const 指针，指向 int（顶层 const） |
| `const int* const p` | 否 | 否 | 两者都锁死 |

从右往左读最可靠：`const int* const p` → "p 是一个 const 指针，指向 const int"。

两个容易忽略的点：

- 顶层 const 作形参时会被忽略：`void f(int* const p)` 和 `void f(int* p)` 是**同一个函数**，不能构成重载。
- `const int* p` 只是"不许通过 p 写"，不代表对象本身是常量——别人仍然可以改它，你通过 `p` 读到的值会跟着变。

还有一个与 `const` 强相关的坑：字符串字面量 `"abc"` 的类型是 `const char[4]`，存在只读段里。`char* p = "abc";` 在 C++11 起是非法（C++03 里合法但已弃用），因为标准不允许把 `const char*` 隐式转成 `char*`（clang 默认只报 `-Wwritable-strings` 警告，但标准上它已经是 ill-formed）；老代码里硬转再写入会直接崩。正确写法是 `const char* p = "abc";`，要改内容就用 `std::string`。

```cpp
#include <iostream>

int main() {
    int x = 1;
    int y = 2;

    const int* pc = &x;         // 指向 const：不能经 pc 改值，但 pc 可以改指向
    pc = &y;
    std::cout << *pc << '\n';

    int* const cp = &x;         // const 指针：不能改指向，但可以改值
    *cp = 10;
    std::cout << x << '\n';

    const int* const cpc = &y;  // 两者都锁死
    std::cout << *cpc << ' ' << (cpc == &y) << '\n';

    x = 7;                      // const 只是“访问限制”，对象本身不是常量
    std::cout << *cp << ' ' << x << '\n';
}
```

输出：
```text
2
10
2 1
7 7
```

### 7.6 二级指针与输出参数

```text
   x: [ 1 ]            y: [ 2 ]
   p: [ &y ]  --------^        p 现在指向 y
   pp:[ &p ]                   pp 指向 p

   *pp  == p   （一级指针本身）
   **pp == y == 2
   *pp = &x;  →  p 改成指向 x
```

用途：

1. **输出参数**：函数要修改调用者的指针本身（比如"找到就让你指过去"），只能传 `T**` 或返回新指针。
2. 动态二维数组、`char** argv`。
3. C 风格接口里的"分配并返回"。

二级指针更常见的身份是 **C 风格动态二维数组**：`int** m = new int*[R];` 之后每行再 `new int[C]`。行与行各自独立分配，地址互不相邻；释放顺序必须和分配相反——先逐行 `delete[] m[r]`，最后 `delete[] m`。这也是现代 C++ 宁可写 `vector<vector<int>>` 或一维 `vector` 加下标换算的原因：不是语法不优雅，而是"这块内存归谁、什么时候配对释放"这个问题，裸指针交给程序员自己保证，容器交给标准库保证。

❌ `void f(int** m)` 接收 `int a[2][4]`：类型不匹配，编译错；强转后按下标访问是 UB。

```cpp
#include <iostream>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// 输出参数：让调用者的指针改指向
void pick(int* first, int* second, bool useSecond, int** out) {
    *out = useSecond ? second : first;
}

int main() {
    int x = 1, y = 2;
    swap(&x, &y);
    std::cout << x << ' ' << y << '\n';

    int* chosen = nullptr;
    pick(&x, &y, true, &chosen);
    std::cout << *chosen << '\n';

    int** pp = &chosen;         // 二级指针：指针的地址
    *pp = &x;                   // 经二级指针改一级指针
    std::cout << *chosen << '\n';
}
```

输出：
```text
2 1
1
2
```

### 7.7 函数指针

```cpp-fragment
int add(int a, int b);
int (*fp)(int, int) = add;      // fp 是指针，指向“两个 int 进、一个 int 出”的函数
int r1 = fp(1, 2);              // 调用写法一
int r2 = (*fp)(1, 2);           // 调用写法二，与上面完全等价
using BinOp = int (*)(int, int);    // 用别名简化
BinOp table[2] = {add, sub};        // 函数指针数组：跳转表
```

- 函数名取地址和不取地址一样：`add` 和 `&add` 都是 `int(*)(int,int)`。
- 函数指针存的是**代码地址**，不能指向有捕获的 lambda（捕获 lambda 有状态，需要 `std::function`）。
- 与 `std::function` 对比见 5.6：函数指针零开销但只能指函数，`std::function` 灵活但有间接调用开销。

### 7.8 new / delete 与 new[] / delete[]

| 分配 | 释放 | 说明 |
| --- | --- | --- |
| `new T` | `delete p` | 单对象，调用一次构造/析构 |
| `new T[n]` | `delete[] p` | 数组，元素个数由实现记录 |
| `new T(args)` | `delete p` | 带初始化的单对象 |
| `new T[n]{...}` | `delete[] p` | 数组 + 初始化列表，其余元素值初始化 |

规则与坑：

- **必须配对**：`new[]` 配 `delete[]`，`new` 配 `delete`。错配是 UB，可能只析构第一个元素，也可能直接破坏堆。
- `delete p` 之后指针的值不变，它变成悬垂指针；再次 `delete` 是二次释放，UB。
- `new` 失败抛 `std::bad_alloc`；`new (std::nothrow) T` 失败返回 `nullptr`。
- `new` 出来的内存不会自动回收，忘记 `delete` 就是泄漏。
- 现代写法：用 `std::vector`、`std::string`、`std::unique_ptr` 代替裸 `new`/`delete`（RAII）。本讲义不展开智能指针。

实现上，`new T[n]` 常常在返回地址前面多存一份元素个数（有的实现叫 cookie），`delete[]` 正是靠它才知道要析构几个对象、要归还多少字节。这就是"`new[]` 必须配 `delete[]`"的物理原因：两者对这块内存布局的假设不一样，混用等于按错误的图纸拆房子。

```cpp
#include <iostream>

int main() {
    int* one = new int(42);     // 单对象
    std::cout << *one << '\n';
    delete one;                 // 与 new 配对

    int* arr = new int[5]{1, 2, 3, 4, 5};   // 数组
    int sum = 0;
    for (int i = 0; i < 5; ++i) sum += arr[i];
    std::cout << sum << '\n';
    delete[] arr;               // 与 new[] 配对

    one = nullptr;              // 释放后置空，避免悬垂
    arr = nullptr;
    std::cout << (one == nullptr) << (arr == nullptr) << '\n';
}
```

输出：
```text
42
15
11
```

### 7.9 野指针与悬垂指针

| 名字 | 含义 | 典型来源 |
| --- | --- | --- |
| 野指针（wild） | 值不确定，指向随机位置 | `int* p;` 未初始化 |
| 悬垂指针（dangling） | 指向已经销毁/失效的对象 | `delete` 后继续用、容器扩容后、返回局部变量地址 |
| 空指针 | 明确不指向任何对象 | `nullptr` |

```cpp-fragment
int* p;                 // 野指针：值不确定，读它都是 UB
*p = 1;                 // ❌ 可能直接崩

int* q = new int(5);
delete q;
std::cout << *q;        // ❌ 悬垂：内存已归还

std::vector<int> v{1, 2, 3};
int* first = &v[0];
v.push_back(4);         // 可能重新分配
std::cout << *first;    // ❌ 悬垂：first 可能已失效
```

防范：定义时就初始化（`= nullptr`）、释放后立刻置空、明确谁拥有这块内存、优先用容器和智能指针。调试时用 `-fsanitize=address` 编译，ASan 能抓到大部分 use-after-free，并直接打印出错的地址、分配点和释放点——比"程序偶尔算错一个数"好定位得多。

### 7.10 本节坑清单

| 写法 | 后果 |
| --- | --- |
| `int* p; *p = 1;` | 野指针解引用，UB |
| `delete` 后继续用指针 | 悬垂，UB |
| `new[]` 配 `delete` | UB，堆损坏或只析构一个元素 |
| 不同数组的指针相减/比较 | UB（看似能跑） |
| `p + 1` 以为前进 1 字节 | 静默错误，实际走 `sizeof(*p)` |
| `void*` 上做算术 | 编译错 |
| 用 `NULL` 调指针重载 | 选中 `int` 版本，静默错误 |

## 8. 引用

引用是已存在对象的**别名**，不是新对象。它的价值在于：既能像值一样书写（不用 `*`、不用 `&`），又能避免拷贝、支持"函数改实参"和"返回容器元素"。代价是两条硬约束——必须初始化、绑定后不能改。

### 8.1 别名、必须初始化

```text
   x: [ 10 ]  @0x1000
   r: 没有自己的存储，只是 x 的另一个名字
      &r == &x == 0x1000
      sizeof(r) == sizeof(int)

   对比指针：
   p: [ 0x1000 ] @0x2000   —— 指针自己是对象，占 8 字节
```

- 声明时必须绑定：`int& r;` 编译错。
- 绑定后不能改绑：`r = y;` 是**赋值**，把 y 的值写进 x。
- 没有"引用的引用"：`int& r2 = r;` 中 `r2` 仍然绑定 x。
- 不能有引用数组、不能有指向引用的指针、不能有引用的引用类型。
- 引用本身没有"顶层 const"的概念，因为改绑本来就不允许。

引用最常见的两个用途：**函数参数**（避免拷贝、支持修改实参）和**函数返回值**（让 `operator[]`、`at()` 能出现在赋值左边）。它不替代指针——需要"可空、可改绑、可做算术"的场合仍然用指针。

```cpp
#include <iostream>

int main() {
    int x = 10;
    int& r = x;                 // 引用：x 的别名
    ++r;                        // 等价于 ++x
    std::cout << x << ' ' << (&r == &x) << '\n';

    int& r2 = r;                // “引用的引用”还是 x 的别名
    r2 = 30;
    std::cout << x << '\n';

    int y = 7;
    r = y;                      // 这是赋值，不是重新绑定：x 被改成 7
    std::cout << x << ' ' << y << ' ' << (&r == &y) << '\n';
}
```

输出：
```text
11 1
30
7 7 0
```

### 8.2 引用传参

| 形参 | 是否拷贝 | 能否改实参 | 能否接字面量/临时量 | 典型用途 |
| --- | --- | --- | --- | --- |
| `T` | 拷贝 | 否 | 能 | 小对象、需要副本 |
| `T&` | 不拷贝 | 能 | 否 | 输出参数、原地修改 |
| `const T&` | 不拷贝 | 否 | 能 | 只读大对象（默认选择） |
| `T&&` | 不拷贝 | 能（移动语义） | 能（右值） | 移动构造、完美转发 |

`const T&` 之所以能接字面量，是因为编译器会为它创建一个临时对象并延长其生命周期到引用作用域结束。

引用的高频实战场景是范围 for：`for (const auto& x : v)` 读元素不拷贝；`for (auto& x : v)` 能原地修改元素；`for (auto x : v)` 改的只是副本，对容器毫无影响。这三种写法的区别几乎每年都考，判断标准就是"`&` 有没有让形参变成别名"。

下面用"拷贝构造函数打印一行"的办法，把"值传递真的拷贝了、const 引用真的没拷贝"变成可见的输出。

```cpp
#include <iostream>
#include <string>
#include <utility>

struct Probe {
    std::string name;
    explicit Probe(std::string n) : name(std::move(n)) {}
    Probe(const Probe& other) : name(other.name) {
        std::cout << "copy " << name << '\n';
    }
};

void byValue(Probe p) { (void)p; }
void byConstRef(const Probe& p) { (void)p; }

int main() {
    Probe s("beta");            // 一次构造，无拷贝
    std::cout << "byValue:" << '\n';
    byValue(s);
    std::cout << "byConstRef:" << '\n';
    byConstRef(s);

    const int& cr = 40 + 2;     // 绑定临时量，生命周期延长到 cr 结束
    std::cout << cr << '\n';
}
```

输出：
```text
byValue:
copy beta
byConstRef:
42
```

### 8.3 引用返回

返回引用让调用者拿到对象本身，所以可以出现在赋值左边：

```cpp-fragment
std::vector<int> v{1, 2, 3};
v[1] = 9;            // operator[] 返回 int&，才能这样写
```

安全性与 5.5 完全相同：能返回静态对象、成员、容器元素、引用形参；**不能**返回局部变量、值形参、临时量。额外一条：返回容器元素的引用后，如果容器扩容，这个引用立刻失效。

引用在运算符重载里几乎是必需品，这也是"为什么非得返回引用"的标准答案：`operator=` 返回 `T&` 才能写 `a = b = c`（右结合，每次赋值的值要能继续当左值用），`operator<<` 返回 `std::ostream&` 才能链式 `std::cout << a << b`，`operator[]` 返回 `T&` 才能写在赋值左边。若这些运算符改成返回值，`a = b = c` 会变成给临时对象赋值，链式输出也编译不过。

```cpp
#include <iostream>
#include <vector>

int& at(std::vector<int>& v, std::size_t i) { return v[i]; }

int main() {
    std::vector<int> v{10, 20, 30};

    at(v, 1) = 99;              // 返回引用，可以当左值用
    std::cout << v[1] << '\n';

    int& slot = at(v, 2);
    slot += 1;
    std::cout << v[2] << '\n';

    std::cout << (&at(v, 0) == &v[0]) << '\n';
}
```

输出：
```text
99
31
1
```

### 8.4 引用与指针对比

| 维度 | 指针 `T*` | 引用 `T&` |
| --- | --- | --- |
| 是否必须初始化 | 不必须（但应写 `nullptr`） | 必须，且必须绑定到对象 |
| 能否为空 | 能 | 不能 |
| 能否改绑 | 能 | 不能（赋值是改值） |
| 访问语法 | `*p`、`p->m` | 与普通变量一样 |
| 是否占独立存储 | 占（8 字节） | 语义上不占 |
| `sizeof` | 8 | `sizeof(T)` |
| 多级 | `T**`、`T***` | 没有"引用的引用" |
| 指针算术 | 有 | 无 |
| 能否组成数组 | 能 | 不能 |
| 典型场景 | 可选参数、动态内存、多态、C 接口 | 必存在的别名、运算符重载、避免拷贝 |

`f(int&)` 和 `f(int*)` 是**不同的重载**，可以共存；但 `f(int)` 和 `f(const int&)` 同时存在时，传左值会二义。

### 8.5 生命周期与悬垂引用

引用**不会**延长被绑定对象的生命周期，唯一的例外是：`const T&` 或 `T&&` **直接**绑定一个临时量时，临时量的生命周期延长到引用的作用域。

```cpp-fragment
const int& a = 42;             // ✅ 生命周期延长，安全
const int& b = a + 1;          // ✅ 延长
int&& c = 42;                  // ✅ 右值引用同样延长

struct S { const int& r; };
S s{42};                       // ❌ 悬垂：临时量在完整表达式结束就销毁
                               //    生命周期延长不会“穿过”成员引用

int& bad() { int x = 0; return x; }   // ❌ 悬垂：局部变量已销毁
int& bad2() { static int x = 0; return x; }  // ✅ 静态对象，安全
```

判断标准很简单：**被引用的对象一定比引用活得久吗？** 局部变量、临时量、已 `delete` 的对象、已扩容的容器元素，答案都是"不"。

引用与容器要格外小心：`auto& x = v[0];` 之后只要发生 `push_back`、`insert`、`resize` 或 `clear`，`x` 就可能悬垂。`vector` 扩容时会把元素搬到新内存，旧地址上的对象已经销毁，读它是 UB，写它更是。规则是：**持有一个元素引用/指针的时间，不要跨越容器的结构性修改**。

### 8.6 引用成员与初始化列表

引用成员有三条硬规则：

1. 必须用**成员初始化列表**初始化，不能在构造函数体里赋值（引用没有"先默认构造再赋值"这条路）。
2. 初始化顺序由**声明顺序**决定，与列表书写顺序无关（顺序不一致时 clang 会给 `-Wreorder` 警告）。
3. 含引用成员的类，编译器生成的拷贝赋值运算符被定义为 **deleted**，因为引用无法改绑。

```cpp-fragment
struct Bad {
    int& r;
    Bad(int& x) { r = x; }        // ❌ 编译错：引用成员未初始化
};
struct Good {
    int& r;
    Good(int& x) : r(x) {}        // ✅
};
```

需要"可空、可改绑"的成员时用指针，别用引用。

```cpp
#include <iostream>
#include <string>

struct Account {
    int& balance;               // 引用成员：必须初始化
    const std::string& owner;
    Account(int& b, const std::string& o) : balance(b), owner(o) {}
    void deposit(int amount) { balance += amount; }
};

int main() {
    int wallet = 100;
    std::string name = "ann";
    Account acc(wallet, name);
    acc.deposit(50);
    std::cout << wallet << ' ' << acc.owner << '\n';

    name = "bob";               // 外部改名，成员看到的是同一对象
    std::cout << acc.owner << '\n';
}
```

输出：
```text
150 ann
bob
```

### 8.7 左值、右值引用与 move 简介

- **左值**：有名字、能取地址，如变量、`*p`、`v[0]`。
- **右值**：临时量、字面量、函数返回的临时对象，如 `42`、`a + b`、`f()`。
- `T&` 只能绑左值；`const T&` 两者都能绑；`T&&` 只能绑右值。

`std::move(x)` **不移动任何东西**，它只是把 `x` 强制转换成右值引用（`static_cast<T&&>(x)`），从而让重载决议选中移动构造/移动赋值。真正的"搬走资源"发生在移动构造函数里。

```cpp-fragment
std::string a = "hello";
std::string b = std::move(a);   // 选中移动构造，a 的资源被搬走
// 此后 a 处于“有效但未指定”的状态：可以析构、可以重新赋值，不要读它的值
```

两个实践要点：

- 移动构造应标 `noexcept`，否则 `std::vector` 扩容时为了强异常保证会退化成拷贝。
- 返回局部变量时**不要**写 `return std::move(x);`，那会阻止 NRVO（返回值优化），反而多一次移动。

移动的收益在"持有堆内存"的类型上最明显：`std::string`、`std::vector` 的移动构造只搬几个指针和长度，复杂度 O(1)；拷贝则要重新分配并逐元素复制，O(n)。对只含内置类型的小结构（比如 `struct Point { int x, y; }`），移动和拷贝的开销其实一样，此时写移动构造没有意义。

```cpp
#include <iostream>
#include <string>
#include <utility>

struct Buffer {
    std::string data;
    explicit Buffer(std::string d) : data(std::move(d)) {
        std::cout << "ctor " << data << '\n';
    }
    Buffer(const Buffer& o) : data(o.data) {
        std::cout << "copy " << data << '\n';
    }
    Buffer(Buffer&& o) noexcept : data(std::move(o.data)) {
        std::cout << "move " << data << '\n';
    }
};

int main() {
    Buffer a("payload");
    Buffer b = a;               // 左值 -> 拷贝
    Buffer c = std::move(a);    // 右值 -> 移动（a.data 被搬空）
    std::cout << b.data << ' ' << c.data << '\n';
    std::cout << c.data.size() << '\n';
}
```

输出：
```text
ctor payload
copy payload
move payload
payload payload
7
```

### 8.8 本节坑清单

| 写法 | 后果 |
| --- | --- |
| `int& r;`（未初始化） | 编译错 |
| `r = y;` 以为重新绑定 | 静默错误：改的是被引用对象的值 |
| `int& f() { int x; return x; }` | 悬垂引用，UB |
| `S s{42};`（成员是 `const int&`） | 悬垂：延长规则不穿过成员 |
| 构造函数体里给引用成员赋值 | 编译错，必须用初始化列表 |
| 有引用成员的类做赋值 | 拷贝赋值被 deleted，编译错 |
| `return std::move(local);` | 阻止 NRVO，性能更差 |
| `T&&` 形参在函数体内当右值用 | 具名右值引用是左值，要再 `std::move` |
