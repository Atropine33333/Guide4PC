# C++ 基础语法复习讲义

> 面向期中/期末复习的单页提纲：规则 + 对比表 + 可直接运行、且输出已经逐字验证的示例。
> 运行环境：C++20，`clang++ -std=c++20 -Wall -O0`；示例不读取 stdin，可以直接粘贴到 `/lab/` 运行。

## 目录

1. 程序结构、编译与输入输出
2. 基本类型与字面量
3. 运算符与表达式
4. 控制流
5. 函数
6. 数组与多维数组
7. 指针
8. 引用
9. 字符、字符串与 string
10. 结构体与类基础
11. 拷贝、移动与运算符重载
12. 继承与多态
13. 模板与泛型
14. STL 容器与迭代器
15. 算法与常用技巧
16. 常见陷阱清单与速查表

---
## 1. 程序结构、编译与输入输出

本节把三件事串起来：一个 C++ 程序由什么组成，源码怎样变成可执行文件，以及怎样用标准库读写数据。期中考试里这部分常以选择题、改错题和"输出什么"的形式出现，重点在入口函数、命名空间、头文件与源文件的分工，以及 `cin`/`cout` 的细节。

### 1.1 最小程序与 main

每个 C++ 程序必须有且只有一个 `main` 函数，它是程序入口。标准规定 `main` 返回 `int`，返回值交给操作系统：0 表示成功，非 0 表示失败。`main` 是唯一允许"不写 return 就到达末尾"的函数，此时等价于 `return 0`；但显式写出更清楚，也不容易在考试中丢分。

`#include <iostream>` 是预处理指令，编译前把标准库头文件的声明"粘贴"到当前位置，`std::cout`、`std::cin` 的声明就来自这里。`std` 是标准库的命名空间，`cout`、`cin`、`string`、`vector` 都在其中。可以写 `using namespace std;` 省掉前缀，但大型工程中容易和自定义名字冲突；更稳的写法是全写 `std::`，或只引入需要的名字（`using std::cout;`）。

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, C++20!\n";
    return 0;
}
```

输出：
```text
Hello, C++20!
```

### 1.2 编译、链接与运行

从源文件到能运行的程序，至少经历三个阶段：

| 阶段 | 做什么 | 产物 | 典型错误 |
| --- | --- | --- | --- |
| 预处理 | 展开 `#include`、宏，处理条件编译 | 翻译单元 | 找不到头文件 |
| 编译 | 语法、类型检查，生成目标代码 | `.o` / `.obj` | 语法错、类型错、名字未声明 |
| 链接 | 合并目标文件和库，解析外部符号 | `.exe` / `a.out` | `undefined reference` / `undefined symbol` |

编译错误定位在单个源文件，链接错误则说明"名字有声明、找不到定义"。例如函数只在头文件里声明、忘了写函数体，编译能过，链接会报未定义符号：GNU ld 写作 `undefined reference to 'square(int)'`，本机 lld 写作 `undefined symbol: square(int)`。运行阶段才出现的问题（越界、空指针、逻辑错）编译器一般发现不了，这也是"能编译不等于对"的原因。

### 1.3 头文件、源文件与声明/定义

声明（declaration）只告诉编译器名字和类型；定义（definition）才真正分配存储或给出函数体。同一个函数可以声明多次，但只能定义一次，这叫单一定义规则（ODR）。所以头文件里通常放声明、类定义、模板和 `inline` 函数，普通函数的定义放 `.cpp`；头文件还要用 `#pragma once` 或 include guard 防止重复包含。

```cpp
#include <iostream>

int square(int x);        // 声明：只告诉编译器函数的样子

int main() {
    std::cout << square(7) << '\n';
}

int square(int x) {       // 定义：真正给出函数体
    return x * x;
}
```

输出：
```text
49
```

### 1.4 用 cin/cout 读写

`std::cout << x` 把 x 按类型转成文本写到标准输出；`std::cin >> x` 先跳过前导空白，再按 x 的类型解析。两者都返回流对象，所以能链式写 `cin >> a >> b`。关键细节：`>>` 读完数字后把后面的换行符留在缓冲区，紧接着调用 `getline` 会读到空行。解决办法是先用 `cin.ignore()` 吃掉换行，或者全程用 `getline` 再解析。

下面的程序用 `std::istringstream` 提供固定输入，这样不依赖键盘、输出可复现；真实程序把构造 `in` 的那一行换成 `std::cin` 即可。

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::istringstream in("42 3.5\nhello world\n");   // 用字符串模拟键盘输入

    int n = 0;
    double d = 0.0;
    in >> n >> d;              // 按空白分隔读取，格式由变量类型决定
    in.ignore();               // 吃掉数字行末的换行符

    std::string line;
    std::getline(in, line);    // 读一整行，不含换行

    std::cout << n << ' ' << d << '\n';
    std::cout << line << '\n';
}
```

输出：
```text
42 3.5
hello world
```

`std::getline(in, line)` 读一整行并丢掉行尾换行，但保留行首空格；空行会得到空串。判断输入是否成功，可以把读取放进条件：`while (std::getline(in, line))`。

### 1.5 格式化输出 iomanip

`<iomanip>` 提供操纵符控制宽度、精度、对齐和填充。它们修改的是流的状态，**大多数设置一次后一直有效**，只有 `setw` 例外，只影响下一次输出。

| 操纵符 | 作用 | 是否持续 |
| --- | --- | --- |
| `std::fixed` | 定点表示浮点数 | 是 |
| `std::setprecision(n)` | 小数位数（配合 `fixed`） | 是 |
| `std::setw(n)` | 下一次输出的最小宽度 | 否 |
| `std::left` / `std::right` | 左对齐 / 右对齐 | 是 |
| `std::setfill(c)` | 空白处填充字符 c | 是 |
| `std::boolalpha` | bool 输出 `true`/`false` | 是 |

```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(2) << 3.14159 << '\n';

    std::cout << '[' << std::setw(6) << 42 << "]\n";
    std::cout << '[' << std::left << std::setw(6) << 42 << "]\n";
    std::cout << '[' << std::right << std::setfill('0') << std::setw(5) << 7 << "]\n";
}
```

输出：
```text
3.14
[    42]
[42    ]
[00007]
```

`std::endl` 输出换行并刷新缓冲区，`'\n'` 只换行不刷新。大量输出时 `'\n'` 更快；只在确实需要立即看到输出（交互提示、调试）时用 `endl`。

### 1.6 常见编译与链接错误

- ❌ 写 `cout` 却忘了 `#include <iostream>` 或 `std::`，编译报 `use of undeclared identifier 'cout'`；✅ 补上头文件，并写 `std::cout`。
- ❌ 漏分号，报错位置常常指向下一行；✅ 先看上一行末尾，补上 `;`。
- ❌ 写成 `void main()`，clang 报 `'main' must return 'int'`；✅ 写成 `int main()`。
- ❌ 头文件里定义普通函数，被多个源文件包含，链接期报 `multiple definition`；✅ 头文件只放声明，定义放进一个 `.cpp`。
- ❌ 函数只有声明没有定义，链接期报未定义符号（`undefined reference` / `undefined symbol`）；✅ 补上函数体，或确认链接了对应的目标文件。

## 2. 基本类型与字面量

本节梳理 C++ 的基本类型、它们占多少空间、能表示多大范围，以及字面量、`auto`、`const`/`constexpr` 和类型转换。这些内容本身不难，但边界（溢出、截断、精度、有无符号混用）是选择题和"程序输出什么"的高频考点。

### 2.1 整型家族与 sizeof

C++ 的基本整型有 `char`、`short`、`int`、`long`、`long long`，每种都有对应的 `unsigned` 版本，另有 `bool`。标准只规定最小宽度和相对大小：`sizeof(char) == 1`，且 `sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)`；具体字节数由平台决定。本讲义环境是 64 位 Windows（LLP64 模型），`long` 只有 4 字节，和 `int` 一样宽——这是从 Linux（LP64，`long` 为 8 字节）迁移代码时最常见的差异之一。考试若问"int 一定占 4 字节吗"，答案是否定的，标准只保证至少 16 位。

```cpp
#include <iostream>

int main() {
    std::cout << "char      " << sizeof(char) << '\n';
    std::cout << "short     " << sizeof(short) << '\n';
    std::cout << "int       " << sizeof(int) << '\n';
    std::cout << "long      " << sizeof(long) << '\n';
    std::cout << "long long " << sizeof(long long) << '\n';
    std::cout << "float     " << sizeof(float) << '\n';
    std::cout << "double    " << sizeof(double) << '\n';
    std::cout << "bool      " << sizeof(bool) << '\n';
}
```

输出：
```text
char      1
short     2
int       4
long      4
long long 8
float     4
double    8
bool      1
```

`sizeof` 的结果类型是 `size_t`，无符号，单位是字节。它对表达式不求值（`sizeof(i++)` 不会让 i 自增），对数组返回整个数组的字节数，这也是 `sizeof(a)/sizeof(a[0])` 求元素个数的来历。

### 2.2 有无符号、范围与溢出

n 位无符号整数的范围是 0 到 2^n−1；n 位补码有符号整数的范围是 −2^(n−1) 到 2^(n−1)−1。32 位 `int` 的最大值是 2147483647，再加 1 属于**有符号溢出**，是未定义行为（UB），编译器可以假设它不会发生，因此不能依赖它"变成负数"或"回绕"。无符号整数的加减则按 2^n 取模，回绕是良定义的，结果一定是 0 到 2^n−1。

| 对比项 | 有符号整数 | 无符号整数 |
| --- | --- | --- |
| 表示范围 | −2^(n−1) ~ 2^(n−1)−1 | 0 ~ 2^n−1 |
| 溢出行为 | 未定义行为（UB） | 按 2^n 回绕，有定义 |
| 混合运算 | 转成无符号，负数变成巨大正数 | 保持无符号 |
| 典型用途 | 计数、差值、可能为负的量 | 位掩码、下标、`size()` |

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << INT_MAX << ' ' << INT_MIN << '\n';

    unsigned int u = 0;
    u -= 1;                     // 无符号回绕：结果有定义
    std::cout << u << '\n';

    unsigned char c = static_cast<unsigned char>(300);  // 取模 256，得 44
    std::cout << static_cast<int>(c) << '\n';

    char a = 100;
    char b = 100;
    int sum = a + b;            // 先提升为 int，不会在 char 里溢出
    std::cout << sum << '\n';
}
```

输出：
```text
2147483647 -2147483648
4294967295
44
200
```

整型提升（integral promotion）：`char`、`short`、`bool` 等小于 `int` 的类型参与算术运算时，先提升为 `int`（前提是 int 能表示其全部值）。所以两个 `char` 相加得到 `int`，不会在 8 位里溢出；把结果存回 `char` 才发生截断。有符号与无符号混用时，按"通常算术转换"统一到公共类型，通常是有符号转无符号，负数的补码被当成巨大的正数，这是很多隐蔽 bug 的来源。

### 2.3 浮点类型与精度

`float` 一般 4 字节、约 7 位十进制有效数字；`double` 8 字节、约 15 到 17 位；`long double` 在 x86 上常见为 80 位扩展精度（本机 `sizeof` 为 16）。浮点数按二进制科学计数法存储，`0.1`、`0.2`、`0.3` 都无法精确表示，因此 `0.1 + 0.2 == 0.3` 的结果是 `false`。判断相等应比较差值：`std::abs(a - b) < 1e-9`，epsilon 的大小按量级选取。

```cpp
#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    double a = 0.1;
    double b = 0.2;

    std::cout << std::setprecision(17) << (a + b) << '\n';
    std::cout << std::boolalpha << (a + b == 0.3) << '\n';
    std::cout << (std::abs((a + b) - 0.3) < 1e-9) << '\n';
}
```

输出：
```text
0.30000000000000004
false
true
```

### 2.4 char、转义与 bool

`char` 既是字符也是小整数。`'A'` 是字符字面量，类型为 `char`，值 65（ASCII）；`"A"` 是字符串字面量，类型为 `const char[2]`，末尾还有一个 `'\0'`，两者不能混用。常用转义：`\n` 换行、`\t` 制表、`\\` 反斜杠、`\"` 双引号、`\'` 单引号、`\0` 空字符、`\x41` 十六进制、`\101` 八进制。

`bool` 只有 `true` 和 `false` 两个值，`sizeof(bool)` 通常为 1。非零值转 bool 为 true，0 为 false；bool 参与算术时提升为 1 或 0。默认 `cout` 打印 bool 是 0/1，设置 `std::boolalpha` 后才打印 `true`/`false`。

```cpp
#include <iostream>

int main() {
    char c = 'A';
    std::cout << c << ' ' << static_cast<int>(c) << '\n';

    std::cout << "tab:\t|" << '\n';
    std::cout << "quote:\"" << '\n';
    std::cout << "backslash:\\" << '\n';

    std::cout << std::boolalpha << true << ' ' << false << '\n';
    std::cout << std::noboolalpha << true << '\n';
}
```

输出：
```text
A 65
tab:	|
quote:"
backslash:\
true false
1
```

### 2.5 auto、const、constexpr 与字面量

`auto` 由初始化器推导类型，必须初始化。它**丢弃顶层 const 和引用**：`const int& r = x; auto a = r;` 得到 `int`；要保留引用写 `auto&`，保留 const 写 `const auto`。字面量的类型也遵循规则：`10` 是 `int`，`10u` 是 `unsigned int`，`10LL` 是 `long long`，`10.0f` 是 `float`，`10.0` 是 `double`。

`const` 表示运行期不可修改，初始化后不能赋值；`constexpr` 要求编译期可求值，能用于数组大小、模板参数和 `case` 标签。`constexpr` 函数在实参是常量表达式时编译期求值，否则退化为普通函数。

```cpp
#include <iostream>

constexpr int square(int x) {
    return x * x;
}

int main() {
    auto n = 10;              // int
    auto u = 10u;             // unsigned int
    auto big = 10LL;          // long long
    auto f = 10.0f;           // float

    constexpr int k = square(5);
    const int limit = 10;

    std::cout << n << ' ' << u << ' ' << big << ' ' << f << '\n';
    std::cout << k << ' ' << limit << '\n';
    std::cout << sizeof(n) << ' ' << sizeof(u) << ' '
              << sizeof(big) << ' ' << sizeof(f) << '\n';
}
```

输出：
```text
10 10 10 10
25 10
4 4 8 4
```

常用后缀：`u`/`U`、`l`/`L`、`ll`/`LL`、`f`/`F`。`0x` 十六进制、`0b` 二进制（C++14）、前导 `0` 八进制；C++14 起可用单引号分隔数字，如 `1'000'000`。十进制整数字面量默认 `int`，放不下时依次尝试 `long`、`long long`；八进制、十六进制和二进制还会尝试对应的无符号类型，所以 `0xFFFFFFFF` 在 32 位 `int` 上是 `unsigned int`。

### 2.6 类型转换与常见坑

隐式转换发生在赋值、传参和混合运算中，窄化会丢数据。`double d = 3.99; int i = d;` 得到 3，是向零截断而不是四舍五入。`static_cast<T>(x)` 是编译期检查的显式转换，意图清楚、容易搜索；C 风格 `(T)x` 会依次尝试多种转换（`const_cast`、`reinterpret_cast` 等），危险且难查。列表初始化 `int i{3.9}` 禁止窄化，编译直接报错，是推荐的写法。

```cpp-fragment
double d = 3.99;
int i = d;                      // 隐式窄化：i == 3
int j = static_cast<int>(d);    // 显式转换，意图清楚
int k{d};                       // 错误：列表初始化不允许窄化
```

常见坑：
- ❌ `char c = 300;`：`char` 只有 8 位，存不下；若 `char` 有符号，结果是实现定义（本机为 44），换平台可能不同。✅ 用 `static_cast<unsigned char>(300)` 明确取模，或改用 `int` 等更宽的类型。
- ❌ `for (unsigned i = n - 1; i >= 0; --i)`：i 减到 0 后再减会回绕成最大值，循环永不结束。✅ 用 `int`，或写 `for (unsigned i = n; i-- > 0;)`。
- ❌ `-1 < v.size()`：`size()` 返回无符号的 `size_t`，`-1` 会被转成最大无符号值，比较结果为 false。✅ 两边转成同一类型，例如 `static_cast<long long>(v.size())`，或避免与负数比较。

## 3. 运算符与表达式

本节整理运算符的分类、优先级、结合性，以及短路求值、求值顺序、整型提升这些"表达式层面的规则"。考试中常以"下列表达式的值是多少""这行代码哪里错了"的形式出现，核心是分清优先级和未定义行为。

### 3.1 运算符分类

| 类别 | 运算符 | 说明 |
| --- | --- | --- |
| 算术 | `+ - * / %` | `%` 只用于整型；除法向零截断 |
| 关系/相等 | `< <= > >= == !=` | 结果是 bool |
| 逻辑 | `&&`、`\|\|`、`!` | 短路求值 |
| 位 | `&`、`\|`、`^`、`~`、`<<`、`>>` | 按二进制位操作 |
| 赋值 | `=`、`+=`、`-=`、`*=`、`/=`、`%=`、`&=`、`\|=`、`^=`、`<<=`、`>>=` | 右结合 |
| 自增/自减 | `++`、`--` | 前缀先改后取，后缀先取后改 |
| 三目 | `?:` | 唯一的三目运算符，右结合 |
| 其他 | `sizeof`、`,`、`[]`、`.`、`->`、`()` | 逗号表达式取最后一个值 |

```cpp
#include <iostream>

int main() {
    std::cout << 7 / 2 << ' ' << 7 % 2 << '\n';
    std::cout << -7 / 2 << ' ' << -7 % 2 << '\n';
    std::cout << 7.0 / 2 << '\n';
    std::cout << (1 << 4) << ' ' << (0xF0 >> 2) << '\n';
    std::cout << (5 & 3) << ' ' << (5 | 3) << ' '
              << (5 ^ 3) << ' ' << (~5) << '\n';
}
```

输出：
```text
3 1
-3 -1
3.5
16 60
1 7 6 -6
```

C++ 的 `/` 对整数是向零截断：`-7 / 2 == -3`；`%` 的结果符号与被除数一致：`-7 % 2 == -1`。混合 `int` 和 `double` 时先转成 `double`，所以 `7.0 / 2 == 3.5`。位运算只对整型有效，`~5` 在补码下是 `-6`。

### 3.2 优先级与结合性

优先级决定先算谁，结合性决定同级从左还是从右。常用运算符从高到低如下（同一行优先级相同）：

| 优先级 | 运算符 | 结合性 |
| --- | --- | --- |
| 1 | `::` | 左 |
| 2 | `()` `[]` `.` `->` 后缀 `++` `--` | 左 |
| 3 | 前缀 `++` `--` `!` `~` `*` `&` `sizeof` `static_cast` 等 | 右 |
| 4 | `.*` `->*` | 左 |
| 5 | `*` `/` `%` | 左 |
| 6 | `+` `-` | 左 |
| 7 | `<<` `>>` | 左 |
| 8 | `<=>` | 左 |
| 9 | `<` `<=` `>` `>=` | 左 |
| 10 | `==` `!=` | 左 |
| 11 | `&` | 左 |
| 12 | `^` | 左 |
| 13 | `\|` | 左 |
| 14 | `&&` | 左 |
| 15 | `\|\|` | 左 |
| 16 | `?:` | 右 |
| 17 | `=` `+=` `-=` `*=` `/=` `%=` `&=` `\|=` `^=` `<<=` `>>=` | 右 |
| 18 | `,` | 左 |

赋值、三目、单目是右结合，其余多为左结合；记不住时先查表，再给易混处加括号。

最容易错的是位运算优先级低于 `==`：`a & b == c` 实际是 `a & (b == c)`，而不是 `(a & b) == c`，必须加括号。另一组高频坑是 `*p++` 与 `(*p)++`：后缀 `++` 优先级高于 `*`，前者是"取当前指向的值，然后指针后移"，后者是"把指向的值加一"。

```cpp-fragment
int a = 2, b = 1, c = 0;
bool x = a & b == c;    // 实际是 a & (b == c)，不是 (a & b) == c
bool y = (a & b) == c;  // 加括号才是本意
```

### 3.3 短路求值

`&&` 和 `||` 保证先算左边，且左边能决定结果时右边完全不求值。这既是优化也是安全手段：`p != nullptr && *p > 0` 在 p 为空时不会解引用。短路只属于逻辑运算符，位运算 `&`、`|` 两边都会求值，不能拿来代替。

```cpp
#include <iostream>

int calls = 0;

bool check() {
    ++calls;
    return true;
}

int main() {
    int* p = nullptr;
    if (p != nullptr && *p > 0) {   // 左边为假，右边不会解引用
        std::cout << "unreachable\n";
    }

    std::cout << (false && check()) << ' ' << calls << '\n';
    std::cout << (true || check()) << ' ' << calls << '\n';
    std::cout << (true && check()) << ' ' << calls << '\n';
}
```

输出：
```text
0 0
1 0
1 1
```

### 3.4 自增与自减

`++i` 先加 1 再返回新值，`i++` 先返回旧值再自加。后缀版本要保存旧值，对内置类型编译器会优化掉，对迭代器则可能真有开销，所以循环里习惯写 `++it`。`i++` 不能作左值，`++i` 对内置类型返回引用，可以作左值。

```cpp
#include <iostream>

int main() {
    int i = 5;
    int a = i++;           // 先取 5，再自增
    int b = ++i;           // 先自增到 7，再取值
    std::cout << i << ' ' << a << ' ' << b << '\n';

    int arr[3] = {10, 20, 30};
    int j = 0;
    int first = arr[j++];  // 取 arr[0]，然后 j 变为 1
    std::cout << first << ' ' << j << '\n';
}
```

输出：
```text
7 5 7
10 1
```

### 3.5 逗号、赋值与三目

逗号表达式从左到右求值，整个表达式的值是**最后一个**子表达式的值，常用于 `for` 的迭代表达式。赋值运算符右结合，`a = b = c` 等价于 `a = (b = c)`，表达式的值是赋进去的值——这也是 `if (a = b)` 能通过编译的原因。三目运算符 `cond ? a : b` 是唯一的三目运算符，右结合；两个分支类型不同时会做公共类型转换，`true ? 1 : 2.5` 的结果是 `double`。

```cpp
#include <iostream>

int main() {
    int x = 0;
    int y = 0;
    x = (y = 3, y + 1);            // 逗号表达式取最后一个值
    std::cout << x << ' ' << y << '\n';

    int m = 5;
    int n = (m > 3) ? 100 : 200;   // 三目运算符
    std::cout << n << '\n';

    int p = 1, q = 2, r = 3;
    p = q = r = 10;                // 赋值右结合
    std::cout << p << ' ' << q << ' ' << r << '\n';
}
```

输出：
```text
4 3
100
10 10 10
```

### 3.6 求值顺序与未定义行为

C++17 起明确了一部分顺序：赋值号右侧先于左侧，移位运算符左操作数先于右操作数，函数调用的后缀表达式先于实参，列表初始化元素从左到右。函数实参之间的求值顺序仍是**未指定**的：`f(i++, i++)` 自 C++17 起不再因此构成未定义行为，但两个实参谁先求值、各自拿到旧值还是新值并不确定，可读性极差。真正仍然是 UB 的是同一变量在同一表达式中被**无顺序**地修改多次，例如 `i++ + i++`、`i = ++i + i++`。考试中遇到"写出表达式的值"，只要涉及同一变量多次修改且没有确定顺序，答案往往是"未定义"或"未指定"。

### 3.7 有符号、无符号与截断

二元算术运算先把操作数提升，再转换到公共类型；有符号遇到无符号时，有符号数转成无符号。于是 `unsigned u = 2; int i = -3; u + i` 的结果是 4294967295，而不是 -1。赋值给更窄的类型时只保留低位，发生截断。移位要注意：移位量小于 0 或不小于位宽仍是 UB。C++20 起有符号数采用补码，右移规定为算术右移（保留符号位），负数左移也按 2^N 取模定义；但为了可移植和易读，位运算仍建议优先用无符号类型。

```cpp
#include <iostream>

int main() {
    unsigned int u = 2;
    int i = -3;

    std::cout << (u + i) << '\n';                    // int 先转成 unsigned
    std::cout << (static_cast<int>(u) + i) << '\n';

    unsigned int down = 0;
    down -= 1;
    std::cout << down << '\n';
}
```

输出：
```text
4294967295
-1
4294967295
```

### 3.8 常见坑

- ❌ `if (a = b)`：赋值表达式的值就是 b，条件几乎总为真；✅ 本意是 `a == b`，把常量写左边（`0 == a`）能让编译器在漏写等号时报错。
- ❌ `a & b` 当成逻辑与：`2 & 1` 是 0（假），而 `2 && 1` 才是 true；✅ 逻辑判断用 `&&`，位掩码才用 `&`。
- ❌ 以为 `-7 / 2` 是 -4、`-7 % 2` 是 1：整数除法向零截断，余数符号跟被除数；✅ 记住 `-7 / 2 == -3`、`-7 % 2 == -1`，需要数学取模时自己调整。
- ❌ `1 << 32`：在 32 位 `int` 上移位量等于位宽，是 UB，不是 0；✅ 保证 `0 <= 移位量 < 位宽`，必要时用 `1ull` 并检查。
- ❌ 三目与算术、比较混用却不加括号：`a + b > c ? x : y` 虽然按 `((a + b) > c) ? x : y` 解析，读者仍容易看错；✅ 显式写成 `((a + b) > c) ? x : y`。

## 4. 控制流

本节讲条件分支、循环和跳转语句，以及它们背后的作用域与生命周期规则。控制流本身直观，但悬垂 `else`、`switch` 贯穿、循环边界和范围 `for` 的引用语义是考试与代码审查中的常客。

### 4.1 if/else

`if (cond)` 中 cond 按语境转换为 bool：0、`nullptr`、`false` 为假，其余为真。`else` 与**最近的、尚未配对的** `if` 结合，这就是悬垂 else（dangling else）；缩进不会改变配对关系，加 `{}` 是消除歧义最可靠的办法。

多分支用 `else if` 链，条件互斥且按范围从大到小或从小到大排列。C++17 允许在 if 中写初始化语句：`if (int x = f(); x > 0)`，x 只在 if/else 内可见，可以缩小作用域、避免变量泄漏到外层。

```cpp
#include <iostream>

int main() {
    int scores[] = {95, 82, 60, 41};

    for (int score : scores) {
        if (score >= 90) {
            std::cout << score << " A\n";
        } else if (score >= 80) {
            std::cout << score << " B\n";
        } else if (score >= 60) {
            std::cout << score << " C\n";
        } else {
            std::cout << score << " F\n";
        }
    }
}
```

输出：
```text
95 A
82 B
60 C
41 F
```

### 4.2 switch

`switch` 的表达式必须是整型或枚举类型（`char` 也可以），`case` 标签必须是常量表达式且不能重复。执行时从匹配的 `case` 开始，**一直往下执行**，直到遇到 `break` 或 `switch` 结束，这叫贯穿（fallthrough）。多个 `case` 连续写可以共用一段代码；忘记 `break` 则会多执行后面的分支，是经典错误。C++17 起用 `[[fallthrough]];` 显式表示"故意贯穿"，编译器便不再警告。

`default` 可以放在任意位置，习惯放最后。若在某个 `case` 里定义变量且其作用域会跨越后面的 `case`，要用 `{}` 把该分支包起来，否则报 `jump to case label crosses initialization`。

```cpp
#include <iostream>

int main() {
    for (int day = 1; day <= 3; ++day) {
        switch (day) {
            case 1:
            case 2:
                std::cout << day << ": weekday\n";
                break;
            case 3:
                std::cout << day << ": Wednesday\n";
                [[fallthrough]];          // 故意贯穿到 default
            default:
                std::cout << "  fallthrough to default\n";
        }
    }
}
```

输出：
```text
1: weekday
2: weekday
3: Wednesday
  fallthrough to default
```

### 4.3 while、do-while 与 for

| 循环 | 判断时机 | 最少执行次数 | 适用场景 |
| --- | --- | --- | --- |
| `while` | 每次迭代前 | 0 | 次数未知、依赖条件 |
| `do-while` | 每次迭代后 | 1 | 至少执行一次（如输入校验） |
| `for` | 每次迭代前 | 0 | 次数已知、有明确初值和步长 |
| 范围 `for` | 遍历序列 | 0 | 遍历数组或容器全部元素 |

`for (init; cond; step)` 的执行顺序是 init → cond → body → step → cond → …，init 只执行一次。`while` 先判断后执行，可能一次都不执行；`do-while` 先执行后判断，至少执行一次，且末尾必须写分号。

选择循环时：遍历整个序列优先用范围 `for`；需要下标、倒序或跳步时用普通 `for`；次数未知、依赖条件用 `while`；至少要执行一次用 `do-while`。循环条件里尽量用固定值，例如先把容器的 `size()` 存到 `n`，并让下标与 `n` 同类型，符号问题和边界一起变得清楚。

```cpp
#include <iostream>

int main() {
    int n = 12345;
    int digits = 0;
    do {
        n /= 10;
        ++digits;
    } while (n > 0);
    std::cout << "digits=" << digits << '\n';

    int i = 0;
    while (i < 3) {
        std::cout << i << '\n';
        ++i;
    }
}
```

输出：
```text
digits=5
0
1
2
```

### 4.4 范围 for

`for (decl : range)` 依次把 range 的每个元素绑定到 decl，等价于用 `begin()`/`end()` 的迭代器遍历。`decl` 写成 `int x` 时是**拷贝**，改 x 不影响容器；写成 `int& x` 才能修改元素；只读遍历大对象时写 `const auto&` 避免拷贝。遍历期间不要增删容器元素，否则迭代器失效，行为未定义。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, -2, 3, -4, 5};

    int sum = 0;
    for (int x : v) {
        if (x < 0) continue;   // 跳过负数
        if (x == 5) break;     // 遇到 5 提前结束
        sum += x;
    }
    std::cout << sum << '\n';

    for (int& x : v) {
        x *= 2;                // 用引用修改元素
    }
    for (int x : v) {
        std::cout << '[' << x << ']';
    }
    std::cout << '\n';
}
```

输出：
```text
4
[2][-4][6][-8][10]
```

### 4.5 break、continue 与 goto

`break` 跳出最近一层循环或 `switch`；`continue` 跳过本次循环剩余语句，直接进入下一次条件判断（对 `for` 会先执行 step）。嵌套循环里 `break` 只影响内层；想一次跳出多层，可以用标志变量、把循环封装成函数用 `return`，或使用 `goto`。

`goto` 只能在同一个函数内跳转，不能跳过带初始化的变量声明进入其作用域。它损害可读性，现代 C++ 基本只在"从多层循环统一退出"时偶尔使用。

```cpp
#include <iostream>

int main() {
    int i = 0;
loop:
    if (i < 3) {
        std::cout << i << '\n';
        ++i;
        goto loop;
    }

    for (int k = 0; k < 2; ++k) {
        int local = k * 10;
        std::cout << local << '\n';
    }
}
```

输出：
```text
0
1
2
0
10
```

### 4.6 作用域与生命周期

作用域是名字在源码中可见的范围，生命周期是对象实际存在的时间，两者不是一回事。块作用域变量离开 `{}` 就销毁；`for` 头部声明的变量只在循环体内可见；内层同名变量会遮蔽（shadow）外层变量。`static` 局部变量只初始化一次，生命周期持续到程序结束，但作用域仍限于所在块；全局变量在 `main` 之前构造、之后析构。

### 4.7 常见坑

- ❌ `if (x = 0)` 恒假、`if (x = 1)` 恒真：这是赋值不是比较，编译器可能只给 `-Wparentheses` 警告；✅ 用 `x == 0`，或把常量写左边 `0 == x`。
- ❌ 悬垂 else：`if (a) if (b) f(); else g();` 中的 `else` 属于内层 `if`，不是外层；✅ 给内层或外层加 `{}`，让归属一眼可见。
- ❌ 循环边界写成 `i <= n`：n 个元素的下标是 0 到 n−1，会越界；✅ 用 `for (int i = 0; i < n; ++i)`。
- ❌ `do { ... } while (cond)` 末尾漏分号：编译错；✅ 补上 `;`。
- ❌ 范围 `for` 中删除元素：迭代器失效，UB；✅ 先收集要删的下标，循环结束后统一删除，或用迭代器写法。
- ❌ `switch` 漏 `break` 造成贯穿，或在 `case` 中定义变量未加 `{}`：前者是静默错误，后者可能编译错；✅ 每个分支显式 `break`，需要贯穿时写 `[[fallthrough]];`，变量用 `{}` 限定作用域。

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

## 9. 字符、字符串与 string

C++ 里没有"字符串"这种内建类型：`char` 只是一个 1 字节的小整数，字符串是"一段连续的 char + 结尾的 `'\0'`"这个约定，而 `std::string` 是标准库用这个约定包出来的管理器。这一节把三者串起来：先看 `char` 的算术本质，再看 C 风格字符串为什么危险，最后看 `std::string` 和它的只读视图 `string_view`。

### 9.1 char 是整数，不是"字符"

`sizeof(char) == 1` 是 C++ 唯一硬性保证（1 字节至少 8 位）。`'A'` 的类型就是 `char`，值是 65。所以大小写转换、数字字符转数值，全是整数运算：

常用 ASCII 段必须背下来：

| 范围 | 十进制 | 性质 |
| --- | --- | --- |
| `'0'`–`'9'` | 48–57 | 连续，`c - '0'` 得到数值 |
| `'A'`–`'Z'` | 65–90 | 连续 |
| `'a'`–`'z'` | 97–122 | 连续，与对应大写差 32 |
| `' '` | 32 | 空格 |
| `'\0'` | 0 | 字符串结束符，也叫 NUL |

❌ 常见错误：把 `'0'` 当成 0。`int n = '7';` 得到 55，不是 7。✅ 正确：`int n = '7' - '0';`。

注意：`char` 到底有没有符号是**实现定义**的。x86-64 上 clang/gcc 默认 `signed char`，所以值 128–255 的字节存进 `char` 会变成负数，`c > 0` 这类判断会出错。处理原始字节（编码、二进制）请显式用 `unsigned char` 或 `std::uint8_t`。

```cpp
#include <iostream>

int main() {
    char c = 'A';
    std::cout << c << ' ' << int(c) << '\n';       // 字符与码值
    std::cout << ('a' - 'A') << '\n';              // 大小写间距
    std::cout << char(c - 'A' + 'a') << '\n';      // 转小写
    char d = '7';
    int v = d - '0';
    std::cout << v << ' ' << v * 2 << '\n';
    std::cout << (d == 7) << ' ' << (d == '7') << '\n';
}
```

输出：
```text
A 65
32
a
7 14
0 1
```

最后一行 `d == 7` 是 `false` 而 `d == '7'` 是 `true`——`'7'` 的码值是 55，跟数字 7 完全无关。这类比较写错了编译器不会提醒。

### 9.2 C 风格字符串：`'\0'` 撑起的一切

字符串字面量 `"abc"` 的类型是 `const char[4]`（三个字符 + 一个 `'\0'`）。两条铁律：

- `strlen` 数的是 `'\0'` **之前**的字符数，不含结尾；
- `sizeof` 数的是整个数组的字节数，含结尾。

所以 `sizeof("abc") == 4` 而 `strlen("abc") == 3`。

`strlen`/`strcmp`/`strcpy`/`strcat` 全部靠"扫到 `'\0'` 为止"工作，它们**不接收长度参数，也不检查边界**。给一个没有 `'\0'` 的数组调用 `strlen` 就是越界读（UB）；`strcpy` 写进装不下的缓冲区就是越界写（UB）。C++ 标准库保留它们只是为了兼容，新代码应当用 `std::string`。

```cpp
#include <cstring>
#include <iostream>

int main() {
    char buf[8] = "abc";                       // 剩余字节自动补 0
    std::cout << sizeof(buf) << ' ' << std::strlen(buf) << '\n';
    std::cout << (std::strcmp("abc", "abc") == 0) << '\n';
    std::cout << (std::strcmp("abc", "abd") < 0) << '\n';   // 只看正负号
    std::strcat(buf, "de");                    // 追加，必须自己保证空间够
    std::cout << buf << ' ' << std::strlen(buf) << '\n';
    char raw[3] = {'a', 'b', 'c'};             // 没有 '\0'
    std::cout << sizeof(raw) << '\n';
}
```

输出：
```text
8 3
1
1
abcde 5
3
```

❌ `if (std::strcmp(a, b) == -1)`：`strcmp` 只保证返回**负数/0/正数**，具体值由实现决定，可能是 -1，也可能是 -32。✅ 用 `< 0`、`== 0`、`> 0`。

另一个更隐蔽的坑：`if (a == b)` 当 `a`、`b` 是 `const char*` 时比较的是**地址**，两个内容相同的字面量几乎必然不相等，而且编译器不报警。

### 9.3 std::string 的常用操作

`std::string` 自动管理内存、自动扩容、带长度，`size()` 是 O(1)（不用扫描）。核心 API：

| 操作 | 写法 | 说明 |
| --- | --- | --- |
| 构造 | `std::string s = "hi";` | 从字面量拷贝 |
| 拼接 | `s += "x"; s = a + b;` | 自动扩容 |
| 长度 | `s.size()` / `s.empty()` | O(1) |
| 查找 | `s.find("x")` | 找不到返回 `npos` |
| 截取 | `s.substr(pos, len)` | 返回新 string |
| 比较 | `s1 < s2`, `s1 == s2` | 按字节字典序 |
| 转 C 串 | `s.c_str()` | 返回只读的 `const char*` |

`npos` 的类型是 `std::string::npos`（即 `size_t` 的最大值）。❌ 用 `int` 接 `find` 的返回值，转成 -1 后永远不等于 `npos`。✅ 直接用 `std::size_t` 或 `auto`，比较时和 `std::string::npos` 比。

```cpp
#include <cstring>
#include <iostream>
#include <string>

int main() {
    std::string s = "Hello";
    s += ", world";
    std::cout << s << ' ' << s.size() << '\n';

    std::string t = s.substr(7, 5);
    std::cout << t << '\n';

    std::size_t p = s.find("world");
    std::cout << (p != std::string::npos) << ' ' << p << '\n';
    std::cout << (s.find("xyz") == std::string::npos) << '\n';

    std::cout << (std::string("abc") < std::string("abd")) << '\n';
    std::cout << std::strlen(s.c_str()) << '\n';
}
```

输出：
```text
Hello, world 12
world
1 7
1
1
12
```

`substr(pos, len)` 的两个边界要注意：`len` 超过剩余长度时**自动截断**（不报错）；但 `pos > size()` 会抛 `std::out_of_range`。同理 `s[i]` 越界是 UB（不检查），`s.at(i)` 越界抛异常。期末题里"哪个会检查"是高频考点。

### 9.4 string 与 char[] 对比

| 维度 | `char[]` / `const char*` | `std::string` |
| --- | --- | --- |
| 内存 | 栈上定长，或手动 `new` | 堆上自动管理、自动扩容 |
| 长度 | `strlen` 每次 O(n) | `size()` O(1) |
| 拷贝 | `strcpy`，需自己算空间 | `=` 自动深拷贝 |
| 比较内容 | `strcmp` | `==`、`<` 直接可用 |
| `==` 的语义 | 比较**指针地址** | 比较内容 |
| 拼接 | `strcat`，需预留空间 | `+`、`+=` |
| 可含 `'\0'` | 否（会提前截断） | 是（长度独立记录） |
| 作为参数 | 退化为指针，丢失长度 | 传 `const std::string&` |

一句话总结：`std::string` 把"长度 + 容量 + 缓冲区"打包成不变式，越界和悬垂的空间大幅缩小；`char[]` 只在需要与 C 接口交互、或确实想要定长栈缓冲区时使用。

补充一个常考的细节：`std::string` 允许中间出现 `'\0'`，因为它用独立的 `size_` 记录长度，只把 `'\0'` 当成普通字符。`std::string s("a\0b", 3);` 的 `size()` 是 3。但一旦用 `s.c_str()` 交给 `strlen`、`printf`、`strcmp` 这些 C 函数，它们依旧只认第一个 `'\0'`，看到的是 `"a"`。**C 字符串的能力边界由 `'\0'` 决定，`std::string` 的能力边界由长度决定**，这是两者最本质的差别；也正因如此，`c_str()` 返回的指针只能用于"立即传给 C 函数"，不能长期保存。

### 9.5 string_view：不拥有的只读视图

`std::string_view` 只是"指针 + 长度"两个字段，拷贝它不拷贝字符，构造它也不分配内存。它适合做**只读参数**：`void print(std::string_view s)` 既能接 `std::string`，也能接字面量，且零拷贝。

代价是它**不拥有**数据，因此有悬垂风险：

```cpp-fragment
std::string_view sv = "literal";        // OK：字面量生命周期是静态的
std::string s = "text";
std::string_view v2 = s;                // OK：只要 s 活着
std::string_view bad = std::string("t"); // ❌ 临时 string 在语句结束就销毁，bad 悬垂
```

另外 `string_view` 不保证 `'\0'` 结尾（可能只是某个长字符串的一段），所以**没有 `c_str()`**，不能直接传给 C 函数。

```cpp
#include <iostream>
#include <string>
#include <string_view>

std::string_view first_word(std::string_view sv) {
    std::size_t p = sv.find(' ');
    return p == std::string_view::npos ? sv : sv.substr(0, p);
}

int main() {
    std::string line = "alpha beta gamma";
    std::cout << first_word(line) << '\n';
    std::cout << first_word("literal") << '\n';

    std::string_view sv = line;
    sv.remove_prefix(6);                   // 只移动起点，不复制
    std::cout << sv << ' ' << sv.size() << '\n';
}
```

输出：
```text
alpha
literal
beta gamma 10
```

### 9.6 getline 与空格

`std::cin >> s` 的行为是"跳过前导空白 → 读到下一个空白为止"，所以它读不了带空格的整行；`std::getline(in, line)` 读到 `'\n'` 为止（`'\n'` 被吃掉、不存入）。

混用两者是经典坑：`cin >> n` 之后，输入流停在数字后面，换行符 `'\n'` 还留在缓冲区里；紧接着的 `getline` 立刻遇到这个 `'\n'`，返回**空串**。修法是在两者之间 `std::cin.ignore()`，或者用 `std::getline(std::cin >> std::ws, line)` 先吃掉前导空白。

下面的例子用 `std::istringstream` 提供固定输入，这样不需要人工敲键盘，输出也能逐字重现：

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::istringstream in("42 hello world\nsecond line\n");

    int n = 0;
    in >> n;                                   // 停在 "42" 之后，'\n' 之前
    std::string rest;
    std::getline(in, rest);                    // 读到行尾，含前导空格
    std::cout << n << '[' << rest << ']' << '\n';

    std::string line;
    std::getline(in, line);
    std::cout << '[' << line << ']' << '\n';

    std::istringstream words("a b c");
    std::string w;
    while (words >> w) std::cout << w << '|';
    std::cout << '\n';
}
```

输出：
```text
42[ hello world]
[second line]
a|b|c|
```

第一行输出 `[ hello world]` 里那个前导空格正是"`>>` 不吃行尾空白、`getline` 原样保留"的直接证据。

## 10. 结构体与类基础

`struct`/`class` 是把"一组数据和操作这组数据的函数"绑在一起的机制；构造与析构则是 C++ 区别于 C 的关键——对象在创建和销毁时会自动执行代码，资源的获取与释放可以绑在对象生命周期上（RAII）。这一节先把这些零件逐个讲清楚，重点是初始化列表和"什么时候会调用谁"。

### 10.1 struct 与 class 只差默认访问权限

| 关键字 | 默认成员访问 | 默认继承方式 | 习惯用法 |
| --- | --- | --- | --- |
| `struct` | `public` | `public` | 纯数据聚合、POD |
| `class` | `private` | `private` | 有不变式、有资源 |

除了这两个默认值，两者在语言层面完全等价（`struct` 也能有成员函数、构造、继承、虚函数）。三种访问控制：

| 说明符 | 谁能访问 |
| --- | --- |
| `public` | 任何人 |
| `protected` | 本类与派生类 |
| `private` | 仅本类成员与友元 |

注意：`private` 是**编译期**检查，只影响名字可见性，不影响内存布局——对象里照样有那些字节，只是你没法用名字碰它们。

### 10.2 成员函数与 this

成员函数在内存里只有一份，编译器给它加了一个隐藏参数 `this`，指向调用它的对象。所以 `p.x()` 实际约等于 `Point_x(&p)`。

- `this` 的类型在普通成员函数里是 `Point* const`——指针本身不能改指向，但 `*this` 可改。
- 在 `const` 成员函数里是 `const Point*`，所以不能改成员。
- 链式调用就是 `return *this;`（返回引用，不是拷贝）。

`this` 最常见的三个用途：区分同名参数（`this->x = x;`）、返回自身引用、在 `operator=` 里判断自赋值。

### 10.3 构造与析构：对象的一生

构造函数与类同名、无返回类型、可以重载；析构函数写作 `~T()`、无参数、无返回、不能重载。它们都不是"你调用"的，而是编译器在特定时机插入的。

```
Derived d;                    // Derived : public Base，且有一个成员 m

 1. 分配 sizeof(Derived) 字节          ← 此时内容是随机的
 2. Base()                             ← 基类先构造
 3. m 的构造函数（按声明顺序）          ← 成员构造
 4. Derived() 的函数体                  ← 到这里对象才"可用"
        ...  使用对象  ...
 5. ~Derived() 的函数体                 ← 析构从最外层开始
 6. ~m（声明逆序）                      ← 成员逆序析构
 7. ~Base()                            ← 基类最后析构
```

**构造永远从基类往派生走（基类 → 成员 → 自己），析构永远倒着来。** 这条规则在第 12 节讲继承时会反复用到。

析构的触发时机：局部对象离开作用域、`delete` 一个指针、临时对象所在的完整表达式结束。函数正常 `return` 和提前 `return` 都会触发，所以析构是放"清理代码"的正确位置。

### 10.4 初始化列表：不只是效率问题

语法是构造函数名后面接冒号：

```cpp-fragment
Point(int x, int y) : x_(x), y_(y) { }   // 成员初始化列表
```

关键区别：**列表是初始化，函数体内是赋值。**

| 写法 | 实际发生的事 |
| --- | --- |
| `: x_(x)` | 用 `x` 直接构造 `x_`，一步到位 |
| `{ x_ = x; }` | 先默认构造 `x_`（对类类型要构造一次），再调用 `operator=` |

对 `int` 只差一点效率，但对下面三类成员，**函数体内赋值根本无法编译**：

- `const` 成员：没有赋值运算符；
- 引用成员：必须在创建时绑定，之后不能重新绑定；
- 没有默认构造函数的类类型成员（以及基类）。

```cpp
#include <iostream>

class Sensor {
public:
    Sensor(int id, int& total) : id_(id), total_(total) {   // 两个都必须用列表
        ++total_;
        std::cout << "ctor " << id_ << '\n';
    }
    ~Sensor() { std::cout << "dtor " << id_ << '\n'; }
    int id() const { return id_; }
private:
    const int id_;      // const：只能初始化
    int& total_;        // 引用：只能初始化
};

int main() {
    int count = 0;
    {
        Sensor a(1, count);
        Sensor b(2, count);
        std::cout << "alive=" << count << '\n';
    }
    std::cout << "after=" << count << '\n';
}
```

输出：
```text
ctor 1
ctor 2
alive=2
dtor 2
dtor 1
after=2
```

注意 `count` 是通过引用共享的计数器：两个对象都加到了同一个变量上；析构时没有减它，所以 `after=2`。

⚠️ **顺序坑**：成员按**声明顺序**初始化，与你在列表里写的顺序无关。下面这种写法看起来是先算 `b` 再算 `a`，实际上 `a_` 声明在前、先初始化，`b_(a_ * 2)` 读到的是**未初始化**的 `a_`：

```cpp-fragment
struct Bad {
    int a_;          // 声明在前 → 先初始化
    int b_;
    Bad(int x) : b_(a_ * 2), a_(x) { }   // ❌ 读未初始化的 a_
};
```

`clang++ -Wall` 会对列表顺序与声明顺序不一致给出 `-Wreorder-ctor` 警告，对"用未初始化成员初始化另一个成员"给出 `-Wuninitialized` 警告——但只有开了警告才看得见，所以规则要自己记住。

### 10.5 默认构造函数

**只要声明了任何一个构造函数，编译器就不再隐式生成默认构造函数。** 这是最高频的编译错误来源：

```cpp-fragment
class Point {
public:
    Point(int x, int y) : x_(x), y_(y) {}
private:
    int x_ = 0, y_ = 0;
};

Point p;        // ❌ 编译错：没有默认构造函数
Point q(1, 2);  // ✅
```

补救两种写法：`Point() = default;`（显式要回编译器生成的版本）或者给参数加默认值 `Point(int x = 0, int y = 0)`——后者同时充当默认构造。

还要区分两个看起来一样的语句：

| 写法 | 对类类型 | 对内建类型成员 |
| --- | --- | --- |
| `T t;` | 调用默认构造 | **不初始化**（值不确定） |
| `T t{};` | 调用默认构造 | 零初始化 |

### 10.6 static 成员

`static` 数据成员属于**类**而不属于对象，所有对象共享同一份；`static` 成员函数没有 `this`，因此不能访问非静态成员，也不能是 `const`。

```cpp-fragment
class Counter {
public:
    static int created() { return created_; }   // 无 this
private:
    static int created_;                         // 声明
};
int Counter::created_ = 0;                       // 定义（类外，只能一处）
```

C++17 起可以写成 `inline static int created_ = 0;` 直接在类内定义，省掉类外那一行。注意 `static` 成员函数里写 `x_` 会直接编译错——不是"访问到某个对象"，而是根本没有对象。

### 10.7 const 成员函数

在参数表后面加 `const`：

```cpp-fragment
int size() const { return size_; }
```

它把 `this` 变成 `const T*`，承诺"不修改对象状态"。作用有两个：

1. `const` 对象、`const` 引用、`const` 指针**只能**调用 `const` 成员函数；
2. 它是对调用者的契约，编译器替你检查。

❌ 给 `const` 对象调用非 `const` 成员函数会编译错。✅ 把只读的成员函数都标上 `const`；确实要在 `const` 函数里改的成员（缓存、计数器）声明为 `mutable`。

`const` 与非 `const` 版本可以重载，编译器按对象的 const 性质选：

```cpp-fragment
const int& at(int i) const { return data_[i]; }   // const 对象选中这个
int&       at(int i)       { return data_[i]; }   // 非 const 对象选中这个
```

### 10.8 友元简介

`friend` 写在类里，授予某个函数或类访问私有成员的权限：

```cpp-fragment
class Vec {
    friend std::ostream& operator<<(std::ostream&, const Vec&);
    friend class Inspector;          // 整个类都能看
private:
    double x_ = 0, y_ = 0;
};
```

三条性质：友元关系**不能继承**、**不能传递**（朋友的朋友不是朋友）、单向（我允许你进来，不代表我能进你那里）。它破坏封装，所以只在"运算符重载必须访问内部表示"这类场合用，最典型的就是 `operator<<`。

### 10.9 聚合初始化

**聚合**（aggregate）大致要求：所有非静态数据成员都是 `public`、没有用户声明的构造函数、没有虚函数、没有 `private`/`protected` 基类。对聚合可以按声明顺序花括号初始化：

```cpp-fragment
struct Point { int x; int y; };
Point p{1, 2};
```

C++20 的**指定初始化器**允许写成员名，可读性更好，但有硬性限制：必须按声明顺序、不能跳过中间成员再回头、不能和位置初始化混用。

### 10.10 常见坑

**坑 1：未初始化的成员。** `struct S { int a; }; S s; std::cout << s.a;` 输出的是**不确定值**，不是 0。聚合体 `S s{};` 或给成员写默认初始化器 `int a = 0;` 才能保证。

**坑 2：返回局部对象的引用。**

```cpp-fragment
const std::string& bad() {
    std::string local = "tmp";
    return local;              // ❌ 函数返回后 local 已析构，引用悬垂
}
```

返回引用/指针的前提是**被引用对象的生命周期长于调用者**：返回成员、返回 `static` 变量、返回参数里传进来的对象都可以，返回局部变量一定错。`-Wall` 会给 `-Wreturn-stack-address` 警告。

**坑 3：以为列表顺序决定初始化顺序**（见 10.4）。

**坑 4：在构造函数里调用虚函数**，此时派生类部分还没构造，只会调用到当前层的版本（第 12 节展开）。

下面是一个把这些零件拼起来的完整例子：聚合初始化、默认成员初始化器、`static` 成员、`const` 成员函数同时出现。

```cpp
#include <iostream>
#include <string>

struct Config {                 // 聚合：可以花括号初始化
    std::string name;
    int port = 8080;            // 默认成员初始化器
    bool verbose = false;
};

class Registry {
public:
    static int count() { return count_; }        // static：无 this
    void add(const std::string& n) {             // 非 const：会改 state_
        ++count_;
        state_ += n.size();
    }
    int state() const { return state_; }         // const：只读
private:
    int state_ = 0;
    static int count_;
};

int Registry::count_ = 0;

int main() {
    Config a{"server"};                    // 其余成员用默认值
    Config b{};                            // name 空、port 仍是 8080
    Config c{.name = "db", .port = 5432};  // 指定初始化器，按声明顺序
    std::cout << a.name << ' ' << a.port << ' ' << a.verbose << '\n';
    std::cout << '[' << b.name << "] " << b.port << '\n';
    std::cout << c.name << ' ' << c.port << '\n';

    Registry r;
    r.add("abc");
    r.add("de");
    const Registry& cr = r;                // const 引用只能调 const 成员
    std::cout << Registry::count() << ' ' << cr.state() << '\n';
}
```

输出：
```text
server 8080 0
[] 8080
db 5432
2 5
```

`b{}` 里 `name` 是空串、`port` 仍然是 8080——这证明默认成员初始化器在花括号初始化时同样生效（`{}` 只是没给值，不是把成员清零）。`cr.state()` 能编译而 `cr.add(...)` 不能，正是 `const` 成员函数在起作用。

## 11. 拷贝、移动与运算符重载

一个对象从"另一个同类型对象"被创建（拷贝构造）或被赋值（拷贝赋值）时，编译器默认会帮你生成逐成员的版本。这在成员都是值时完全正确，但只要类里有一个"拥有资源"的指针，默认版本就会让两个对象指向同一块内存——析构两次，程序直接崩。这一节讲清楚三个特殊成员函数、深浅拷贝、移动语义，以及运算符重载的返回值约定。

### 11.1 拷贝是"逐成员"的

编译器会隐式生成：拷贝构造函数、拷贝赋值运算符、析构函数（前提是你没自己声明）。它们的默认行为都是**对每个成员调用对应的拷贝/析构**，指针成员只拷贝指针值，不拷贝指向的内容。

```
浅拷贝（默认行为）                     深拷贝（手写）

a: { p -> [h e l l o \0] }             a: { p -> [h e l l o \0] }
b: { p -> ──────┘        }             b: { p -> [h e l l o \0] }  ← 新分配

析构时 delete p 两次 → double free (UB)   各删各的，互不影响
改 b 的内容，a 也跟着变                   改 b 不影响 a
```

`double free` 是 UB，可能当场崩溃、可能静默损坏堆、也可能在小程序里"看起来正常"——这是它最可怕的地方。

### 11.2 Rule of Three

**如果你需要自己写析构函数（说明你在管理资源），那么拷贝构造和拷贝赋值几乎肯定也需要自己写。** 反过来，如果三个都不需要，就一个都别写。

| 函数 | 签名 | 何时被调用 |
| --- | --- | --- |
| 拷贝构造 | `T(const T& other)` | `T b = a;`、按值传参、按值返回 |
| 拷贝赋值 | `T& operator=(const T& other)` | `b = a;`（`b` 已存在） |
| 析构 | `~T()` | 生命周期结束、`delete` |

两个细节：拷贝构造的参数**必须是引用**，写成 `T(T other)` 会因为按值传参又要拷贝、无限递归，编译器直接报错；拷贝赋值要返回 `T&` 以支持 `a = b = c`。

### 11.3 移动：把资源"偷"过来

拷贝一个持有 1MB 缓冲区的对象要复制 1MB，但如果这个源对象马上就要销毁（临时对象、`return` 的局部变量），复制纯属浪费——直接把指针拿过来、把源置空就行。这就是移动语义。

右值引用 `T&&` 用来识别"可以被掏空的源"。移动构造/移动赋值把源对象的指针搬到自己身上，并把源改成空状态，这样源析构时 `delete nullptr` 是安全的。

```
移动前                              移动后
a: { n=5, p -> [buf] }              a: { n=0, p = nullptr }
b: { n=0, p = nullptr }             b: { n=5, p -> [buf] }

没有复制任何字符，只是两个字段的搬移；a 变成"空壳"，只能析构或重新赋值。
```

`std::move(x)` **不移动任何东西**，它只是一个 `static_cast<T&&>(x)`，把左值标记成右值，好让重载决议选中移动版本。真正搬资源的是移动构造/移动赋值里的代码。

⚠️ 被移动之后的对象处于"有效但未指定"状态：可以析构、可以重新赋值，但不要读它的值（比如 `cout << a.size()`）来推断业务含义。

### 11.4 Rule of Five 与 = default / = delete

加上移动构造、移动赋值就是 Rule of Five：

```cpp-fragment
class Buf {
public:
    Buf(const Buf&) = delete;              // 禁止拷贝
    Buf& operator=(const Buf&) = delete;
    Buf(Buf&&) noexcept = default;         // 要默认的移动
    Buf& operator=(Buf&&) noexcept = default;
    ~Buf() = default;
};
```

- `= default`：显式要编译器生成（常用于"我想保留默认行为，但必须写出来"的场合）。
- `= delete`：显式禁用。被删除的函数参与重载决议，所以报错信息是"调用了被删除的函数"，比"找不到函数"清楚得多。
- 移动操作应当标 `noexcept`，否则 `std::vector` 扩容时会退回复制——因为它要保证强异常安全。

🚨 **隐形陷阱**：只要你自己声明了移动构造/移动赋值（哪怕写的是 `= default`），编译器就**不再生成**拷贝构造和拷贝赋值，它们被隐式删除。所以"我只想要移动"的类，必须在五件套里明确写出你要哪些、删掉哪些。

### 11.5 运算符重载：成员还是非成员

| 运算符 | 建议形式 | 返回类型 | 理由 |
| --- | --- | --- | --- |
| `+=`、`-=` | 成员 | `T&`（`*this`） | 左操作数就是对象，且要能链式 |
| `+`、`-` | 非成员 | `T`（新值） | 左右对称，允许隐式转换 |
| `==`、`<` | 非成员（或 `<=>`） | `bool` | 对称 |
| `<<`、`>>` | **必须**非成员 | `std::ostream&` | 左操作数是流，不是你 |
| `=`、`[]`、`()` | 必须成员 | — | 语言规定 |

`<<` 必须是非成员，因为它的左操作数是 `std::ostream`，你没法给标准库的类加成员函数——所以通常配 `friend` 或提供公有 getter。

❌ 不要重载 `&&`、`||`、`,`：重载后**失去短路求值**和规定的求值顺序，代码会静默改变行为。

### 11.6 自赋值

`a = a;` 看着荒谬，但 `v[i] = v[j]` 在 `i == j` 时就发生了。如果 `operator=` 的写法是"先 `delete` 旧资源，再从参数拷贝"，自赋值时参数已经被删掉了，读到的是已释放内存。

✅ 两种写法都行：开头 `if (this == &other) return *this;`，或者"先分配新的、成功后再释放旧的"（后者顺便提供了强异常安全）。

### 11.7 完整例子：手写 Rule of Three 的字符串类

```cpp
#include <cstring>
#include <iostream>

class Str {
public:
    Str(const char* s = "") : n_(std::strlen(s)), p_(new char[n_ + 1]) {
        std::memcpy(p_, s, n_ + 1);
        std::cout << "ctor " << p_ << '\n';
    }
    Str(const Str& o) : n_(o.n_), p_(new char[o.n_ + 1]) {   // 深拷贝
        std::memcpy(p_, o.p_, n_ + 1);
        std::cout << "copy " << p_ << '\n';
    }
    Str& operator=(const Str& o) {
        if (this == &o) return *this;          // 自赋值：直接返回
        char* np = new char[o.n_ + 1];          // 先分配，失败也不会破坏自己
        std::memcpy(np, o.p_, o.n_ + 1);
        delete[] p_;
        p_ = np;
        n_ = o.n_;
        std::cout << "assign " << p_ << '\n';
        return *this;
    }
    ~Str() { delete[] p_; }

    void set_first(char c) { p_[0] = c; }
    const char* c_str() const { return p_; }
    std::size_t size() const { return n_; }
private:
    std::size_t n_;
    char* p_;
};

void assign_copy(Str& dst, const Str& src) { dst = src; }

int main() {
    Str a("hello");
    Str b = a;                 // 拷贝构造
    b.set_first('J');
    std::cout << a.c_str() << ' ' << b.c_str() << '\n';

    Str c("x");
    c = a;                     // 拷贝赋值
    std::cout << c.c_str() << ' ' << c.size() << '\n';

    assign_copy(c, c);         // 两个引用可能指向同一个对象 → 自赋值
    std::cout << c.c_str() << '\n';
}
```

输出：
```text
ctor hello
copy hello
hello Jello
ctor x
assign hello
hello 5
hello
```

`hello Jello` 这一行是深拷贝的证据：修改 `b` 的第一个字符，`a` 完全没变。如果漏写拷贝构造，这一行会变成 `Jello Jello`，而程序结束时两次 `delete[]` 同一块内存——崩溃只是时间问题。

### 11.8 移动构造/移动赋值

```cpp
#include <iostream>
#include <utility>

class Buf {
public:
    explicit Buf(std::size_t n) : n_(n), p_(new int[n]) {
        for (std::size_t i = 0; i < n_; ++i) p_[i] = static_cast<int>(i);
        std::cout << "ctor n=" << n_ << '\n';
    }
    Buf(Buf&& o) noexcept : n_(o.n_), p_(o.p_) {      // 偷资源
        o.n_ = 0;
        o.p_ = nullptr;
        std::cout << "move-ctor n=" << n_ << '\n';
    }
    Buf& operator=(Buf&& o) noexcept {
        if (this == &o) return *this;
        delete[] p_;
        n_ = o.n_;
        p_ = o.p_;
        o.n_ = 0;
        o.p_ = nullptr;
        std::cout << "move-assign n=" << n_ << '\n';
        return *this;
    }
    ~Buf() { delete[] p_; }
    std::size_t size() const { return n_; }
private:
    std::size_t n_;
    int* p_;
};

int main() {
    Buf a(4);
    Buf b = std::move(a);              // 移动构造
    std::cout << "a=" << a.size() << " b=" << b.size() << '\n';

    Buf c(1);
    c = std::move(b);                  // 移动赋值
    std::cout << "b=" << b.size() << " c=" << c.size() << '\n';
}
```

输出：
```text
ctor n=4
move-ctor n=4
a=0 b=4
ctor n=1
move-assign n=4
b=0 c=4
```

`a=0 b=4` 说明资源确实转移了、源对象被置空。注意移动构造函数里**没有** `new`：`n=4` 那块缓冲区自始至终只分配一次，后面只是换了主人——没有复制任何 `int`。

### 11.9 运算符重载实战

```cpp
#include <iostream>

class Vec2 {
public:
    Vec2(double x = 0, double y = 0) : x_(x), y_(y) {}

    Vec2& operator+=(const Vec2& o) {        // 成员：返回 *this
        x_ += o.x_;
        y_ += o.y_;
        return *this;
    }
    double x() const { return x_; }
    double y() const { return y_; }
private:
    double x_, y_;
};

Vec2 operator+(Vec2 a, const Vec2& b) { return a += b; }   // 复用 +=

bool operator==(const Vec2& a, const Vec2& b) {
    return a.x() == b.x() && a.y() == b.y();
}

std::ostream& operator<<(std::ostream& os, const Vec2& v) { // 必须非成员
    return os << '(' << v.x() << ',' << v.y() << ')';
}

int main() {
    Vec2 a(1, 2), b(3, 4);
    Vec2 c = a + b;
    std::cout << a << " + " << b << " = " << c << '\n';

    a += b;
    std::cout << a << ' ' << (a == c) << ' ' << (a == b) << '\n';
}
```

输出：
```text
(1,2) + (3,4) = (4,6)
(4,6) 1 0
```

`operator+` 按值收第一个参数并返回 `a += b` 的结果：按值收参允许 `Vec2 + 字面量` 这类隐式转换，返回新值符合"加法不改变操作数"的语义。`operator<<` 返回 `os` 的引用，才能串成 `cout << a << b << c`。

## 12. 继承与多态

继承让派生类复用基类的接口与实现；多态让"通过基类指针/引用调用"在运行时分派到实际类型。这两件事是现代 C++ 面向对象的基础，也是期中卷面上错误率最高的部分——因为它的坑几乎全是"编译通过、运行结果不对"。

### 12.1 公有继承表达"是一种"

`class Derived : public Base` 表达 is-a：`Derived` **就是**一个 `Base`，所以任何接受 `Base&`/`Base*` 的地方都能传 `Derived`。

| 继承方式 | 基类 public 成员在派生类中变成 |
| --- | --- |
| `public` | `public` |
| `protected` | `protected` |
| `private` | `private` |

`class` 的默认继承是 `private`，`struct` 是 `public`——写 `class Dog : Animal` 会得到一个"实现继承"，外部无法把 `Dog*` 当 `Animal*` 用。这是很隐蔽的编译错误来源，**永远显式写 `public`**。

继承是为了"接口兼容"，不是为了复用代码。如果只是想要基类的实现而不满足 is-a（比如 `Stack` 想复用 `Vector` 的代码），用**组合**（把 `Vector` 作为成员），否则会得到"栈可以按下标乱改"这种语义错误。

### 12.2 构造与析构顺序

派生类对象里包含一个完整的基类子对象，所以构造必须先把它建好：

```
new Dog(3)                      // Dog : public Animal
 1. Animal()                    ← 基类先构造（没有默认构造就必须在列表里显式调）
 2. Dog 的成员按声明顺序构造
 3. Dog() 的函数体
        ...  使用  ...
 4. ~Dog() 的函数体
 5. Dog 的成员逆序析构
 6. ~Animal()                   ← 基类最后析构
```

构造函数不能"继承"，但 C++11 起可以 `using Base::Base;` 把基类构造函数引入派生类。派生类构造函数若没显式调用基类构造，编译器会尝试调用 `Base()`；基类没有默认构造时**必须**写 `Derived(args) : Base(args) {}`。

```cpp
#include <iostream>

struct Base {
    Base() { std::cout << "1. Base()\n"; }
    ~Base() { std::cout << "6. ~Base()\n"; }
};

struct Member {
    Member() { std::cout << "2. Member()\n"; }
    ~Member() { std::cout << "5. ~Member()\n"; }
};

struct Derived : Base {
    Member m;
    Derived() { std::cout << "3. Derived()\n"; }
    ~Derived() { std::cout << "4. ~Derived()\n"; }
};

int main() {
    {
        Derived d;
        std::cout << "-- in scope --\n";
    }
    std::cout << "-- out of scope --\n";
}
```

输出：
```text
1. Base()
2. Member()
3. Derived()
-- in scope --
4. ~Derived()
5. ~Member()
6. ~Base()
-- out of scope --
```

编号就是执行顺序：构造从基类往派生走、析构从派生往基类走，严格镜像。这条规则保证"派生类析构时基类部分还活着"，所以派生类析构函数里可以安全使用基类的成员。

### 12.3 虚函数与 override

普通成员函数在**编译期**按静态类型绑定（早绑定）；`virtual` 函数在**运行期**按对象的动态类型分派（晚绑定）。实现方式是每个多态对象里藏一个 `vptr`，指向类的虚函数表：

```
Dog 对象                    Dog 的 vtable
┌──────────────┐           ┌──────────────────┐
│ vptr ────────┼──────────>│ &Dog::speak      │
│ data         │           │ &Dog::name       │
└──────────────┘           └──────────────────┘
Animal* p = new Dog;  p->speak()  →  查 vptr → 调 Dog::speak
```

`override` 不是必需的，但**必须写**：它让编译器检查"我确实在覆盖一个基类虚函数"。漏写 `const`、参数类型写错、基类那个函数忘了 `virtual`——这些错误在没有 `override` 时会变成"定义了新函数、悄悄隐藏基类版本"，程序照常编译，行为错得莫名其妙。

| 写法 | 效果 |
| --- | --- |
| `virtual void f();` | 声明虚函数，派生类可覆盖 |
| `void f() override;` | 覆盖并让编译器校验签名 |
| `void f() final;` | 禁止再被覆盖 |
| `virtual void f() = 0;` | 纯虚，派生类必须实现 |

```cpp
#include <iostream>
#include <string>
#include <vector>

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string name() const { return "animal"; }
    virtual std::string sound() const { return "..."; }
};

class Dog : public Animal {
public:
    std::string name() const override { return "dog"; }
    std::string sound() const override { return "woof"; }
};

class Cat : public Animal {
public:
    std::string name() const override { return "cat"; }
    std::string sound() const override { return "meow"; }
};

void speak(const Animal& a) {          // 引用：多态生效
    std::cout << a.name() << " says " << a.sound() << '\n';
}

int main() {
    std::vector<Animal*> zoo;
    zoo.push_back(new Dog);
    zoo.push_back(new Cat);
    zoo.push_back(new Animal);

    for (const Animal* p : zoo) speak(*p);

    for (Animal* p : zoo) delete p;    // 虚析构：每个都能正确销毁
}
```

输出：
```text
dog says woof
cat says meow
animal says ...
```

`speak` 只有一个版本，参数是 `const Animal&`，却调到了三个不同的 `sound()`——这就是多态。如果 `speak` 的参数写成 `Animal a`（按值），立刻退化成 12.6 的切片问题。

### 12.4 纯虚函数与抽象类

`= 0` 的虚函数叫纯虚函数，含纯虚函数的类是**抽象类**：不能创建对象，只能作为接口被继承；派生类必须实现全部纯虚函数才能实例化。

```cpp
#include <iostream>

class Shape {
public:
    explicit Shape(const char* n) : name_(n) {}
    virtual ~Shape() { std::cout << "~Shape(" << name_ << ")\n"; }
    virtual double area() const = 0;            // 纯虚 → Shape 是抽象类
    const char* name() const { return name_; }
private:
    const char* name_;
};

class Square : public Shape {
public:
    explicit Square(double s) : Shape("square"), s_(s) {}
    ~Square() override { std::cout << "~Square\n"; }
    double area() const override { return s_ * s_; }
private:
    double s_;
};

int main() {
    Shape* p = new Square(3);            // 基类指针指向派生对象
    std::cout << p->name() << ' ' << p->area() << '\n';
    delete p;                            // 虚析构：~Square 先，~Shape 后
    // Shape s;                          // ❌ 编译错：抽象类无法实例化
}
```

输出：
```text
square 9
~Square
~Shape(square)
```

### 12.5 为什么基类析构必须是虚的

`delete p;` 时如果 `p` 的静态类型是 `Base*` 而 `~Base` **不是**虚函数，编译器只会调用 `~Base`，派生类的那部分成员根本没有析构——标准里这是**未定义行为**，实践中表现为派生类持有的资源泄漏。

```cpp-fragment
class Bad {
public:
    ~Bad() {}                    // ❌ 非虚
};
class BadDerived : public Bad {
public:
    ~BadDerived() { delete[] buf_; }   // 永远不会被调用
private:
    int* buf_ = new int[100];
};

Bad* p = new BadDerived;
delete p;                        // UB：只执行 ~Bad，buf_ 泄漏
```

规则很简单：**一个类只要可能被通过基类指针 `delete`，析构函数就必须是 `virtual`。** 基类析构写成虚的之后，派生类的析构自动是虚的。代价是每个对象多一个 `vptr`，所以纯数据类（不会被多态删除）不要无脑加虚析构。

### 12.6 对象切片

把派生类对象**按值**赋给基类对象（或按值传参）时，只复制基类子对象，派生类新增的成员被丢弃、`vptr` 也换成了 `Base` 的——这叫切片（slicing）。

```cpp
#include <iostream>

class Base {
public:
    virtual ~Base() = default;
    virtual const char* who() const { return "Base"; }
};

class Derived : public Base {
public:
    const char* who() const override { return "Derived"; }
};

void byValue(Base b) { std::cout << "byValue  " << b.who() << '\n'; }
void byRef(const Base& b) { std::cout << "byRef    " << b.who() << '\n'; }

int main() {
    Derived d;
    std::cout << "object   " << d.who() << '\n';

    Base sliced = d;                          // 切片：只拷 Base 子对象
    std::cout << "sliced   " << sliced.who() << '\n';

    byValue(d);                               // 按值传参同样切片
    byRef(d);                                 // 按引用不切片

    Base* p = &d;                             // 指针保留动态类型
    std::cout << "ptr      " << p->who() << '\n';

    std::cout << "cast     " << (dynamic_cast<Derived*>(p) != nullptr) << '\n';
    std::cout << "castbase " << (dynamic_cast<Derived*>(&sliced) != nullptr) << '\n';
}
```

输出：
```text
object   Derived
sliced   Base
byValue  Base
byRef    Derived
ptr      Derived
cast     1
castbase 0
```

`sliced` 的 `who()` 返回 `Base`，因为它的 `vptr` 已经指向 `Base` 的虚表——切片是**合法的、静默的**类型退化，编译器不会报警。最后两行是 `dynamic_cast` 的用途：`p` 指向的确实是 `Derived`，转换成功；`&sliced` 只是一块 `Base`，转换返回 `nullptr`。**多态只能通过指针或引用获得，值传递一定丢类型。**

`dynamic_cast` 要点：只对有虚函数的类型有效；指针版失败返回 `nullptr`，引用版失败抛 `std::bad_cast`；它比 `static_cast` 安全但慢，能用虚函数解决的问题不要用它。

### 12.7 名字隐藏与 using

派生类里声明一个与基类**同名**的函数，会**隐藏**基类那一个名字下的**全部重载**——不是覆盖，也不是重载。注意它跟 `virtual` 是两套机制：这里讲的是普通名字查找，跟虚函数分派无关。

```cpp
#include <iostream>

class Base {
public:
    void f(int x) { std::cout << "Base::f(int) " << x << '\n'; }
    void f(double x) { std::cout << "Base::f(double) " << x << '\n'; }
};

class Derived : public Base {
public:
    void f(int x) { std::cout << "Derived::f(int) " << x << '\n'; }
};

class Derived2 : public Base {
public:
    using Base::f;                    // 把 Base 的两个重载拉回作用域
    void f(int x) { std::cout << "Derived2::f(int) " << x << '\n'; }
};

int main() {
    Derived d;
    d.f(1);          // 精确匹配 Derived::f(int)
    double v = 2.5;
    d.f(v);          // Base::f(double) 被隐藏，double 隐式转成 int

    Derived2 d2;
    d2.f(1);         // Derived2::f(int)
    d2.f(v);         // Base::f(double) 回来了
}
```

输出：
```text
Derived::f(int) 1
Derived::f(int) 2
Derived2::f(int) 1
Base::f(double) 2.5
```

`d.f(v)` 打印的是 `2` 而不是 `2.5`，因为 `Base::f(double)` 被隐藏了，`2.5` 只能隐式转成 `int` 去匹配 `Derived::f(int)`——**静默的精度丢失**。加一行 `using Base::f;` 就恢复正常重载决议。

### 12.8 常见坑清单

1. **基类析构非虚 + 通过基类指针 delete** → 派生析构不执行，UB（12.5）。
2. **对象切片** → 多态静默失效，编译器不报警（12.6）。
3. **忘记 `override`** → 签名差一个 `const` 就变成"新函数"，不报错但不多态。
4. **构造函数/析构函数里调用虚函数**：构造期间派生类部分还没建好，虚调用只到达当前正在构造的那一层；析构期间同理，`~Base()` 里调虚函数只会调 `Base` 版本。所以构造/析构里不要指望多态。
5. **默认继承是 private**（`class D : B`），外部无法隐式转换成 `Base*`。
6. **`= 0` 的纯虚函数仍可以有定义**（`virtual void f() = 0;` 之后在类外写 `void Base::f() {}`），派生类可以显式调用它，但通常没必要。
7. **不要为了复用代码而公有继承**：不满足 is-a 时用组合，否则基类的全部接口都会漏给使用者。

记住一条主线：**虚函数决定"调用谁"，虚析构决定"析构谁"，而这一切只有通过指针或引用才生效。**

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
