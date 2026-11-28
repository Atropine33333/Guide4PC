## 5. 函数

函数的本质是"把一段计算命名、参数化，并规定它和外界的接口"。在 C++ 里这句话有三个层次：编译期它是一条**签名**，链接期它是一个**符号**，运行期它是**一段机器码加一个栈帧**。由此派生出三个高频问题：实参是拷贝还是别名、同名函数选了哪一个、返回值是值还是引用。它们的共同点是**语法上看不出来、结果却完全不同**，所以是选择题和"输出是什么"的重灾区。

### 5.1 声明、定义与 inline

**是什么。** 声明（declaration）只给出名字和类型，让编译器知道"怎么调用它"：要几个参数、各是什么类型、返回什么。定义（definition）在此之上给出函数体，让链接器找得到这段代码。

```cpp-fragment
int add(int, int);                       // 声明：参数名可以全部省略
int add(int a, int b) { return a + b; }  // 定义：给出函数体
```

**为什么必须分开。** C++ 是分离编译的：编译器一次只处理一个 `.cpp`（一个翻译单元 translation unit），处理 `main.cpp` 时看不到 `util.cpp` 里的定义。但调用 `add(1, 2)` 时它必须知道参数个数、类型（好做隐式转换和重载决议）和返回值怎么用，这些只能来自声明。所以规则是"调用点之前必须见过**声明**"，不是"必须见过定义"。链接器最后把各翻译单元的符号表拼起来，才填上调用指令里的地址。

**ODR（One Definition Rule，单一定义规则）。** 一个函数在整个程序里只能有一份定义。原因很直接：链接器按"名字 + 参数类型"（名字修饰 name mangling 之后的符号）合并，两份不同的实现无法二选一。例外是 `inline` 函数、模板、类内定义的成员函数——它们可以出现在多个翻译单元里，但每一份必须**逐字相同**，链接器任意保留一份。

**`inline` 到底是什么意思。** 它的核心含义是"允许违反 ODR：多个翻译单元可以各持一份相同定义"，而不是"必须内联展开"——是否内联由优化器决定，`-O0` 下基本不会。实际用途是"把定义放进头文件而不引起重复定义"；至于性能，写不写通常没区别。

**内存布局。** 函数体编译成机器码放在 `.text` 段，整个程序只有一份，不随调用次数增长。每次调用增长的是**栈**：返回地址、保存的寄存器、局部变量、临时量都在新栈帧里。

```text
进程地址空间（简化，从高地址到低地址）

  +------------------+
  | 栈 stack          |  每次函数调用向下长出一个栈帧（x86-64 约定）
  |                  |  返回地址、保存的 rbp、局部变量都在这里
  +------------------+
  | 堆 heap           |  new、vector 的动态内存
  +------------------+
  | .bss              |  未初始化的全局/静态变量（启动时清零）
  +------------------+
  | .data             |  已初始化的全局/静态变量
  +------------------+
  | .text             |  所有函数的机器码，只有一份
  +------------------+
```

`static` 局部变量是例外：写在函数内，存储却在 `.data`/`.bss`，生命周期到程序结束，函数返回后值还在。

❌ **反例一：头文件里写普通函数定义。** 这是最常见的链接错误来源。

```cpp-fragment
// ---------- util.h ----------
int square(int x) { return x * x; }   // ❌ 非 inline 的完整定义

// ---------- a.cpp ----------
#include "util.h"
int main() { return square(3); }

// ---------- b.cpp ----------
#include "util.h"
int twice(int x) { return square(x) * 2; }
```

两个 `.cpp` **各自都能编译通过**（每个翻译单元里都有完整定义），错误出现在链接期：`multiple definition of 'square(int)'`（GNU ld）或 `duplicate symbol 'square(int)'`（Apple ld）。这种"编译过了、链接挂了"的错误容易被当成环境问题。修法二选一：头文件只留 `int square(int);`，定义放进一个 `.cpp`；或写成 `inline int square(int x) { ... }`。

### 5.2 参数传递：值、指针、引用

**是什么。** 三种形参对应三种完全不同的机制：

- **值形参** `T x`：在函数栈帧里**新建一个对象**，用实参拷贝初始化。函数体里的 `x` 是这个新对象，和实参是两个不同的对象。
- **指针形参** `T* p`：拷贝的是**地址值**。`p` 本身是新对象（一个机器字），但 `*p` 访问的是调用者的那个对象。
- **引用形参** `T& r`：语言层面 `r` **就是**实参的别名，不构造新对象；实现上通常仍是一个地址，但语法上不必解引用，也不能为空。

```text
调用 byValue(a) 之后              调用 byRef(a) 之后

main 的栈帧                       main 的栈帧
  +--------+                        +--------+
  | a = 10 |                        | a = 10 | <-----+
  +--------+                        +--------+       |
                                                     |
byValue 的栈帧（新对象 x）         byRef 的栈帧（别名 r）
  +--------+                        +--------+
  | x = 11 |  <- 改的是副本        | &a     | ------+
  +--------+                        +--------+
```

**为什么。** C 只有值传递，"改实参"只能传地址，于是有满屏 `*` 和 `&` 的 `swap(&x, &y)`。引用是语法糖：调用点写 `swap(x, y)`，函数体写 `a = b`，既可读又不可能为空。**代价**是调用点看不出它会被修改——所以工程规范要求"输出参数用指针、输入用 `const T&`"，让调用点至少有个 `&` 作提示。

**怎么选。**

| 方式 | 形参写法 | 能否改实参 | 开销 | 能否为空 | 调用点 |
| --- | --- | --- | --- | --- | --- |
| 值 | `T x` | 不能 | 一次拷贝构造（加析构） | 不涉及 | `f(a)` |
| 指针 | `T* p` | 能，`*p = ...` | 拷一个地址 | 能 | `f(&a)` |
| 引用 | `T& r` | 能，`r = ...` | 无 | 不能 | `f(a)` |
| const 引用 | `const T& r` | 不能 | 无 | 不能 | `f(a)` |

原则：内置类型和小结构按值传，寄存器就能带走；`std::string`、`std::vector` 这类大对象，只读传 `const T&`，要改传 `T&`；"可能不存在"用 `T*`，因为 `nullptr` 合法而引用没有空值。**不要**为"看起来快"给 `int` 加 `const&`。

**与数组的边界。** 数组不能按值传递。写 `void f(int a[10])` 时，形参里的 `10` 被编译器**直接忽略**，类型调整（adjust）为 `int*`；`void f(int a[])` 同理。所以函数内 `sizeof(a)` 是指针大小而不是数组大小。这是"数组必须额外传长度"的根源，第 6 节还会反复回到它。

```cpp
#include <iostream>

void byValue(int x)    { x += 1; }      // 改副本
void byPointer(int* p) { *p += 1; }     // 通过地址改
void byRef(int& r)     { r += 1; }      // 别名，直接改

void swapPtr(int* a, int* b) { int t = *a; *a = *b; *b = t; }
void swapRef(int& a, int& b) { int t = a; a = b; b = t; }

int main() {
    int a = 10;
    byValue(a);
    std::cout << "after value:   " << a << '\n';
    byPointer(&a);
    std::cout << "after pointer: " << a << '\n';
    byRef(a);
    std::cout << "after ref:     " << a << '\n';

    int x = 1, y = 2;
    swapPtr(&x, &y);
    std::cout << "swapPtr: " << x << ' ' << y << '\n';
    swapRef(x, y);
    std::cout << "swapRef: " << x << ' ' << y << '\n';
}
```

输出：
```text
after value:   10
after pointer: 11
after ref:     12
swapPtr: 2 1
swapRef: 1 2
```

两次交换只差一个 `&`，语义等价——这正是引用存在的理由。`byValue` 那行里 `x += 1` 确实执行了，改的是副本，所以 `a` 纹丝不动。

### 5.3 默认参数与重载决议

**默认实参（default argument）是什么。** 在形参后面写 `= 表达式`，调用时可以省略尾部的实参，编译器在**调用点**把默认表达式补上。

1. **必须从右往左连续给**：调用时只能从尾部省略，`f(1)` 无法说明省的是哪一个，所以 `void f(int a = 1, int b);` 非法。
2. **同一作用域只指定一次**，通常写在声明处。否则两个默认值冲突。
3. **在调用点求值**，每次调用重新计算，因此可以依赖调用点可见的变量，也不是"算好一次的静态值"。
4. **不改变函数类型**：`void f(int, int = 0)` 仍是 `void(int, int)`，所以不能用来区分重载，也不能通过函数指针传递。

还有一条交叉坑：默认实参按**静态类型**编译期绑定，虚函数按动态类型派发。通过基类指针调 `p->draw()` 时，函数体是派生类的，默认参数却是基类那份。

**重载（overload）是什么。** 同一作用域内，同名函数的**参数列表**不同（个数、类型、顺序）。返回类型不同**不构成**重载：`f(x)` 的类型只由实参决定，编译器没有依据用返回类型去选。

**决议顺序**（从好到差）：精确匹配 > 提升（`char`/`short`→`int`，`float`→`double`）> 标准转换（`int`↔`double`、`int`→`unsigned` 等）> 用户定义转换 > 省略号。同一等级有两个候选就是**二义性**，直接编译报错。

为什么 `f(0)` 选 `f(int)` 而不是 `f(char*)`：`0` 的类型就是 `int`，精确匹配；转指针属于"空指针转换"，是标准转换，等级更低。要选指针版本必须写 `nullptr`，它有独立类型 `std::nullptr_t`。这也是 C++11 后不推荐 `NULL` 的原因之一：它多半就是 `0`。

```cpp
#include <iostream>

void f(int x)        { std::cout << "f(int) " << x << '\n'; }
void f(double x)     { std::cout << "f(double) " << x << '\n'; }
void f(int a, int b) { std::cout << "f(int,int) " << a << ',' << b << '\n'; }
void f(int* p)       { std::cout << "f(int*) " << (p ? "non-null" : "null") << '\n'; }

void g(int a, int b = 10, int c = 20) {
    std::cout << "g " << a << ' ' << b << ' ' << c << '\n';
}

int main() {
    f(3);        // 精确匹配 int
    f(3.0);      // 精确匹配 double
    f(3.0f);     // float 提升为 double，优于转成 int 的“转换”
    f('a');      // char 提升为 int，优于转成 double
    f(3, 4);     // 个数参与决议
    f(0);        // 0 是 int：选 f(int)，不是 f(int*)
    f(nullptr);  // 只有 f(int*) 能精确接住

    g(1);        // 后两个取默认值
    g(1, 2);     // 只覆盖 b
    g(1, 2, 3);  // 全部覆盖
}
```

输出：
```text
f(int) 3
f(double) 3
f(double) 3
f(int) 97
f(int,int) 3,4
f(int) 0
f(int*) null
g 1 10 20
g 1 2 20
g 1 2 3
```

`f(3.0f)` 值得盯住：`float` 既能提升成 `double`，也能转换成 `int`，前者是"提升"、后者是"转换"，等级不同，所以选 `double`。`f('a')` 对称：`char`→`int` 是提升，`char`→`double` 是转换。两行放在一起记，比背规则表有效。

❌ **反例二：默认参数与重载的四种典型错误。**

```cpp-fragment
void f(int a = 1, int b);            // ❌ 默认参数必须从右往左连续
void g(int a, int b = 2);
void g(int a, int b = 2) { }         // ❌ 同一作用域重复指定默认实参
void h(int);
double h(int);                       // ❌ 返回类型不同，不构成重载

void k(int);
void k(int, int = 0);                // 语法合法，但 k(1) 变成二义性
```

前三个是编译错误，报错分别是 `default argument missing for parameter`、`redefinition of default argument`、`cannot be overloaded`。第四个更阴险：它**能编译**，但任何 `k(1)` 都报 `call to 'k' is ambiguous`——一个候选精确匹配、一个靠默认参数补齐，编译器认为同样好。默认参数把"参数个数不同"这个重载依据破坏了，这是设计重载时最容易忽略的相互作用。

### 5.4 递归

**是什么。** 函数直接或间接调用自身。它需要两个条件：**出口**（某个输入不再递归）和**每层规模严格变小**（保证有限步到达出口）。少一个就是无限递归，程序以栈溢出（stack overflow）结束——这不是"死循环"，栈用完时进程被操作系统直接杀掉，通常表现为 `Segmentation fault`。

**为什么贵。** 每次调用都要建一个栈帧：返回地址、保存的 rbp、局部变量、对齐填充，几十字节。深度 n 意味着 n 个栈帧**同时存活**。

```text
fact(3) 的调用过程（栈向下增长）

  fact(3)  n=3   等待 3 * fact(2)
    fact(2)  n=2   等待 2 * fact(1)
      fact(1)  n=1   return 1        <- 最深处，栈上有 3 帧
    fact(2) 返回 2
  fact(3) 返回 6
```

代价分两部分：**时间**是每层的调用开销，**空间**是栈深度。默认栈一般 8 MB，一帧几十到几百字节，深度到十万级通常就溢出——所以"递归求 1 到 n 的和"在 n 很大时不是慢，是把栈用完。

**边界。** 尾递归在函数式语言里能优化成循环，C++ 标准**不要求**，GCC/Clang 在 `-O2` 常能做到但不能依赖。朴素 `fib(n)` 是另一类问题：递归树约 1.6^n 个结点，`fib(40)` 上亿次调用，慢在**重复计算**而不是栈深，修法是记忆化（memoization）或迭代。两种"递归不行"要分清：一个缺空间，一个缺时间。

```cpp
#include <iostream>

long long fact(int n) { return n <= 1 ? 1 : n * fact(n - 1); }

// 汉诺塔：把 n 个盘子从 from 移到 to，via 作中转；moves 用引用累加
void hanoi(int n, char from, char to, char via, int& moves) {
    if (n == 0) return;                    // 出口
    hanoi(n - 1, from, via, to, moves);
    ++moves;
    std::cout << n << ": " << from << " -> " << to << '\n';
    hanoi(n - 1, via, to, from, moves);
}

int main() {
    std::cout << "fact(10)=" << fact(10) << '\n';

    int moves = 0;
    hanoi(3, 'A', 'C', 'B', moves);
    std::cout << "moves=" << moves << '\n';
}
```

输出：
```text
fact(10)=3628800
1: A -> C
2: A -> B
1: C -> B
3: A -> C
1: B -> A
2: B -> C
1: A -> C
moves=7
```

`hanoi` 里 `moves` 必须用引用：按值传的话每层改的都是自己的副本，回到 `main` 仍是 0。递归不改变传参语义，每一层都是一次普通调用。

### 5.5 返回引用与返回指针

**是什么。** `T& f()` 返回别名，`T* f()` 返回地址。是否合法只取决于**被返回对象在函数返回之后是否还活着**，与语法形式无关。

| 返回的东西 | 是否安全 | 原因 |
| --- | --- | --- |
| 静态/全局对象 | 安全 | 生命周期到程序结束 |
| 类的成员 | 安全（对象还活着时） | 注意对象本身何时销毁 |
| 容器元素 | 安全（容器不重新分配时） | `push_back`/`resize` 后可能失效 |
| 引用形参 | 安全 | 生命周期由调用者保证 |
| 局部变量 | **悬垂** | 函数返回后栈帧已销毁 |
| 值形参 | **悬垂** | 形参本身也是局部对象 |
| 临时量 | **悬垂** | 表达式结束即销毁 |

**为什么允许。** 两个刚需：避免拷贝，以及让函数调用能出现在赋值左边。`operator[]` 必须返回引用，否则 `v[0] = 1` 无法工作；`at`、`map::operator[]` 同理。反过来说，返回引用等于给调用者一个"绕过封装改内部状态"的入口，所以 `const T&` 更常见。

❌ **反例三：三种悬垂。**

```cpp-fragment
int& bad1() { int x = 0; return x; }                      // ❌ 返回局部变量
int& bad2(int x) { return x; }                            // ❌ 返回值形参
const std::string& bad3() { return std::string("tmp"); }  // ❌ 返回临时量
int* bad4() { int x = 0; return &x; }                     // ❌ 指针版同样悬垂
```

这四个都能编译（`bad1`/`bad4` 在 clang 下有 `-Wreturn-stack-address` 警告，`bad2`/`bad3` 往往一声不吭），运行时是未定义行为：函数返回后栈帧被回收，那块内存可能立刻被下一次调用覆盖。典型现象是"单独测试时值还对，加一行 `std::cout` 就变了"。判断口诀只有一句：**返回引用或指针之前，先问"这个对象在函数返回之后还活着吗"**。

```cpp
#include <iostream>
#include <string>
#include <vector>

// 返回引用：调用者拿到的是容器里那个元素本身
int& element(std::vector<int>& v, std::size_t i) { return v[i]; }

// 返回 const 引用：两个形参都比函数活得久，选中的那个也活着
const std::string& longer(const std::string& a, const std::string& b) {
    return a.size() >= b.size() ? a : b;
}

int main() {
    std::vector<int> v{10, 20, 30};

    element(v, 1) = 99;                 // 返回引用才能放在赋值左边
    std::cout << v[1] << '\n';

    element(v, 0) += 5;                 // 复合赋值同样作用于容器元素
    std::cout << v[0] << '\n';

    std::string s1 = "hi";
    std::string s2 = "hello";
    std::cout << longer(s1, s2) << '\n';

    int& alias = element(v, 2);         // 引用就是元素的别名
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

`element(v, 1) = 99` 之所以成立，是因为返回的是 `v[1]` 本身而不是副本。签名若写成 `int element(...)`，这行会编译错（`expression is not assignable`）——很好的自检：**能出现在赋值左边的函数调用，一定返回了引用**。

### 5.6 lambda、函数指针与 std::function

**是什么。** 三者都是"把一段代码当值来传递"，但机制不同：

- **函数指针** `R (*p)(Args...)`：存一个函数地址。只能指向普通函数、静态成员函数、无捕获 lambda。
- **lambda**：编译器生成一个匿名的**闭包类**，捕获的变量成为它的数据成员，函数体成为 `operator()`。`[base]` 意味着类里有一个 `int base` 成员，创建时用当前的 `base` 初始化。
- **`std::function<R(Args...)>`**：类型擦除（type erasure）的容器，内部存"可调用对象 + 调用它的方式"，能装下捕获 lambda、函数对象、成员函数包装。

**为什么三者并存。** 这是性能与灵活性的谱系：模板参数（如 `sort` 的比较器）编译期就知道类型，可内联，最快；函数指针多一次间接跳转还挡住内联；`std::function` 再付出类型擦除的代价，对象大时还可能堆分配。经验法则：能编译期定下来就用模板/`auto`，要存进容器或跨接口传递才用 `std::function`。

**内存布局。** 闭包对象的大小约等于捕获变量的大小（加对齐填充），无捕获 lambda 是 1 字节（空类也要有唯一地址）。`operator()` 的代码只有一份，在 `.text` 里。

```text
auto lam = [base](int x) { return x + base; };   // base 是 int

lam 对象（栈上，大小同 int）
  +----------+
  | base = 10|        operator()(int) 的机器码在 .text，全局一份
  +----------+

[&base] 版本：对象里存的是 &base（一个地址），base 本身还在 main 的栈帧里
```

**边界。** 捕获列表在 **lambda 创建时**求值：`[=]` 拷贝那一刻的值，之后原变量怎么变都不影响；`[&]` 存引用，被引用对象先死就悬垂。按值捕获的副本在体内默认是 `const`，要改必须写 `mutable`。无捕获 lambda 能隐式转成函数指针，有捕获的不行——它要携带数据，而函数指针只有一个地址。另外，成员函数里的 `[=]` 捕获的是 `this` 而不是成员的副本（C++20 起这种隐式捕获已弃用）。

```cpp
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

int addOne(int x) { return x + 1; }

int main() {
    int base = 10;
    auto lam = [base](int x) { return x + base; };   // 按值捕获，存的是副本
    std::cout << lam(5) << '\n';

    auto counter = [n = 0]() mutable { return ++n; };  // 副本默认 const，要改就 mutable
    std::cout << counter() << '\n';
    std::cout << counter() << '\n';

    int (*sq)(int) = [](int x) { return x * x; };      // 无捕获：可转函数指针
    std::cout << sq(6) << '\n';

    std::function<int(int)> fn = lam;                  // 类型擦除，什么都能装
    std::cout << fn(5) << '\n';
    fn = addOne;
    std::cout << fn(5) << '\n';

    int outside = 100;
    auto byRef = [&outside](int x) { return x + outside; };
    outside = 200;                                     // 引用看得到这个修改
    std::cout << byRef(1) << '\n';

    std::cout << "lambda size==int: " << (sizeof lam == sizeof(int)) << '\n';

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
1
2
36
15
6
201
lambda size==int: 1
3 2 1
```

`byRef(1)` 输出 201 而不是 101，因为捕获的是引用，`outside = 200` 之后 lambda 看到的是新值。反过来，若把 `byRef` 存进 `std::function`、等 `outside` 销毁后再调用，就是标准的悬垂引用 UB——这正是"lambda 别随便返回或存起来"的原因。

### 5.7 main 的参数

**是什么。** 标准只保证两种形式：`int main()` 和 `int main(int argc, char** argv)`（等价于 `char* argv[]`）。`argc` 是参数个数，**含程序名**，所以至少为 1；`argv[0]` 是程序名（可能是空串），`argv[argc]` 标准保证是 `nullptr`。

**为什么长这样。** 进程启动时，启动代码把命令行拆成一个个以 `\0` 结尾的字符串，连同指针数组放在栈顶，再调用 `main`。所以 `argv` 指向的内存在栈上，内容按标准只读（改它属于 UB）。参数全是字符串也很好理解：命令行没有类型信息，`-n 42` 里的 `42` 只是两个字符。

**边界。** 要数字得自己转换：`std::stoi`（抛异常）、`std::strtol`（不抛，能查错误）。`argv[i]` 的合法范围是 `0..argc-1`，`argv[argc]` 只能读来判空，不能解引用。`main` 不写 `return` 等价于 `return 0`。

```cpp
#include <iostream>
#include <string>

// 真实运行时由启动代码填好 argc/argv，这里手工构造一份来演示它的语义
int sumArgs(int argc, char* argv[]) {
    int sum = 0;
    for (int i = 1; i < argc; ++i) sum += std::stoi(argv[i]);
    return sum;
}

int main() {
    char p0[] = "prog.exe";
    char p1[] = "10";
    char p2[] = "20";
    char p3[] = "12";
    char* argv[] = {p0, p1, p2, p3, nullptr};
    int argc = 4;                       // 程序名 + 3 个参数

    std::cout << "argc=" << argc << '\n';
    std::cout << "argv[0]=" << argv[0] << '\n';
    std::cout << "sum=" << sumArgs(argc, argv) << '\n';
    std::cout << "argv[argc]==nullptr: " << (argv[argc] == nullptr) << '\n';
}
```

输出：
```text
argc=4
argv[0]=prog.exe
sum=42
argv[argc]==nullptr: 1
```

循环从 `i = 1` 开始（`argv[0]` 是程序名，不是数据），到 `i < argc` 结束（`argv[argc]` 是哨兵）。这两个边界正好对应"数组长度必须自己管"的两端。

### 5.8 本节坑清单

| 写法 | 后果 |
| --- | --- |
| 头文件里写非 inline 函数定义 | 链接期 `multiple definition` |
| 值形参里改参数，指望影响实参 | 静默错误，实参不变 |
| `void f(int a[10])` 后用 `sizeof(a)` 求长度 | 得到指针大小，静默错误 |
| 返回局部变量的引用/指针 | 悬垂，UB |
| `[&]` 捕获局部变量后把 lambda 存起来晚点调用 | 悬垂引用，UB |
| 用 `0`/`NULL` 调用指针重载 | 选中 `int` 版本，静默错误 |
| `void f(int a = 1, int b);` | 编译错：默认参数必须靠右连续 |
| 重载时加一个带默认参数的版本 | 某些调用变成二义性 |

一句话总结：**函数的三件事——参数怎么进、重载怎么选、返回值怎么出——编译器都按固定规则办，"看起来应该"一律不成立。**

## 6. 数组与多维数组

C 数组是"一块连续内存 + 编译期固定的长度 + 零边界检查"。这三件事直接推出三个后果：数组不能拷贝、不能赋值、不能整体比较；数组名在大多数场合会退化成指针，长度信息随之丢失；`a[i]` 只是 `*(a + i)` 的语法糖，越界不报错。`std::array` 补上值语义，`std::vector` 补上运行期长度，但它们都改不了"元素在内存里连续排列"这个底层事实。二维数组则是"数组的数组"，不是"矩阵对象"——这一句话决定了它的内存布局、传参写法和遍历性能。

本节所有字节数都按 `sizeof(int) == 4`、64 位平台（`sizeof(int*) == 8`）计算；标准只保证 `sizeof(int) >= 2`，换平台时字节数要按比例改，但下标公式和退化规则不变。

### 6.1 定义、初始化与长度

**是什么。** `T a[N];` 定义一个含 N 个 `T` 的数组，类型是 `T[N]`，N 必须是**常量表达式**（converted constant expression）。类型的一部分就是长度，所以 `int[3]` 和 `int[4]` 是两种不同的类型，不能互相赋值。

初始化写法与结果：

| 写法 | 结果 |
| --- | --- |
| `int a[5];` | 块作用域：默认初始化，元素值**不确定**；全局/静态：零初始化 |
| `int a[5] = {1, 2, 3};` | 前三个是 1、2、3，其余**补 0** |
| `int a[5] = {};` | 全部 0 |
| `int a[5]{};` | 同上（C++11 统一初始化） |
| `int a[] = {1, 2, 3};` | 长度由初始化项推出，等于 3 |
| `int a[5] = {1,2,3,4,5,6};` | 编译错：初始化项过多 |
| `char s[] = "hi";` | 3 字节：`'h'`、`'i'`、`'\0'` |
| `char s[2] = "hi";` | 编译错：放不下结尾的 `'\0'` |
| `std::array<int, 3> a{1, 2, 3};` | 聚合初始化，长度是类型的一部分 |

**为什么 `{1,2,3}` 后面会自动补零。** 规则是：初始化项少于元素个数时，剩余元素按空列表 `{}` 拷贝初始化。对 `int` 来说空列表就是 0；对类类型来说就是调用默认构造函数。这是 C 继承下来的"部分初始化自动补零"，也是数组唯一会"顺手帮你清零"的场合。

**为什么 `int a[5];` 不清零。** 函数内的自动变量是**默认初始化**（default-initialized），对内置类型就是"什么都不做"，留下的是不确定值（indeterminate）。零初始化只对静态存储期对象（全局、`static`、`thread_local`）保证。这是 C 的设计哲学：不为你不用的东西付费。代价是"忘了写 `{}`"成了垃圾值的头号来源——所以考试里凡是出现"局部数组没初始化就求和/求最大值"，答案一定是"结果不确定"，而不是某个具体数字。

**为什么长度必须是常量表达式。** 数组要在栈帧里预留固定空间，`sizeof` 要在编译期算出，`a[i]` 要在编译期化成"基址 + i * sizeof(T)"。这三件事都要求 N 在编译期已知。变长数组（VLA，variable length array）是 C99 的特性，**C++ 标准没有**；GCC/Clang 把它当扩展接受，`-Wall` 下会报 `variable length arrays are a C99 feature`。考试写 `int n; std::cin >> n; int a[n];` 属于错。

**为什么数组不能拷贝、赋值、比较。** 数组是聚合（aggregate），没有构造函数也没有赋值运算符；标准直接规定数组类型的左值不能出现在赋值的左边，函数也不能返回数组。`a == b` 能编译，但两边都退化成指针，比较的是**地址**，几乎永远为假。要"能拷贝、能比较、能返回"，就换 `std::array`。

```cpp
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>

int g[5];                     // 静态存储期：保证零初始化

int main() {
    int b[5] = {1, 2, 3};     // 后两个补 0
    int c[5] = {};            // 全部 0
    int d[] = {4, 5, 6, 7};   // 长度由初始化项推出
    char s[] = "hi";          // 'h' 'i' '\0'
    std::array<int, 3> e{7, 8, 9};

    std::cout << "b:";
    for (int x : b) std::cout << ' ' << x;
    std::cout << '\n';

    std::cout << "c:";
    for (int x : c) std::cout << ' ' << x;
    std::cout << '\n';

    std::cout << "g:";
    for (int x : g) std::cout << ' ' << x;
    std::cout << '\n';

    std::cout << "e:";
    for (int x : e) std::cout << ' ' << x;
    std::cout << '\n';

    std::cout << "size d=" << std::size(d) << " sizeof d=" << sizeof d << '\n';
    std::cout << "sizeof s=" << sizeof s << '\n';
}
```

输出：
```text
b: 1 2 3 0 0
c: 0 0 0 0 0
g: 0 0 0 0 0
e: 7 8 9
size d=4 sizeof d=16
sizeof s=3
```

`b` 和 `c` 只差一个 `{}`，结果一个留了三个有效值、一个全零；`g` 是全局数组，什么也没写却全是 0——对比 `b` 所在的行，就能看出"静态存储期"和"自动存储期"的区别。`sizeof s` 是 3 而不是 2，因为字符串字面量初始化字符数组时会**连结尾的 `'\0'` 一起拷**。

### 6.2 sizeof、std::size 与数组名退化

**是什么。** 数组名在大多数表达式里会转换成"指向首元素的指针"，这叫数组到指针的转换（array-to-pointer conversion），俗称**退化**（decay）。转换后的类型是 `T*`，长度信息彻底消失。

**哪些场合不退化**（这是判断"会不会丢长度"的清单）：`sizeof`、`&`（取数组的地址）、`decltype`、绑定到数组引用（`T (&)[N]`）、以及依赖 `T(&)[N]` 的模板推导。`std::size` 之所以能对 C 数组工作，正是因为它的签名是 `template <class T, std::size_t N> std::size_t size(T (&)[N])`——用引用接住数组，N 就推导出来了。

```text
main 里：a 的类型是 int[5]，整块 20 字节

     +-----+-----+-----+-----+-----+
     |  1  |  2  |  3  |  0  |  0  |
     +-----+-----+-----+-----+-----+
     a     a+1   a+2   a+3   a+4        a+i 的字节偏移是 i*4
     sizeof a = 20        sizeof a[0] = 4

传进 void f(int* p) 之后：p 是一个指针对象（本机 8 字节）

     +-------------+
     |  &a[0]      | -----------------> 上面那块 20 字节的内存
     +-------------+
     sizeof p = 8         &p 是 p 自己的地址，和 a 无关
```

**为什么退化是灾难。** `sizeof a / sizeof a[0]` 在定义所在的作用域里得到 5，是正确的；一旦写进函数，`a` 已经是 `int*`，`sizeof` 得到 8（本机），除出来是 2，而且**不报错**。这类错误编译器不会提醒，运行时可能刚好不越界，也可能读到垃圾——是最难查的一类。

**`&a` 与 `a` 的区别。** 它们数值相同（都指向那块内存的起点），但类型不同：`a` 退化成 `int*`，步长 4 字节；`&a` 的类型是 `int (*)[5]`，步长 20 字节，`&a + 1` 直接跳过整个数组。这是"指针类型决定步长"最干净的演示，也是理解 6.7 节 `int (*)[4]` 的铺垫。

**one-past-end。** `a + N` 是合法的指针值（可以取、可以比较、可以和 `a` 相减），但**不能解引用**。标准容器和算法统一用半开区间 `[begin, end)`，就是靠这条规则：`end` 指向最后一个元素的下一个位置，循环条件写 `it != end`。

```cpp
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>

void showPtr(int* p) {
    // 形参已经是指针：sizeof 得到指针大小，与数组长度无关
    std::cout << "showPtr: sizeof(p)==sizeof(int*): " << (sizeof p == sizeof(int*)) << '\n';
}

template <std::size_t N>
void showRef(int (&a)[N]) {           // 绑定到数组引用，N 被推导出来
    std::cout << "showRef: N=" << N << " sizeof(a)=" << sizeof a << '\n';
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};
    std::cout << "sizeof a=" << sizeof a << '\n';
    std::cout << "count=" << sizeof a / sizeof a[0] << '\n';
    std::cout << "std::size=" << std::size(a) << '\n';

    showPtr(a);                       // 退化发生在这条调用上
    showRef(a);                       // 用引用接住，不退化

    int* p = a;
    std::cout << "p[4]=" << p[4] << '\n';

    int* last = a + 4;
    std::cout << "a+4 is &a[4]: " << (last == &a[4]) << '\n';

    // 只比较地址差值，输出的是步长，不依赖具体地址
    auto stepElem = reinterpret_cast<std::uintptr_t>(a + 1) - reinterpret_cast<std::uintptr_t>(a);
    auto stepWhole = reinterpret_cast<std::uintptr_t>(&a + 1) - reinterpret_cast<std::uintptr_t>(&a);
    std::cout << "a+1 step=" << stepElem << " bytes, &a+1 step=" << stepWhole << " bytes\n";
}
```

输出：
```text
sizeof a=20
count=5
std::size=5
showPtr: sizeof(p)==sizeof(int*): 1
showRef: N=5 sizeof(a)=20
p[4]=5
a+4 is &a[4]: 1
a+1 step=4 bytes, &a+1 step=20 bytes
```

关注最后一行：同一个数组，`a + 1` 走 4 字节，`&a + 1` 走 20 字节。区别不在值，在**类型**——`a + 1` 是 `int*`，`&a + 1` 是 `int (*)[5]`。考试里给 `&a[0] + 1`、`a + 1`、`&a + 1` 问地址，考的就是这个。

### 6.3 越界与未初始化

**是什么。** `a[i]` 的定义就是 `*(a + i)`，标准对 `i` 的范围没有任何运行期检查。`i < 0` 或 `i >= N` 时行为未定义（undefined behavior，UB）。

**UB 不是"一定崩溃"。** 它的准确含义是"标准对程序行为不作任何保证"：可能读到相邻变量、可能写坏栈帧里的返回地址、可能看起来完全正常，换个编译器、换个优化级别、甚至多打一行日志就变了。所以"我这样写过，能跑"不能证明代码正确。真正危险的是越界**写**：它可能改掉循环变量、改掉函数返回地址，导致程序在离错误很远的地方崩溃，回头看栈已经面目全非。

**为什么不做检查。** 零开销原则：检查需要长度信息（而数组退化后没有）、需要一个分支、需要一条异常路径，而 C++ 的设计目标是"你不用的东西不付出代价"。要检查的场景有替代品：`std::array::at` / `std::vector::at` 会抛 `std::out_of_range`（本讲义不演示异常），或者自己 `assert`。

**未初始化。** `int a[5];` 在函数内是默认初始化，元素是不确定值，**读取它是 UB**。注意它不等于"随机数"：不能拿它当随机种子、不能假设它等于 0、也不能通过打印来"看看是什么"。C++26 起这类读取被明确为 erroneous behavior，工具链（如 MSan）会直接报错。

❌ **反例四：越界与未初始化。**

```cpp-fragment
int a[5];
std::cout << a[0];                    // ❌ 读未初始化值（UB）

int b[3] = {1, 2, 3};
b[3] = 4;                             // ❌ 越界写（UB）

int sum = 0;
for (int i = 0; i <= 5; ++i) sum += a[i];   // ❌ 多走一格，i==5 越界

std::vector<int> v{1, 2, 3};
std::cout << v[3];                    // ❌ vector 的 [] 同样不检查
std::cout << v[v.size()];             // ❌ 同上
```

`i <= n` 是最常见的越界来源，尤其出现在"从 1 开始数"的题目里。防御写法只有一条：循环条件统一用 `i < n`，`n` 取 `std::size(a)` 或 `v.size()`；需要"最后一个元素"就写 `a[n-1]`，不要靠 `<=` 去够。

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{4, 8, 15, 16, 23, 42};

    // 写法一：下标循环，条件是 < size()，不是 <=
    long long sum = 0;
    for (std::size_t i = 0; i < v.size(); ++i) sum += v[i];
    std::cout << "sum=" << sum << '\n';

    // 写法二：范围 for，边界由编译器算，不可能写错
    int mx = v.front();
    for (int x : v) mx = std::max(mx, x);
    std::cout << "max=" << mx << '\n';

    // 写法三：at()，越界抛异常；正常路径与 [] 等价
    std::cout << "v.at(5)=" << v.at(5) << '\n';

    // 写法四：迭代器半开区间 [begin, end)，算法自己管边界
    std::cout << "count>10="
              << std::count_if(v.begin(), v.end(), [](int x) { return x > 10; }) << '\n';

    // one-past-end：只能比较/相减，不能解引用
    int* first = v.data();
    int* last = first + v.size();
    std::cout << "distance=" << (last - first) << '\n';
}
```

输出：
```text
sum=108
max=42
v.at(5)=42
count>10=4
distance=6
```

四种写法算的是同一批数据，但只有后三种在"改需求"时不容易越界。实践建议：能范围 for 就范围 for；需要下标时把上界写成 `std::size(a)` 而不是手写常数。

### 6.4 std::array 与 std::vector

**`std::array<T, N>` 是什么。** 一个聚合类型，实现上内部只有 `T[N]` 这一个成员（标准要求它是聚合、元素连续存放），所以布局和 `int[N]` 一样，`data()` 能拿到裸指针，`sizeof` 等于整块大小。它把 C 数组缺的三件事补上了：**值语义**（可拷贝、可赋值、可比较、可结构化绑定）、**不退化**（传参不丢长度）、**带检查的访问**（`at()`）。长度仍然是类型的一部分，仍然在栈上，仍然零开销——没有堆分配，没有额外指针成员。

**`std::vector<T>` 是什么。** 堆上的连续数组加三个指针（首元素、尾后、容量尾）。`size()` 是已构造元素个数，`capacity()` 是已分配容量。`push_back` 摊还 O(1)，但扩容时会重新分配一块更大的内存、把元素搬过去、释放旧块——**此前取得的所有指针、引用、迭代器全部失效**。`reserve(n)` 只改容量不动元素；`resize(n)` 改元素个数，新增的元素值初始化为 `T{}`。

```text
std::vector<int> v{1, 2, 3};  v.reserve(5);

栈上的 v 对象（3 个指针，24 字节）      堆上一块连续内存（capacity 个 int 的空间）
  +-----------+                     +-----+-----+-----+-----+-----+
  | begin     | ------------------> |  1  |  2  |  3  |  ?  |  ?  |
  | end       | ----------+         +-----+-----+-----+-----+-----+
  | cap_end   | ----+     |           ^begin            ^end   ^cap_end
  +-----------+     |     |           3 个已构造元素，后面是“已分配但未构造”的裸内存
                    +-----+
```

`?` 那块内存**不能读**：它已经分配，但没有对象。`resize` 才会在那里构造元素；`push_back` 会在 `end` 处构造。

**长度与类型。** `size()` 返回无符号的 `std::size_t`，和 `int` 混用在 `-Wextra` 下会报 `-Wsign-compare`。更隐蔽的是 `v.size() - 1`：`v` 为空时它不变成 -1，而是回绕成一个天文数字，于是 `i <= v.size() - 1` 这种写法在空表上直接越界。标准库统一用无符号下标，原因就在这里；遍历时要么用范围 for，要么把 `i` 声明成 `std::size_t`。

**边界。** `vector<bool>` 是特化版本，按位压缩，`&v[0]` 不成立、不保证连续，别拿它当普通数组用。`vector` 存基类对象会发生**对象切片**（slicing），多态要存指针或 `reference_wrapper`。`array<T, 0>` 合法但 `data()` 可能返回空指针，不要解引用。

❌ **反例五：扩容导致的悬垂。**

```cpp-fragment
std::vector<int> v{1, 2, 3};
int* p = v.data();
v.push_back(4);            // 可能扩容：重新分配 + 搬移 + 释放旧块
std::cout << *p;           // ❌ p 可能已经悬垂（UB）
```

标准不保证 `push_back` 一定扩容（容量够就不扩），所以这个 bug 具有"测试时好的、上线后崩"的特征。规则：只要容器可能增长，就不要长期保存指向元素的指针/引用/迭代器；需要稳定地址就存下标，或者用 `std::deque`/`std::list`（代价是失去连续性）。

```cpp
#include <array>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::array<int, 4> a{3, 1, 4, 1};
    std::array<int, 4> b = a;          // 值语义：整块拷贝，两个独立对象
    b[0] = 99;
    std::cout << "a[0]=" << a[0] << " b[0]=" << b[0] << '\n';
    std::cout << "a==b: " << (a == b) << '\n';

    auto [w, x, y, z] = a;             // 结构化绑定，按元素拆开
    std::cout << w << x << y << z << '\n';

    std::vector<int> v{1, 2, 3};
    std::cout << "size=" << v.size() << " cap>=3: " << (v.capacity() >= 3) << '\n';
    v.reserve(100);                    // 只改容量，元素不动
    std::cout << "cap>=100: " << (v.capacity() >= 100) << '\n';
    v.resize(5);                       // 改元素个数，新元素值初始化为 0
    std::cout << "size=" << v.size() << " back=" << v.back() << '\n';
    std::cout << "sum=" << std::accumulate(v.begin(), v.end(), 0) << '\n';
    std::cout << "contiguous: " << (v.data() + 1 == &v[1]) << '\n';
}
```

输出：
```text
a[0]=3 b[0]=99
a==b: 0
3141
size=3 cap>=3: 1
cap>=100: 1
size=5 back=0
sum=6
contiguous: 1
```

`b = a` 之后改 `b[0]`，`a[0]` 仍是 3——这就是"值语义"，C 数组给不了。`capacity()` 的具体数值是实现定义的（不同标准库增长策略不同），所以这里只输出 `>=` 的布尔值，不输出具体数字；考试如果问"`reserve(100)` 后 `capacity()` 是多少"，标准答案是"**至少** 100"。

### 6.5 二维数组：行主序与地址公式

**是什么。** `int a[3][4]` 的类型是 `int[3][4]`，读法是"a 是 3 个元素的数组，每个元素是 `int[4]`"。所以：

```text
a          : int[3][4]     整个 48 字节的块
a[r]       : int[4]        第 r 行，本身是一个数组（16 字节）
a[r][c]    : int           单个元素
sizeof a       = 48        sizeof a[0]    = 16        sizeof a[0][0] = 4
```

**内存布局。** 整块 3×4 个 `int` 连续排列，**按行主序**（row-major）：先放第 0 行，再放第 1 行……行内按列递增。标准保证数组元素之间没有填充、下标递增对应地址递增，所以 `sizeof a == 3 * 4 * sizeof(int)` 一定成立。

```text
int a[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};

          c=0      c=1      c=2      c=3
       +--------+--------+--------+--------+
 r=0   |   1    |   2    |   3    |   4    |   字节偏移 0..15
       +--------+--------+--------+--------+
 r=1   |   5    |   6    |   7    |   8    |   字节偏移 16..31
       +--------+--------+--------+--------+
 r=2   |   9    |  10    |  11    |  12    |   字节偏移 32..47
       +--------+--------+--------+--------+

线性视图（一整块 48 字节，没有间隙、没有指针）：

  [1][2][3][4][5][6][7][8][9][10][11][12]
   ^a[0][0]              ^a[1][1]        ^a[2][3]
   偏移 0                偏移 24         偏移 44
```

**地址公式。** 单位是元素：

```text
&a[r][c] == a[0] + (r * C + c)           // C 是列数
字节偏移  == (r * C + c) * sizeof(int)
```

验证：`a[1][2]` 的扁平下标是 `1*4 + 2 = 6`，字节偏移 24；`a[2][3]` 是 `2*4 + 3 = 11`，字节偏移 44，正好是最后一个元素。记住"**行号乘列数再加列号**"这一个公式，二维数组的地址计算题就都能算。

**三维。** `int b[D][R][C]` 的公式是

```text
&b[i][j][k] == b[0] + ((i * R + j) * C + k)
```

规律：最右边一维连续；往左每一维的步长等于它右边所有维度大小的乘积。`int b[2][3][4]` 里，`k` 步长 1 个元素，`j` 步长 4 个，`i` 步长 12 个。这也就是为什么"第一维可以省略、后面的维度不能省"——步长是从右往左累乘出来的。

**为什么是行主序。** 因为 C 的声明语法里"最内层"是最后一维：`a[r]` 必须是一个完整的 `int[4]`（要能整体取地址、整体 `sizeof`、绑定到 `int (&)[4]`），所以一行必须连续。Fortran 和 MATLAB 的数组语法是 `A(i,j)`、最左维变化最快，所以它们用列主序（column-major）。同一个矩阵按两种顺序存成线性字节，结果正好是转置关系——跨语言交换二进制矩阵时必须显式约定顺序，否则会出现"数字全对、行列颠倒"的经典 bug。

**为什么二维数组里没有指针。** `a[r]` 是编译器用 `r * sizeof(int[4])` 算出来的地址，不是从某张表里查出来的。所以 `int[3][4]` 这块内存里一个指针都没有，它**不可能**变成 `int**`——这是 6.7 节的核心。

```cpp
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>

int main() {
    int a[3][4] = {
        { 1,  2,  3,  4},
        { 5,  6,  7,  8},
        { 9, 10, 11, 12},
    };

    std::cout << "rows=" << std::size(a) << " cols=" << std::size(a[0]) << '\n';
    std::cout << "sizeof a=" << sizeof a << " sizeof a[0]=" << sizeof a[0] << '\n';

    // 只比较两个地址的差值再除以元素大小，输出是布局偏移，不依赖具体地址
    auto offset = [](const void* hi, const void* lo) {
        return (reinterpret_cast<std::uintptr_t>(hi) -
                reinterpret_cast<std::uintptr_t>(lo)) / sizeof(int);
    };

    std::cout << "flat(1,2)=" << offset(&a[1][2], &a[0][0]) << '\n';
    std::cout << "flat(2,3)=" << offset(&a[2][3], &a[0][0]) << '\n';
    std::cout << "formula(1,2)=" << 1 * 4 + 2 << '\n';
    std::cout << "formula(2,3)=" << 2 * 4 + 3 << '\n';
    std::cout << "a[1][2]=" << a[1][2] << '\n';
}
```

输出：
```text
rows=3 cols=4
sizeof a=48 sizeof a[0]=16
flat(1,2)=6
flat(2,3)=11
formula(1,2)=6
formula(2,3)=11
a[1][2]=7
```

前三行是"类型事实"，后三行验证公式：实测偏移和 `r * C + c` 完全一致。`std::size(a)` 得到 3（第一维），`std::size(a[0])` 得到 4（第二维）——注意 `a[0]` 在这里没有退化，因为 `std::size` 用引用接住了它。

三维可以把同一个公式套两层。`int b[2][3][4]` 的 `sizeof` 会随下标层数递减：整块 96 字节，`b[0]` 是 `int[3][4]` 占 48 字节，`b[0][0]` 是 `int[4]` 占 16 字节，`b[0][0][0]` 才是 4 字节。下面的例子用每个元素记录自己的 `(i,j,k)`，再和公式算出的扁平下标对照。

```cpp
#include <cstddef>
#include <cstdint>
#include <iostream>

int main() {
    int b[2][3][4];
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 4; ++k)
                b[i][j][k] = i * 100 + j * 10 + k;   // 让值本身带上坐标

    std::cout << "sizeof b=" << sizeof b
              << " sizeof b[0]=" << sizeof b[0]
              << " sizeof b[0][0]=" << sizeof b[0][0] << '\n';
    std::cout << "b[0][0][0]=" << b[0][0][0] << " b[1][2][3]=" << b[1][2][3] << '\n';

    auto offset = [](const void* hi, const void* lo) {
        return (reinterpret_cast<std::uintptr_t>(hi) -
                reinterpret_cast<std::uintptr_t>(lo)) / sizeof(int);
    };
    std::cout << "flat(1,2,3)=" << offset(&b[1][2][3], &b[0][0][0]) << '\n';
    std::cout << "formula(1,2,3)=" << (1 * 3 + 2) * 4 + 3 << '\n';
}
```

输出：
```text
sizeof b=96 sizeof b[0]=48 sizeof b[0][0]=16
b[0][0][0]=0 b[1][2][3]=123
flat(1,2,3)=23
formula(1,2,3)=23
```

`(1*3 + 2)*4 + 3 = 23`，说明"先算外层两维的扁平下标，再乘最内维长度加最内下标"这个递推是对的。反过来也成立：任何一维的下标都可以从扁平下标里用除法和取余还原，这就是"按下标算地址"和"按地址反推下标"互逆的原因。

### 6.6 按行遍历与按列遍历

**是什么。** 同一个二维数组可以按行优先访问 `(0,0)(0,1)...(0,C-1)(1,0)...`，也可以按列优先访问 `(0,0)(1,0)...(R-1,0)(0,1)...`。两者的加法次数、求和结果完全一样，但**内存访问序列**不同，性能可能差几倍。

**为什么。** CPU 取数据以缓存行（cache line）为单位，常见大小 64 字节，能装 16 个 `int`。按行遍历时地址连续，取一个缓存行能用满 16 次，硬件预取器还能提前把下一行拉进来；按列遍历时每走一步跳 `C * 4` 字节，`C` 一大就几乎每步都落在新的缓存行上，每次只用其中 1 个元素就丢掉——缓存利用率掉到 1/16。

**数量级。** 1000×1000 的 `int` 矩阵是 4 MB，远超 L2 缓存。按行遍历近似顺序扫描，按列遍历的缓存缺失次数约是按行的 16 倍。这不是"常数因子"层面的差异，在内存墙面前就是几倍的墙钟时间。3×4 这种小数组只有 48 字节，整个装在一两个缓存行里，两种顺序看不出区别——所以这个知识点必须用"规模放大"来解释，不能靠小例子演示。

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

两行输出就是同一块内存在两种下标顺序下的读取序列：第一行是"线性读过去"，第二行是"每隔 4 个跳一次"。求和相同说明算法等价，序列不同说明访问模式不同——性能差异就藏在这个序列里。

### 6.7 二维数组怎么传参

**核心问题：形参必须能描述"一行有多大"。** 因为 `m[r]` 要算成 `*(m + r)`，而 `m + r` 的步长是"一行"的字节数。第一维不需要知道（函数里不会对整个数组做算术，退化后它就是指针），第二维必须写死。

```text
int a[2][4] 的类型链条

a          : int (*)[4]    退化后：指向“4 个 int 的数组”的指针
a[r]       : int[4]        再退化：int*
a[r][c]    : int
```

```cpp-fragment
void f(int m[][4], int rows);   // 人话写法：第一维省略
void f(int (*m)[4], int rows);  // 真实类型：完全等价
void f(int m[2][4]);            // 第一维写 2 也会被忽略，同上
void f(int** m);                // ❌ int[2][4] 不能转成 int**
void f(int m[][]);              // ❌ 第二维不能省，编译错
```

**为什么 `int**` 不行。** `int**` 解引用两次：先得到一个 `int*`（一个指针对象），再得到 `int`。它假设内存里**先有一个指针数组**，每个指针指向一行。而 `int[2][4]` 里一个指针都没有，只有 8 个连续的 `int`。强转之后 `m[1]` 会把第 0 行的前两个 `int`（比如 `1` 和 `2`）当成一个地址来解释，然后去解引用它——几乎必然崩溃，属于 UB。

**`vector<vector<int>>` 是什么。** 外层 vector 的元素是 `vector<int>` 对象（每个 24 字节，三个指针），每个内层 vector 各自在堆上分配一块。所以行与行**不保证相邻**，甚至不保证按创建顺序排列；`v.data()` 的类型是 `vector<int>*`，不是 `int**`；它也不能传给 `int**` 形参。它的优点是每行长度可以不同（锯齿数组），缺点是缓存不友好、每行一次额外分配。

```text
int a[2][4]（一整块，32 字节）          vector<vector<int>>（指针 + 分散的堆块）

  +----+----+----+----+                 v: +--------+--------+
  | 1  | 2  | 3  | 4  |                    | ptr ---|------> [1][2][3][4]  堆块 A
  +----+----+----+----+                    | ptr ---|------> [5][6][7][8]  堆块 B
  | 5  | 6  | 7  | 8  |                    +--------+--------+
  +----+----+----+----+                    每个 vector<int> 占 24 字节；
  连续、无指针、行内行间都相邻              两块内存互不相邻，行长度可变
```

**三种方案的选择。** 要连续内存又不想手写 `new`，用一维 `std::vector<int> flat(R * C)` 加 `flat[r * C + c]`，传参只传一个引用；要 `v[i][j]` 的写法、且行长度不一，用 `vector<vector<int>>`；C++20 起还有 `std::span<int>`，它内部只有"指针 + 长度"两个字段，能接 C 数组、`std::array`、`vector`，传参代价两个机器字，是"既要保留长度、又不想绑定容器"的标准答案。

```cpp
#include <cstddef>
#include <iostream>
#include <vector>

// 第一维可以省，第二维必须写死：形参类型是 int (*)[4]
int sumRows(int (*m)[4], int rows) {
    int total = 0;
    for (int r = 0; r < rows; ++r) {
        int s = 0;
        for (int c = 0; c < 4; ++c) s += m[r][c];
        std::cout << "row " << r << " = " << s << '\n';
        total += s;
    }
    return total;
}

// 传数组引用：维度由模板推导，完全不退化
template <std::size_t R, std::size_t C>
int total2d(const int (&m)[R][C]) {
    int s = 0;
    for (std::size_t r = 0; r < R; ++r)
        for (std::size_t c = 0; c < C; ++c) s += m[r][c];
    return s;
}

// 扁平一维 + 手动下标：内存连续，缓存最友好
int sumFlat(const std::vector<int>& flat, int rows, int cols) {
    int s = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) s += flat[r * cols + c];
    return s;
}

// 锯齿数组：每行独立分配，行长度可以不同
int sumVecVec(const std::vector<std::vector<int>>& m) {
    int s = 0;
    for (const auto& row : m)
        for (int x : row) s += x;
    return s;
}

int main() {
    int a[2][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}};
    int byRows = sumRows(a, 2);          // 先算再打印：函数内部也会输出
    std::cout << "sumRows=" << byRows << '\n';
    std::cout << "total2d=" << total2d(a) << '\n';

    std::vector<int> flat{1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "sumFlat=" << sumFlat(flat, 2, 4) << '\n';

    std::vector<std::vector<int>> vv{{1, 2, 3, 4}, {5, 6, 7, 8}};
    std::cout << "sumVecVec=" << sumVecVec(vv) << '\n';
}
```

输出：
```text
row 0 = 10
row 1 = 26
sumRows=36
total2d=36
sumFlat=36
sumVecVec=36
```

四种写法结果一致，但语义差别很大：`sumRows` 只认"4 列"这一种布局；`total2d` 连行数都从类型里推出来，`a` 换了尺寸也不用改；`sumFlat` 拿到的是连续内存，和 C 接口兼容；`sumVecVec` 最灵活但每行一次独立分配，缓存最差。

### 6.8 选型与坑清单

| 需求 | 选择 | 理由 |
| --- | --- | --- |
| 长度编译期固定，要对接 C 接口 | `int a[N]` | 零开销，布局最直接 |
| 长度编译期固定，要值语义（拷贝、比较、返回） | `std::array<int, N>` | 不退化，有 `size()`/`at()` |
| 长度运行期才知道，还要增长 | `std::vector<int>` | 连续存储，内存自动管理 |
| 二维且要连续内存 | `std::vector<int> flat(R * C)` | 缓存友好，传参只传一个 |
| 二维且要 `v[i][j]` 写法、行长可变 | `vector<vector<int>>` | 每行独立分配，缓存不友好 |

| 常见写法 | 后果 |
| --- | --- |
| 函数内 `sizeof(arr)` 求长度 | 得到指针大小，静默错误 |
| `void f(int** m)` 接收 `int a[2][4]` | 编译错；强转则 UB |
| `char s[2] = "hi";` | 编译错，放不下 `'\0'` |
| `v.push_back(x)` 后继续用旧的 `&v[0]` | 悬垂指针，UB |
| `for (int i = 0; i <= n; ++i)` | 越界，UB |
| `int a[n];`（n 是变量） | 非标准 VLA，clang 给扩展警告 |
| `a = b;`（两个 C 数组） | 编译错，数组不可赋值 |
| 局部 `int a[5];` 直接读 | 未初始化值，UB |

**考试视角。** 给一个二维数组声明，要你说出 `sizeof`、某元素的地址偏移、传参的形参类型，或者问"哪种遍历更快"——都是本节内容的直接应用。判分点通常只有三个：长度是不是类型的一部分、传参时退化成了什么、访问有没有越界。

一句话：**能用 `vector`/`array` 就不要手写 C 数组**，只有对接 C 接口或需要精确控制布局时才用裸数组；无论用哪一种，长度都得自己管住——数组不会替你记，编译器也不会替你查。
