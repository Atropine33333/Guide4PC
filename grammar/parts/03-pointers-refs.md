## 7. 指针

指针是"存放地址的变量"。这句话里其实有两个对象：被指向的那块内存，和指针自己这块内存。指针自己的值是一个地址；这个地址指向谁、那个谁还活着没有，编译器基本不管。C++ 对指针的检查只到"类型对不对"这一层——它不检查地址是否有效、是否越界、对象是否已经销毁。所以学指针的正确姿势不是背语法，而是随时能画出"内存里有什么、谁指向谁、谁还活着"。

### 7.1 地址、对象与解引用

先建立一张图。图中地址只是示意，程序输出永远不会打印真实地址。

```text
栈上的两个对象（地址仅示意）
        0x1000              0x2000
        +--------+          +--------+
    x   |   42   |      p   | 0x1000 |
        +--------+          +--------+
                                |
                                +--------> x

  &x  == 0x1000，类型 int*
  p   的值就是 0x1000
  *p  顺着 p 里的地址找回 x 这块内存，得到 x 本身
```

三个结论直接从图里读出来：

1. `&x` 的类型是 `int*`。取地址不复制对象，只是把这块内存的编号写下来。
2. `*p` 的结果是 `x` 这个对象本身（前缀 `*` 读作解引用，dereference），所以它是**左值**（lvalue）：`*p = 7` 与 `x = 7` 完全等价，`++*p` 就是 `++x`。
3. `p` 和 `x` 是两块互不相干的内存。`p = &y` 改的是 `p`（让它指向别处），`x` 一动不动。

`&` 在 C++ 里有三个身份，看位置区分：跟在类型名后面（`int& r`）是**引用声明符**；出现在两个操作数中间（`a & b`）是**按位与**；作为一元前缀运算符（`&x`）是**取地址**。考题里最常见的混淆是 `int* p = &x;` 中的两个符号各是什么——`*` 属于类型，`&` 是取地址。

还有一个纯语法坑：`int* a, b;` 中 `*` 只修饰 `a`，`b` 是 `int`。这正是很多人主张写 `int *a` 或一行只声明一个变量的原因。

那为什么非要有指针？至少三个理由，而且引用一个都替代不了：

1. **共享而不是复制**。函数拿到指针，操作的就是调用者那块内存；对象可能是几百 MB 的缓冲区，复制不可接受。
2. **可空**。"这次没有结果"必须能表达出来——返回 `nullptr` 可以，返回"空引用"不存在。
3. **指向谁可以运行期决定**。链表节点指向下一个节点、基类指针指向派生类对象，这些关系在编译期根本不知道。指针是一个可以在运行期被改写的值，引用一旦绑定就固定了。

反过来说，如果一个需求三者都不需要——只是给已有对象起个名字、避免拷贝——那引用更合适，因为它不会为空、不会指错、不需要解引用。第 8 节会把这条边界画清楚。

```cpp
#include <iostream>

int main() {
    int x = 42;
    int* p = &x;              // p 存的是 x 的地址
    std::cout << *p << '\n';  // 解引用：拿回 x 本身

    *p = 7;                   // *p 是左值，改的是 x
    std::cout << x << '\n';

    int y = 100;
    p = &y;                   // 改的是 p（指向谁），不是 *p
    std::cout << *p << ' ' << x << ' ' << y << '\n';

    std::cout << (&x == p) << ' ' << (p == &y) << ' ' << (&x == &y) << '\n';
    std::cout << (sizeof(p) == sizeof(void*)) << '\n';
}
```

输出：
```text
42
7
100 7 100
0 1 0
1
```

第三行值得多看一眼：`*p` 是 100，`x` 是 7。`p = &y` 之后，`p` 与 `x` 已经没有任何关系了，`&x == p` 为假。指针的赋值只改"指向"，不搬运数据——这也是它与"引用赋值"最容易搞混的地方（第 8 节会看到 `r = y` 改的是值）。

顺带记住 `p->m` 就是 `(*p).m` 的缩写。`->` 左边的类型必须是指针（或重载了 `operator->` 的类），`.` 左边必须是对象或引用。

### 7.2 空指针、野指针与悬垂指针

一个指针变量只有三种状态，混淆它们是一切指针 bug 的源头：

| 状态 | 含义 | 怎么来的 | 解引用的后果 |
| --- | --- | --- | --- |
| 有效 | 指向一个还活着的对象 | `int* p = &x;` | 正常 |
| 空 | 明确不指向任何对象 | `int* p = nullptr;` | UB（通常段错误） |
| 野（wild） | 值不确定，指向随机位置 | `int* p;` 未初始化 | UB，可能"看似正常" |
| 悬垂（dangling） | 曾经有效，对象已销毁 | `delete p` 之后、容器扩容之后、函数返回局部变量地址 | UB，读到已归还的内存 |

"野"和"悬垂"的区别只在历史：野指针从来没指向过合法对象，悬垂指针曾经合法。对程序来说两者一样危险。

`nullptr` 是 C++11 引入的空指针字面量，类型是 `std::nullptr_t`。它的两个好处：能隐式转换成任意指针类型；**不能**转换成整数（`int n = nullptr;` 编译错）。这解决了 `NULL` 的老问题——`NULL` 在多数实现里就是 `0`，于是 `f(NULL)` 会选中 `f(int)` 而不是 `f(int*)`，静默调错函数。判断口诀：**看到 `0` 或 `NULL` 当指针用，就换成 `nullptr`**。

三条实用规则：

- 定义时就初始化。`int* p = nullptr;` 让"空"这个状态可被检查，而未初始化的指针连检查都没有意义。
- 释放后立刻置空。`delete p; p = nullptr;` 让二次释放变成合法的空操作（`delete nullptr` 是标准保证的 no-op，析构函数里无条件 `delete` 成员正是靠这条）。
- `if (p)` 判断的是"指针是否为空"，**不是**"它指向的值是否为真"：`int zero = 0; if (&zero)` 为真。

```cpp
#include <iostream>

// 在 [first, last) 里找 target；找不到返回 nullptr
int* find(int* first, int* last, int target) {
    for (; first != last; ++first)
        if (*first == target) return first;
    return nullptr;
}

int main() {
    int a[4] = {3, 1, 4, 1};

    int* hit = find(a, a + 4, 4);
    if (hit)                                   // if (p) 判断“是否为空”，不是“指向的值是否为真”
        std::cout << "found " << *hit << " at " << (hit - a) << '\n';

    int* miss = find(a, a + 4, 9);
    std::cout << (miss == nullptr) << ' ' << (hit != nullptr) << '\n';

    int* empty = nullptr;
    delete empty;                              // 对空指针 delete 是合法的空操作
    std::cout << (empty == nullptr) << '\n';
}
```

输出：
```text
found 4 at 2
1 1
1
```

注意 `find` 的返回值设计：找不到时返回 `nullptr`，调用者必须自己检查。这是 C 风格接口的典型契约，也是"指针可以表示'没有'"这一能力的来源——引用做不到（8.6 节对比）。

### 7.3 指针算术：类型决定步长

指针算术的定义只有一句：`p + n` 的字节偏移是 `n * sizeof(*p)`。所以类型不是装饰，它决定了"一步走多远"：

| 表达式 | 类型 | 前进的字节数 |
| --- | --- | --- |
| `p + 1` | `int*` | `sizeof(int)`，通常 4 |
| `p + 1` | `char*` | 1 |
| `p + 1` | `double*` | `sizeof(double)`，通常 8 |
| `pa + 1` | `int (*)[5]` | 整个数组，`5 * sizeof(int)` |

标准只对**同一个数组**（含末尾的下一个位置 one-past-end）内的算术给出定义。两个不同数组的指针相减是 UB；用 `<` 比较两个不同数组的指针，结果**未指定**（编译器可以任选一个方向，只要保证二者恰有一个为真）；只有 `==`/`!=` 是良定义的——不同对象的指针必然不等。这些操作在实际机器上只是两个整数比大小，看起来"能跑"，但标准不保证。`p - q` 的结果类型是 `std::ptrdiff_t`（有符号），单位是元素个数而不是字节。

`p[i]` 语言上就定义为 `*(p + i)`，所以 `p[-1]` 是合法的（前提是 `p` 前面确实还有元素），`i[p]` 也是合法的（加法可交换）。下标运算符不做任何边界检查，越界不报错，只是 UB。

`void*` 是"无类型指针"：任何对象指针都能隐式转成 `void*`，转回来要用 `static_cast`。它不能解引用（不知道按几个字节解释），也不能做算术（标准 C++ 不允许，GCC 的"按字节走"是扩展）。

```cpp
#include <iostream>

int main() {
    int a[5] = {10, 20, 30, 40, 50};
    int* p = a;                        // 数组名退化成 &a[0]
    std::cout << *p << ' ' << *(p + 2) << ' ' << p[3] << '\n';

    p += 2;                            // 按“元素”步进，不是按字节
    std::cout << *p << ' ' << p[-1] << '\n';
    std::cout << (p - a) << '\n';      // 单位是元素个数，类型 ptrdiff_t

    const char* base = reinterpret_cast<const char*>(a);
    std::cout << (reinterpret_cast<const char*>(a + 1) - base) << ' '
              << (reinterpret_cast<const char*>(&a + 1) - base) << '\n';
}
```

输出：
```text
10 30 40
30 20
2
4 20
```

最后一行是本节最值钱的一行：`a + 1` 走 4 字节，`&a + 1` 走 20 字节。原因不在算术规则，而在类型——`a` 在表达式里退化成 `int*`，`&a` 的类型是 `int (*)[5]`（指向整个数组的指针）。同一个地址，两种类型，两种步长。

### 7.4 const 与指针的三种组合

`const` 修饰指针时有两个独立的位置，能锁住的东西不同：

| 写法 | 能否改 `*p` | 能否改 `p` | 术语 | 读法 |
| --- | --- | --- | --- | --- |
| `const int* p` | 否 | 能 | 底层 const（low-level） | 指向 const int 的指针 |
| `int* const p` | 能 | 否 | 顶层 const（top-level） | const 指针，指向 int |
| `const int* const p` | 否 | 否 | 两者都有 | const 指针，指向 const int |

读法用"从变量名往外、从右往左"最可靠：`const int* const p` 读作"p 是一个 const 指针，指向 const int"。

两个高频考点：

- **顶层 const 作形参会被忽略**。`void f(int* const p)` 与 `void f(int* p)` 是**同一个函数**，不能构成重载；同理 `void f(const int)` 与 `void f(int)` 也无法重载。而底层 const 会参与类型：`f(int*)` 与 `f(const int*)` 是两个不同的重载。
- **底层 const 只是访问限制，不是"对象是常量"**。`const int* pc = &x;` 之后，别人仍然可以 `x = 7`，你通过 `pc` 读到的值会跟着变。`const` 约束的是"通过这个名字能做什么"，不是对象本身。
- 转换方向是单向的：`int*` 可以隐式转成 `const int*`（增加限制，安全）；反过来必须 `const_cast`（去掉承诺，危险）。`const_cast` 只有在对象本身确实不是 const 时才安全，否则写它是 UB。

```cpp
#include <type_traits>
#include <iostream>

int main() {
    int x = 1;
    int y = 2;

    const int* pc = &x;      // 底层 const：不能经 pc 写 *pc
    pc = &y;                 // 但可以改指向
    std::cout << *pc << '\n';

    int* const cp = &x;      // 顶层 const：不能改指向
    *cp = 10;                // 但可以改值
    std::cout << x << '\n';

    const int* const cpc = &y;   // 两者都锁死
    std::cout << *cpc << ' ' << (cpc == &y) << '\n';

    x = 7;                   // const 只是“访问限制”，对象本身不是常量
    std::cout << *cp << ' ' << x << '\n';

    std::cout << (std::is_same_v<const int*, int const*>) << ' '
              << (std::is_same_v<int* const, const int*>) << '\n';
}
```

输出：
```text
2
10
2 1
7 7
1 0
```

最后一行输出 `1 0` 说明：`const int*` 与 `int const*` 是同一个类型（`const` 修饰的是 `int`），而 `int* const` 是另一个类型（`const` 修饰的是指针）。`const` 的位置换了，锁的对象就换了。

字符串字面量是这条规则的经典受害者：`"abc"` 的类型是 `const char[4]`，放在只读段。C++11 起 `char* p = "abc";` 是 ill-formed（C++03 里是已弃用但仍允许），必须写 `const char* p = "abc";`；硬转后写入会直接段错误。要改内容就用 `std::string`。

### 7.5 二级指针

`T**` 是"指向 `T*` 的指针"，也就是"指针变量的地址"。

```text
        +---------+        +---------+        +---------+
   x    |    1    |   p    |  &y     |   pp   |  &p     |
        +---------+        +---------+        +---------+
                               |                 |
                               |                 +--> p 这块内存
                               +--------------------> y
   *pp  == p        （一级指针本身，类型 int*）
   **pp == *p == y  （最终的对象，类型 int）
   *pp = &x;        →  p 改成指向 x，y 不变
```

它出现的场合只有两类，但都很重要：

1. **输出参数**。C++ 按值传参，函数想改调用者的 `int*`，就得拿到这个指针的地址，也就是 `int**`（或者用 `int*&`，见 8.3）。"在数组里找到目标就让指针指过去"这类接口都是这个形状。
2. **动态二维数组**。`int** g = new int*[rows];` 先分配"行指针数组"，再对每行 `new int[cols]`。行与行各自独立分配，地址互不相邻，所以不能用 `g[0][cols]` 越到下一行——那是 UB。释放顺序必须与分配相反：先逐行 `delete[] g[r]`，最后 `delete[] g`；反过来会先丢掉行指针，直接泄漏。

`char** argv` 是同一形状的另一个例子：`argv` 指向一个指针数组，每个元素指向一个以 `'\0'` 结尾的字符串。

❌ `void f(int** m)` 接收 `int a[2][3]`：类型不匹配，编译错（`a` 退化成 `int (*)[3]`）。强转后按 `m[i][j]` 访问是 UB，因为二维数组是一整块连续内存，里面根本没有"指针数组"。

```cpp
#include <iostream>

int** makeGrid(int rows, int cols) {
    int** g = new int*[rows];                    // 行指针数组
    for (int r = 0; r < rows; ++r)
        g[r] = new int[cols]{};                  // 每行各自分配，行与行不相邻
    return g;
}

void freeGrid(int** g, int rows) {
    for (int r = 0; r < rows; ++r) delete[] g[r];  // 顺序与分配相反
    delete[] g;
}

int main() {
    int x = 1, y = 2;
    int* chosen = &x;
    int** pp = &chosen;          // 二级指针：指针变量的地址
    *pp = &y;                    // 改写的是 chosen 本身
    std::cout << *chosen << ' ' << (&chosen == pp) << '\n';

    int** g = makeGrid(2, 3);
    for (int r = 0; r < 2; ++r)
        for (int c = 0; c < 3; ++c) g[r][c] = r * 3 + c;
    std::cout << g[0][0] << ' ' << g[1][2] << ' ' << (g[0] + 1 != g[1]) << '\n';
    freeGrid(g, 2);

    int* slot = nullptr;
    int** out = &slot;           // 输出参数：让调用者的指针指向新内存
    *out = new int(5);
    std::cout << *slot << '\n';
    delete slot;
}
```

输出：
```text
2 1
0 5 1
5
```

`*pp = &y` 这一行是二级指针的全部魔力：它改的是 `chosen` 这个变量，而不是 `chosen` 指向的值。想清楚"我要改的是指针，还是指针指向的东西"，就永远不会在 `*` 的层数上犯错：**要改指针本身，就多一层**。

### 7.6 函数指针

函数指针存的是代码段里的地址。它的声明要加括号：

```text
int (*fp)(int, int);   // fp 是指针，指向“两个 int 进、一个 int 出”的函数
int*  fp(int, int);    // 完全不同的东西：返回 int* 的函数
```

括号不能省，因为 `()` 的优先级高于 `*`。读法仍然是"从名字往外"：`fp` 先与 `*` 结合，说明它是指针；再看右边 `(int,int)`，说明指向函数；最后看左边 `int`，说明返回 `int`。

几个必须知道的事实：

- 函数名在表达式里自动退化成函数指针，所以 `add`、`&add`、`*add` 三者等价，`fp(1,2)` 与 `(*fp)(1,2)` 也等价。
- 函数指针可以存进数组做**跳转表**，可以比较、可以传参，是零开销的回调机制。
- 有捕获的 lambda **不能**转成函数指针：它带着状态，没有"一个普通函数的地址"这种东西。无捕获 lambda 可以，因为它就是一个匿名函数。
- 函数指针的大小与对象指针**不一定相同**，标准不保证；别把函数指针塞进 `void*`。

```cpp
#include <iostream>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }

using BinOp = int (*)(int, int);          // 函数指针类型别名

int apply(BinOp op, int a, int b) { return op(a, b); }

int main() {
    int (*fp)(int, int) = add;            // 函数名自动转成函数指针
    std::cout << fp(7, 2) << ' ' << (*fp)(7, 2) << ' ' << (&add == fp) << '\n';

    BinOp table[3] = {add, sub, mul};     // 跳转表
    const char* names[3] = {"add", "sub", "mul"};
    for (int i = 0; i < 3; ++i)
        std::cout << names[i] << " " << apply(table[i], 7, 2) << '\n';

    BinOp mod = [](int a, int b) { return a % b; };   // 无捕获 lambda 可转函数指针
    std::cout << mod(7, 4) << '\n';
}
```

输出：
```text
9 9 1
add 9
sub 5
mul 14
3
```

`using BinOp = int (*)(int, int);` 这类别名在实际代码里几乎必写——裸写函数指针类型，读起来和声明变量没区别。

### 7.7 new / delete 与 new[] / delete[]

`new` 做两件事：向堆申请内存，然后在这块内存上构造对象；`delete` 反过来：先析构，再归还内存。所以它和 `malloc`/`free` 不是一回事——后者只管字节，不管构造与析构。

| 分配 | 释放 | 构造/析构 |
| --- | --- | --- |
| `new T` | `delete p` | 一次 |
| `new T(args)` | `delete p` | 一次，带实参 |
| `new T[n]` | `delete[] p` | n 次默认构造 |
| `new T[n]{...}` | `delete[] p` | n 次，列表外的元素值初始化 |

必须严格配对。实现上，`new T[n]` 往往在返回地址前面多存一个字（cookie）记录元素个数，`delete[]` 正是靠它才知道要析构几个对象、归还多少字节。`delete` 没有这个信息，只会析构一个对象、归还错误的大小——这就是错配 UB 的物理原因。标准并不要求实现真的存 cookie，所以"错配在我机器上没事"完全不可依赖。

其余规则：

- `new` 失败抛 `std::bad_alloc`；`new (std::nothrow) T` 失败返回 `nullptr`。默认的 `new` 不会返回空指针，所以"检查 new 的返回值"在标准形式下是多余代码。
- `delete p` 之后 `p` 的值不变，它变成悬垂指针；再次 `delete` 是二次释放，UB。
- `delete nullptr` 与 `delete[] nullptr` 都是合法的空操作，这是析构函数里无条件释放成员的安全依据。
- 忘了 `delete` 就是内存泄漏。泄漏不会立刻报错，只会让进程的内存曲线一路上涨。

现代 C++ 的答案是 RAII：`std::vector`、`std::string`、`std::unique_ptr` 在析构函数里自动释放，异常路径也不会漏。本讲义不展开智能指针，但结论要记住：**手写 new/delete 的代码，绝大多数都能被容器替代**。

```cpp
#include <iostream>
#include <utility>

struct Tracked {
    int id;
    explicit Tracked(int i) : id(i) { std::cout << "ctor " << id << '\n'; }
    Tracked(const Tracked& o) : id(o.id) { std::cout << "copy " << id << '\n'; }
    ~Tracked() { std::cout << "dtor " << id << '\n'; }
};

int main() {
    int* one = new int(42);
    std::cout << *one << '\n';
    delete one;                        // new 配 delete

    int* arr = new int[4]{1, 2, 3};    // 第 4 个元素被值初始化为 0
    int sum = 0;
    for (int i = 0; i < 4; ++i) sum += arr[i];
    std::cout << sum << '\n';
    delete[] arr;                      // new[] 配 delete[]

    Tracked* t = new Tracked[2]{Tracked(1), Tracked(2)};
    std::cout << "scope end" << '\n';
    delete[] t;                        // 逐个析构，顺序与构造相反
}
```

输出：
```text
42
6
ctor 1
ctor 2
scope end
dtor 2
dtor 1
```

`new Tracked[2]{Tracked(1), Tracked(2)}` 没有打印 `copy`，因为 C++17 起用纯右值初始化数组元素保证省略拷贝（guaranteed copy elision）。析构顺序与构造相反：先 `dtor 2` 再 `dtor 1`，这一点对"后者依赖前者"的资源很重要。

### 7.8 指针与数组：七个维度

关系只有一句：数组名在大多数表达式里退化成首元素地址，所以 `a[i] == *(a + i) == *(i + a) == i[a]`。区别则要一条一条列：

| 维度 | 数组 `int a[4]` | 指针 `int* p` |
| --- | --- | --- |
| `sizeof` | 整块，`4 * sizeof(int)` | 一个地址的大小 |
| 能否赋值 | 不能，数组名不是可修改左值 | 能，可以改指向 |
| `&` 得到的类型 | `int (*)[4]`，指向整个数组 | `int**`，指向指针变量 |
| 是否有独立存储 | 没有，`a` 就是那块内存 | 有，单独一块存地址 |
| `+1` 的效果 | 先退化成指针，再走一个元素 | 走一个元素 |
| 能否用 `sizeof` 求长度 | 能（在同一作用域里未退化时） | 不能 |
| 作形参时 | 退化成 `int*`，长度丢失 | 本身就是指针 |

第一行和第三行是考试重点。"数组名就是指针"这句话是错的：`sizeof` 和 `&` 都能立刻证明它们不是一回事。数组名只是一个"能退化成指针的标识符"，它自己没有存储、不能赋值。

退化在形参上最坑人：`void f(int a[10])` 里的 `10` 会被编译器**完全忽略**，形参 `a` 的真实类型是 `int*`，函数体里 `sizeof(a)` 得到的是一个地址的大小。想在函数里知道长度，只能额外传一个参数（或用 `std::span`/容器的 `size()`）。这条规则来自 C：数组按值传递意味着复制整块内存，语言干脆规定数组不能按值传，一律传首地址。

```cpp
#include <iostream>
#include <type_traits>

int main() {
    int a[4] = {1, 2, 3, 4};
    int* p = a;

    std::cout << (sizeof a / sizeof a[0]) << ' '            // 4：数组名未退化
              << (sizeof a == 4 * sizeof(int)) << '\n';
    std::cout << (sizeof p == sizeof(void*)) << '\n';       // 1：指针只存一个地址

    std::cout << (a == &a[0]) << ' '
              << (reinterpret_cast<void*>(&a) == reinterpret_cast<void*>(a)) << '\n';

    std::cout << (std::is_same_v<decltype(&a), int (*)[4]>) << ' '
              << (std::is_same_v<decltype(&p), int**>) << '\n';

    int (*pa)[4] = &a;                       // 指向“含 4 个 int 的数组”
    std::cout << (*pa)[2] << ' ' << sizeof *pa / sizeof(int) << '\n';

    int* q = a + 4;                          // one-past-end：可以比较，不能解引用
    std::cout << (q - a) << ' ' << (q == a + 4) << '\n';
}
```

输出：
```text
4 1
1
1 1
1 1
3 4
4 1
```

最后两行演示 one-past-end：`a + 4` 是合法的指针值（可以比较、可以相减），但**不能解引用**。标准库的迭代器就是把这个约定包装成了 `[begin, end)` 半开区间，`end()` 永远指向最后一个元素的下一个位置——这解释了为什么 `end()` 不能解引用，也解释了为什么遍历用 `!=` 而不是 `<`。

### 7.9 七种常见未定义行为

下面每一段都是"能编译、可能还能跑"的代码，但它们都是 UB。UB 的可怕之处不是必崩，而是**有时正常**：换编译器、换优化等级、加一行打印，行为就变了。

❌ 野指针：值不确定，写它等于往随机地址写。

```cpp-fragment
int* p;              // 未初始化：值是不确定的内存残留
*p = 1;              // UB：可能段错误，也可能悄悄改坏别的变量
if (p) { }           // 连“判断是否为空”都没有意义
```

❌ 释放后使用（use-after-free）与二次释放。

```cpp-fragment
int* q = new int(5);
delete q;
std::cout << *q;     // UB：内存已归还，可能被重新分配给别人
delete q;            // UB：二次释放，堆结构被破坏
```

✅ 正确做法是 `delete q; q = nullptr;`。置空不能救回已经写出去的悬垂副本，但至少能挡住"同一个指针再删一次"。

❌ `new[]` 配 `delete`（或反过来）。

```cpp-fragment
int* a = new int[10];
delete a;            // UB：只析构一个元素，且按单对象的大小归还内存
int* b = new int(1);
delete[] b;          // UB：delete[] 会去读不存在的 cookie
```

❌ 返回局部变量的地址。

```cpp-fragment
int* bad() {
    int x = 1;
    return &x;       // UB：函数返回后栈帧已销毁，地址还在但对象没了
}
```

✅ 返回值、返回静态/全局对象的地址、或让调用者传入缓冲区。

❌ 越界与解引用 one-past-end。

```cpp-fragment
int a[3] = {1, 2, 3};
int* end = a + 3;    // 合法：one-past-end，可以比较
std::cout << *end;   // UB：这个位置没有对象
a[3] = 4;            // UB：越界写，可能踩坏相邻变量
```

❌ 不同数组的指针相减，或用 `<` 比较它们。

```cpp-fragment
int x[2] = {1, 2}, y[2] = {3, 4};
std::ptrdiff_t d = &x[0] - &y[0];   // UB：两个数组之间没有“距离”这个概念
bool b = &x[1] < &y[0];             // 结果未指定：谁大谁小由实现决定
```

❌ `void*` 上做算术，或把 `void*` 直接解引用。

```cpp-fragment
void* v = &x;
v + 1;               // 编译错：void 的大小未知
*v;                  // 编译错：不知道按什么类型解释
char* c = static_cast<char*>(v) + 1;   // ✅ 要按字节走，先转 char*
```

调试建议：用 `-fsanitize=address` 编译，ASan 能抓住大部分 use-after-free、越界和二次释放，并直接打印出错地址、分配点和释放点。它比"程序偶尔算错一个数"好定位得多，考试之外写代码时值得默认打开。

### 7.10 考试视角与坑清单

指针题的判断口诀按顺序问三句：**它指向的对象还活着吗？类型对吗？越界了吗？** 三句都过才是合法解引用。

| 写法 | 后果 |
| --- | --- |
| `int* p; *p = 1;` | 野指针解引用，UB |
| `delete p` 后继续用或再删 | 悬垂/二次释放，UB |
| `new[]` 配 `delete` | UB，堆损坏或只析构一个元素 |
| `int* a, b;` 以为两个都是指针 | `b` 是 `int`，静默错误 |
| `p + 1` 以为前进 1 字节 | 实际走 `sizeof(*p)` 字节 |
| 用 `NULL` 调指针重载 | 选中 `int` 版本，静默调错 |
| `char* p = "abc";` | ill-formed（C++11 起），写入则崩 |
| `int a[4]; int* p = a; sizeof(p)/sizeof(int)` | 得到 1 或 2，不是 4 |
| 返回局部变量的地址 | 悬垂，UB |

## 8. 引用

引用（reference）是给一个**已经存在的对象**起的另一个名字。它不是一个新对象，不复制数据，也不额外占一块"存地址"的内存。语言给它的两条硬约束——必须初始化、绑定后不能改绑——正是"别名"这个身份的必然结果：名字不能没有主人，也不能中途换主人。

引用的价值在于它同时具备两种气质：**写法像值**（不用 `*`、不用 `&`、和普通变量一模一样），**语义是别名**（不拷贝、能改本体）。C++ 的函数传参、运算符重载、范围 for、移动语义全都建立在它上面。这一节按"它和指针到底差在哪"来组织。

### 8.1 别名：没有自己的存储

先看内存。左边是指针，右边是引用，两者做同一件事——让代码能操作 `x`：

```text
   指针：两个对象                    引用：一个对象，两个名字
        0x1000                            0x1000
        +--------+                        +--------+
   x    |   10   |                   x    |   10   |
        +--------+                        +--------+
        0x2000                                  ^
        +--------+                              |
   p    | 0x1000 |  ----+              r  ------+  r 没有自己的地址
        +--------+      |                 (&r == &x == 0x1000)
                        |
                        +--> x            sizeof(r) == sizeof(int)
```

三个可验证的结论：

- `&r == &x`：对引用取地址，得到的是**被引用对象**的地址。语言里根本没有"引用的地址"这个东西。
- `sizeof(r) == sizeof(int)`：`sizeof` 作用在引用上得到的是被引用类型的大小，不是"一个地址的大小"。这直接证明引用不是对象。
- 所以引用不能组成数组（数组元素必须是对象），不能有指向引用的指针（指针必须指向对象），也不存在"引用的引用"——`int& r2 = r;` 里的 `r2` 仍然绑定 `x`，多重引用会**折叠**成一层。

实现层面，编译器十有八九用地址来实现引用（所以引用形参在 ABI 上常常就是一个指针），但这是实现细节。语言层面把它当别名，才能解释上面三条以及"引用不参与拷贝/赋值"这些规则。

那为什么语言不干脆只用指针？因为大多数场合我们并不需要指针的全部能力：不需要空、不需要改指向、不需要算术。用引用表达"这里就是那个对象"，编译器就能替你挡掉一整类错误——空解引用、忘记解引用、指针算术越界。反过来说，引用也把"可选"和"多态"排除在外了，所以指针不会被取代。C++ 的选择是两套工具并存，让你按语义挑，而不是按语法挑。

```cpp
#include <iostream>

int main() {
    int x = 10;
    int& r = x;                    // r 是 x 的别名，不占独立存储
    std::cout << (&r == &x) << ' ' << (sizeof r == sizeof(int)) << '\n';

    ++r;                           // 等价于 ++x
    std::cout << x << '\n';

    int& r2 = r;                   // “引用的引用”塌缩，仍是 x 的别名
    r2 = 30;
    std::cout << x << ' ' << (&r2 == &x) << '\n';

    int y = 7;
    r = y;                         // 这是赋值，不是重新绑定：x 变成 7
    std::cout << x << ' ' << y << ' ' << (&r == &y) << '\n';
}
```

输出：
```text
1 1
11
30 1
7 7 0
```

最后一行 `7 7 0` 是整节最关键的输出：`r = y` 之后，`x` 变成了 7，`y` 还是 7，而 `&r == &y` 为**假**。引用从头到尾没有换过绑定的对象，`r = y` 只是一次普通的赋值（把 y 的值写进 x）。这是引用题第一大坑：**看到 `=` 就想"重新绑定"，是错的**。

### 8.2 必须初始化，且不能改绑

两条规则，两条理由：

1. **必须初始化**。`int& r;` 编译错：`declaration of reference variable 'r' requires an initializer`。名字必须指向某个东西，否则它什么也不是。这条规则让引用天然是"非空"的——这是它相对指针最大的优势。
2. **不能改绑**。绑定发生在初始化那一刻，此后这个符号就永久代表那个对象。C++ 没有语法能表达"让 r 改指向 y"。

推论一串，全是考点：

- 引用没有"顶层 const"的概念：`int& const r` 是非法的（const 修饰引用本身没意义，因为改绑本来就不允许）。只有 `const int&`，意思是"不能通过这个名字写"。
- 引用不占独立存储，所以对它取地址、做 `sizeof`、做算术，作用的全是被引用对象。`&r + 1` 是"x 后面一个元素"，不是"下一个引用"。
- 引用不能为空，也不能"指向谁待定"。需要"可能没有"的语义，只能用指针。

`int& r2 = r;` 这种写法合法，但别被它骗了：这不是"引用的引用"，而是把 `r` 所代表的对象再起一个名字，`r2` 与 `r` 都绑 `x`。C++ 标准明确说"引用会塌缩"，所以不存在 `int&&&` 这种类型。

### 8.3 传参：值与引用的分界

| 形参写法 | 是否拷贝 | 能否改实参 | 能否接字面量/临时量 | 默认用在 |
| --- | --- | --- | --- | --- |
| `T x` | 拷贝一份 | 否 | 能 | 内置类型、小结构 |
| `T& x` | 不拷贝 | 能 | 否 | 输出参数、原地修改 |
| `const T& x` | 不拷贝 | 否 | 能 | 只读的大对象 |
| `T&& x` | 不拷贝 | 能（搬资源） | 能（右值） | 移动构造、转发 |

`const T&` 能接字面量，是因为编译器会为它造一个临时对象，并让这个临时对象活到函数返回（8.5 节讲延长规则）。这是"只读大对象一律 `const T&`"的底气：既不拷贝，又能接受 `f(42)`、`f("abc")`、`f(a + b)` 这些写法。

选择顺序可以背成一句话：**小对象按值，大对象只读用 `const&`，要改用 `&`，可能没有用 `*`**。

为什么内置类型不推荐 `const int&`？因为引用在实现上通常是一次间接寻址，编译器还必须假设"这个引用可能别名别的变量"，从而放弃一部分优化；对一个 4 字节的 `int`，拷贝一次的代价比这更低。`std::string`、`std::vector` 这类对象拷贝一次要分配内存、复制全部元素，`const&` 的优势才明显。

```cpp
#include <iostream>
#include <string>
#include <utility>

struct Probe {
    std::string name;
    explicit Probe(std::string n) : name(std::move(n)) {}
    Probe(const Probe& o) : name(o.name) { std::cout << "  copy " << name << '\n'; }
};

void byValue(Probe p) { p.name += "!"; }        // 改的是副本
void byRef(Probe& p) { p.name += "?"; }         // 改的是本体
void byConstRef(const Probe& p) {               // 只读，不拷贝
    std::cout << "  read " << p.name << '\n';
}

int main() {
    Probe a("alpha");
    std::cout << "byValue\n";
    byValue(a);
    std::cout << a.name << '\n';

    std::cout << "byRef\n";
    byRef(a);
    std::cout << a.name << '\n';

    std::cout << "byConstRef\n";
    byConstRef(a);
    std::cout << "done\n";
}
```

输出：
```text
byValue
  copy alpha
alpha
byRef
alpha?
byConstRef
  read alpha?
done
```

输出把三件事同时说清楚了：`byValue` 真的拷贝了一次（`copy alpha`），而且改的是副本，`a.name` 仍是 `alpha`；`byRef` 没拷贝，改的是本体，所以变成 `alpha?`；`byConstRef` 也没拷贝，只是读。

引用在**范围 for**（range-based for）里天天出现，三种写法的区别几乎每年都考：

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};

    int doubled = 0;
    for (int x : v) { x *= 2; doubled += x; }     // 改的是副本，容器不变
    std::cout << v[0] << v[1] << v[2] << ' ' << doubled << '\n';

    for (int& x : v) x *= 2;                     // 别名：原地改
    std::cout << v[0] << ' ' << v[1] << ' ' << v[2] << '\n';

    int sum = 0;
    for (const int& x : v) sum += x;             // 只读且不拷贝
    std::cout << sum << '\n';
}
```

输出：
```text
123 12
2 4 6
12
```

判断标准只有一条：循环变量有没有 `&`。没有 `&` 就是每个元素拷一份出来改，容器纹丝不动（第一行 `123 12` 里，容器还是 `123`，被改的副本累加出 12）；有 `&` 才是别名，能改本体（第二行 `2 4 6`）；`const&` 是只读别名，省掉拷贝。大对象（`std::string`、结构体）一律用 `const auto&`，这是现代 C++ 的默认写法。

范围 for 有个隐藏陷阱：循环变量是引用时，如果循环体里对容器做了 `push_back`/`erase`，这个引用可能立刻悬垂。规则是**循环期间不要改变容器结构**。

### 8.4 返回引用：让函数调用出现在赋值左边

返回引用意味着把对象本身交出去，而不是它的副本。所以 `f() = 1;` 能编译——只要 `f` 返回 `int&`。

安全性与"返回指针"完全一致，判断标准是**被引用的对象是否比引用活得久**：

| 返回的东西 | 是否安全 | 原因 |
| --- | --- | --- |
| 静态/全局对象 | 安全 | 生命周期到程序结束 |
| 类的成员 | 安全（对象活着时） | 成员的生命周期跟随对象 |
| 容器元素 | 安全（容器不重新分配时） | `push_back`/`resize` 后可能失效 |
| 引用形参 | 安全 | 生命周期由调用者保证 |
| 局部变量 | **悬垂** | 函数返回后栈帧销毁 |
| 值形参 | **悬垂** | 形参本身也是局部对象 |
| 临时量 | **悬垂** | 表达式结束即销毁 |

运算符重载是"必须返回引用"的标准答案：`operator<<` 返回 `std::ostream&` 才能 `cout << a << b` 链式调用；`operator=` 返回 `T&` 才能 `a = b = c`；`operator[]` 返回 `T&` 才能 `v[i] = x`。如果这些改成返回值，链式表达式就会变成"给临时对象赋值"，编译不过或者静默失效。

`const T&` 作为返回类型是另一种语义："给你看，但不许改"，常见于返回成员或全局配置。它同样受"对象是否还活着"的约束，而且调用者拿到的名字不能绑定到非 const 引用上。

```cpp
#include <iostream>
#include <vector>

// 返回容器元素的引用：调用者拿到的是元素本身
int& at(std::vector<int>& v, std::size_t i) { return v[i]; }

struct Point {
    int x;
    int y;
};

// 返回流引用，才能链式输出
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << '(' << p.x << ',' << p.y << ')';
}

int main() {
    std::vector<int> v{10, 20, 30};

    at(v, 1) = 99;                    // 返回引用才能出现在赋值左边
    std::cout << v[1] << '\n';

    int& slot = at(v, 2);
    slot += 1;
    std::cout << v[2] << '\n';

    std::cout << (&at(v, 0) == &v[0]) << '\n';

    Point p{1, 2};
    std::cout << p << p << " end" << '\n';
}
```

输出：
```text
99
31
1
(1,2)(1,2) end
```

`at(v, 1) = 99` 之所以合法，是因为 `at` 返回的是 `v[1]` 这个 `int` 对象的别名，整个表达式等价于 `v[1] = 99`。而 `operator<<` 每调用一次都返回 `os` 本身的引用，所以能一直往右接。

### 8.5 const 引用与生命周期延长

引用的生命周期规则只有一句话：**引用不会延长被绑定对象的寿命**，唯一的例外是——`const T&`（或 `T&&`）**直接**绑定一个临时量时，临时量的寿命被延长到引用的作用域结束。

```text
const int& a = 42;          // 42 这个临时 int 被延长，活到 a 离开作用域
const int& b = a + 1;       // a + 1 的结果是临时量，同样被延长
int&& c = 7;                // 右值引用也延长
```

"直接"两个字是全部难点。延长**不会传递**：

- 函数返回引用时，被引用对象的寿命由函数决定，与调用者的引用无关。`const int& r = std::max(1, 2);` 看起来人畜无害，实际 `std::max` 返回的是它形参的引用，而那两个临时 `int` 在整条语句结束时就销毁了，`r` 当场悬垂。
- 构造函数把临时量绑到**引用成员**上时，临时量在构造函数调用的完整表达式结束时销毁，成员引用随即悬垂（8.7 节）。
- 容器扩容、`delete`、离开作用域，都会让已有引用失效——延长规则管不了这些。

反过来，函数**形参**是 `const T&` 时，传进去的临时量活到整个调用表达式结束，所以 `show("literal")` 是安全的：临时 `std::string` 至少活到 `show` 返回。

考试里判断悬垂有个笨办法但很好用：**把引用换成"指向临时量的指针"，再问这个临时量什么时候销毁**。`const int& r = a + b;` 的临时量活到 `r` 离开作用域，安全；`const int& r = f();` 在 `f` 按值返回时同样被延长，也安全；只有当 `f` 返回**引用**、而它引用的对象已经销毁时，才悬垂。

```cpp
#include <iostream>
#include <string>

std::string greeting() { return "hello"; }

void show(const std::string& s) { std::cout << '[' << s << ']' << '\n'; }

int main() {
    const int& a = 42;                   // 绑定临时量：生命周期延长到 a 结束
    const int& b = a + 1;                // 同样延长
    const std::string& s = greeting();   // 函数返回的临时 string 也被延长
    std::cout << a << ' ' << b << ' ' << s.size() << '\n';

    int&& r = 7;                         // 右值引用同样延长
    r += 1;
    std::cout << r << '\n';

    show("literal");                     // 形参是 const&，临时 string 活到函数返回
}
```

输出：
```text
42 43 5
8
[literal]
```

`int&& r = 7; r += 1;` 能改，是因为 `r` 虽然绑定的是右值，但它自己是一个**具名的**右值引用——具名右值引用在使用时表现得像左值（8.8 节详述）。

### 8.6 引用与指针：十个维度

| 维度 | 指针 `T*` | 引用 `T&` |
| --- | --- | --- |
| 是否必须初始化 | 不必须（但应该写 `nullptr`） | 必须，且必须绑定对象 |
| 能否为空 | 能，"没有"是一种合法状态 | 不能，永远绑着某个对象 |
| 能否改绑 | 能，赋值改指向 | 不能，赋值改的是值 |
| 访问语法 | `*p`、`p->m`，处处提醒你它在间接 | 与普通变量完全相同 |
| 是否占独立存储 | 占，单独一块存地址 | 不占（语义上） |
| `sizeof` | 一个地址的大小 | `sizeof(T)` |
| 能否多级 | `T**`、`T***` 任意层 | 只有一层，多重引用会塌缩 |
| 指针算术 | 有，按元素步进 | 无，`&r + 1` 是对象后面一个元素 |
| 能否组成数组 | 能 | 不能（元素必须是对象） |
| 典型用途 | 可选参数、动态内存、多态、C 接口、数据结构 | 必存在的别名、运算符重载、避免拷贝、移动语义 |

两个类型层面的补充：

- `f(int&)` 与 `f(int*)` 是**不同的重载**，可以共存，调用时 `f(x)` 与 `f(&x)` 各选一个。
- 但 `f(int)` 与 `f(const int&)` 同时存在时，传左值会**二义**（clang 报 `call to 'f' is ambiguous`）。原因是按值传递与绑定 const 引用对左值来说都是"精确匹配"，谁也不比谁好。这也是"不要给同一组参数同时提供值版本和 const 引用版本"的原因。

一句话选型：**能用引用就用引用，需要"可能没有/需要改指向/需要算术"时才用指针**。

```cpp
#include <iostream>

int main() {
    int x = 5;
    int* p = &x;    // 指针：独立对象，里面存地址
    int& r = x;     // 引用：不是对象，只是 x 的另一个名字

    r = 1;                          // 三种写法改的是同一个 int
    std::cout << x << ' ' << *p << '\n';
    *p = 2;
    std::cout << x << ' ' << r << '\n';
    x = 3;
    std::cout << *p << ' ' << r << ' ' << (&r == &x) << '\n';

    std::cout << (sizeof r == sizeof(int)) << ' '
              << (sizeof p == sizeof(void*)) << '\n';

    p = nullptr;                    // 指针可以脱离对象
    std::cout << (p == nullptr) << ' ' << r << '\n';   // 引用永远绑着 x
}
```

输出：
```text
1 1
2 2
3 3 1
1 1
1 3
```

`x`、`*p`、`r` 三者是同一个 `int` 的三种写法，改哪个都一样。区别在最后：`p = nullptr` 之后指针脱离了对象，而 `r` 永远绑着 `x`——引用没有"空"这个状态，所以它也不需要（不能）被检查。

### 8.7 引用成员与初始化列表

类里的引用成员有三条硬规则：

1. **必须用成员初始化列表**初始化。构造函数体里写 `r = x;` 编译错：`constructor for 'Bad' must explicitly initialize the reference member 'r'`。引用没有"先默认构造、再赋值"这条路——它一出生就必须绑好。
2. **初始化顺序由声明顺序决定**，与初始化列表的书写顺序无关。顺序不一致时 clang 会给 `-Wreorder` 警告，而这往往是真 bug 的前兆（用一个还没初始化的成员去初始化另一个）。
3. **含引用成员的类，拷贝赋值运算符被定义为 deleted**。因为赋值意味着"把右边绑到左边"，而引用不能改绑。拷贝构造仍然可用（新对象可以绑到同一个对象上）。

```cpp
#include <iostream>
#include <string>
#include <type_traits>

struct Account {
    int& balance;                 // 引用成员：只能初始化，不能改绑
    const std::string& owner;
    Account(int& b, const std::string& o) : balance(b), owner(o) {}
    void deposit(int amount) { balance += amount; }
};

static_assert(std::is_copy_constructible_v<Account>);
static_assert(!std::is_copy_assignable_v<Account>);   // 引用不能改绑 → 赋值被删除

int main() {
    int wallet = 100;
    std::string name = "ann";
    Account acc(wallet, name);
    acc.deposit(50);
    std::cout << wallet << ' ' << acc.owner << '\n';

    name = "bob";                 // 外部改的是同一个 string 对象
    std::cout << acc.owner << '\n';

    std::cout << std::is_copy_assignable_v<Account> << ' '
              << std::is_copy_constructible_v<Account> << '\n';
}
```

输出：
```text
150 ann
bob
0 1
```

两个 `static_assert` 就是编译器给出的答案：可拷贝构造（`1`），不可拷贝赋值（`0`）。如果业务真的需要"可空、可改绑的成员"，那就该用指针；用引用表达"一定存在且不会换"的关系，语义才准确。

### 8.8 左值、右值引用与 move

先分清两种表达式：

- **左值**（lvalue）：有名字、能取地址、能放在赋值左边。变量、`*p`、`v[0]`、`++i` 都是。
- **右值**（rvalue）：临时量、字面量、运算结果。`42`、`a + b`、`f()` 的返回值都是。它们没有名字，通常活不过当前语句。

绑定规则：

| 引用类型 | 能绑左值 | 能绑右值 | 典型用途 |
| --- | --- | --- | --- |
| `T&` | 能 | 否 | 修改实参 |
| `const T&` | 能 | 能 | 只读、兼容一切 |
| `T&&` | 否（除非 `std::move`） | 能 | 移动、转发 |

`std::move(x)` **不移动任何东西**，它只是 `static_cast<T&&>(x)`——把左值强行标记成"可以被搬走"。真正搬资源的是移动构造函数。所以：

- `std::string b = std::move(a);` 选中移动构造，`b` 抢走 `a` 的堆缓冲区，代价 O(1)；`a` 进入"有效但未指定"的状态，只能析构或重新赋值，**不要读它的值**。
- 对 `const` 对象用 `std::move` 得到的是 `const T&&`，绑不进 `T&&` 形参，只会静默退化成拷贝。
- **具名的右值引用是左值**：`void f(T&& x) { g(x); }` 里 `x` 有名字，传给 `g` 时选的是 `g(T&)`。要继续搬运必须写 `g(std::move(x))`。
- 移动构造应标 `noexcept`，否则 `std::vector` 扩容时为了强异常保证会退化成拷贝。
- 返回局部变量时**不要**写 `return std::move(x);`，那会阻止 NRVO（返回值优化），反而多一次移动。

移动的收益只在"持有堆资源"的类型上明显：`std::string`、`std::vector` 的移动构造只搬几个指针和长度，O(1)；拷贝要重新分配并逐元素复制，O(n)。对 `struct Point { int x, y; };` 这种只含内置类型的小结构，移动和拷贝的开销完全一样，专门写移动构造没有意义。

```cpp
#include <iostream>
#include <utility>

void which(int&)       { std::cout << "int&" << '\n'; }
void which(int&&)      { std::cout << "int&&" << '\n'; }
void which(const int&) { std::cout << "const int&" << '\n'; }

struct Buffer {
    int* data = nullptr;
    int n = 0;

    explicit Buffer(int k) : data(new int[k]{}), n(k) {}
    ~Buffer() { delete[] data; }

    Buffer(const Buffer& o) : data(new int[o.n]), n(o.n) {
        for (int i = 0; i < n; ++i) data[i] = o.data[i];
        std::cout << "copy" << '\n';
    }
    Buffer(Buffer&& o) noexcept : data(o.data), n(o.n) {
        o.data = nullptr;      // 把资源“掏空”，源对象仍可安全析构
        o.n = 0;
        std::cout << "move" << '\n';
    }
};

int main() {
    int i = 1;
    const int ci = 2;
    which(i);              // 左值
    which(3);              // 纯右值
    which(ci);             // const 左值
    which(std::move(i));   // std::move 只是把 i 转成 int&&

    Buffer a(4);
    Buffer b = a;              // 左值 → 拷贝
    Buffer c = std::move(a);   // 右值 → 移动
    std::cout << b.n << ' ' << c.n << ' ' << a.n << '\n';
}
```

输出：
```text
int&
int&&
const int&
int&&
copy
move
4 4 0
```

前四行是重载决议：`which(i)` 选 `int&`，`which(3)` 选 `int&&`，`which(ci)` 只能选 `const int&`，而 `which(std::move(i))` 说明 `std::move` 只是换了个"身份标签"，`i` 本身一个字节都没动。后三行是移动的本质：`Buffer c = std::move(a)` 打印 `move`，把 `a.data` 这个指针直接偷走并把源指针置空，所以 `a.n` 变成 0——这是我们自己写的类，源对象的状态由我们规定，因此输出是确定的。

### 8.9 常见坑与反例

❌ 引用未初始化——编译期就被拦住，这是引用最好的地方。

```cpp-fragment
int& r;              // 编译错：requires an initializer
int& s = 0;          // 编译错：不能用 int 初始化 int&（0 是右值）
const int& t = 0;    // ✅ 可以：const 引用能绑临时量
```

❌ 返回局部变量的引用——能编译，运行时 UB。

```cpp-fragment
int& bad() {
    int x = 0;
    return x;        // UB：x 随栈帧销毁，返回的引用悬垂
}
int& ok() {
    static int x = 0;
    return x;        // ✅ 静态对象活到程序结束
}
```

❌ 引用成员没走初始化列表。

```cpp-fragment
struct Bad {
    int& r;
    Bad(int& x) { r = x; }   // 编译错：must explicitly initialize the reference member
};
struct Good {
    int& r;
    Good(int& x) : r(x) {}   // ✅
};
```

❌ 临时量绑到引用成员，延长规则不穿过成员——这段**能编译**，是本节最阴的坑。

```cpp-fragment
struct S {
    const int& r;
};
S s{42};             // 编译通过，但 42 的临时量在整条声明语句结束时就销毁
std::cout << s.r;    // UB：悬垂引用
```

✅ 成员要长期持有就用值（`int r;`）或指针（`const int* r;`），别用引用成员去接临时量。

❌ 容器元素的引用跨越了结构性修改。

```cpp-fragment
std::vector<int> v{1, 2, 3};
int& first = v[0];
v.push_back(4);      // 可能重新分配：元素被搬到新内存
first = 9;           // UB：first 指向已释放的旧内存
```

✅ 要么在修改后重新取 `v[0]`，要么先 `v.reserve(n)` 把容量定下来（但仍不要跨越 `erase`）。

❌ 以为 `r = y` 是重新绑定。

```cpp-fragment
int x = 1, y = 2;
int& r = x;
r = y;               // 不是改绑！这是 x = 2，r 仍然代表 x
```

### 8.10 坑清单

| 写法 | 后果 |
| --- | --- |
| `int& r;` | 编译错，引用必须初始化 |
| `r = y;` 以为改绑 | 静默错误：改的是被引用对象的值 |
| `int& f() { int x; return x; }` | 悬垂引用，UB |
| `S s{42};`（成员是 `const int&`） | 编译通过但悬垂，UB |
| 构造函数体里给引用成员赋值 | 编译错，必须用初始化列表 |
| 含引用成员的类做拷贝赋值 | 拷贝赋值被 deleted，编译错 |
| `auto& x = v[0];` 后 `push_back` | 悬垂，UB |
| `f(int)` 与 `f(const int&)` 并存 | 传左值时二义，编译错 |
| `T&&` 形参在函数体内直接传给别的函数 | 具名右值引用是左值，实际走拷贝 |
| `return std::move(local);` | 阻止 NRVO，性能更差 |
