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
