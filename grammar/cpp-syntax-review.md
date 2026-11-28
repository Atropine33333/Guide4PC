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

一个 C++ 程序从源码到运行要经过预处理、编译、汇编、链接四步；理解这条流水线，才能分清"编译错误"和"链接错误"，也才知道头文件里为什么通常只放声明。本节把 `main` 的写法、声明与定义的分工、`cin`/`cout` 的细节串起来讲，因为期中考试的选择题、改错题和"输出是什么"几乎都落在这几处。

### 1.1 最小程序、main 与命名空间

C++ 源文件（source file）由预处理指令、注释、声明和定义组成。编译器真正处理的是预处理之后的**翻译单元**（translation unit）：`#include` 的内容被原地展开，宏被替换，条件编译被裁剪，最后得到一个大文件。所以头文件里写下的每个名字，都会原样出现在每一个包含它的源文件里——这是后面单一定义规则（ODR）所有麻烦的根源。

每个托管（hosted）程序必须有且只有一个 `main` 函数，它是运行时的入口。标准只认可两种形式：`int main()` 和 `int main(int argc, char* argv[])`（等价写法 `char** argv`）。返回类型必须是 `int`：0 或 `EXIT_SUCCESS` 表示成功，非 0 表示失败。`main` 是唯一允许"执行到末尾而不写 return"的函数，此时效果等于 `return 0`；但显式写出更清楚，也能避免初学者的犹豫。还有一条容易忽略的规则：程序内部不能调用 `main`，不能递归，也不能取它的地址再调用，它不是一个普通函数。至于 `void main()`，那是 C 语言教材里流传下来的错误写法，clang 会直接报 `'main' must return 'int'`。

`#include <iostream>` 是预处理指令，把标准库输入输出的声明引入当前翻译单元。`std::cout`、`std::cin`、`std::endl` 都住在命名空间 `std` 里。`using namespace std;` 会把整个命名空间的名字"倒"进当前作用域，写起来省事，但头文件中绝对不要这么干：所有包含该头文件的源文件都会被污染，`count`、`distance`、`data` 这类常见名字很容易与标准库冲突。更稳的做法是写全 `std::`，或者只引入真正用到的名字。

```cpp
#include <iostream>

using std::cout;                    // 只引入一个名字，影响面最小

int main() {
    cout << "Hello" << ", C++20!\n"; // << 返回流本身，可以连续输出
}
```
输出：
```text
Hello, C++20!
```

`<<` 是左结合的：`cout << "Hello" << ", C++20!"` 等价于 `(cout << "Hello") << ", C++20!"`。每次插入都返回同一个流对象的引用，所以才能这样链式书写；这一点在讲运算符重载时会再次遇到。

命名空间的价值在名字冲突时最明显。`mine::value` 叫限定名（qualified name）；`using mine::value;` 是 using 声明，只把一个名字引入当前作用域，写在函数里就只在函数内有效；`using namespace mine;` 是 using 指令，把整个命名空间的所有名字都变成可见的候选。前者精确、影响面小，后者方便但容易在大型程序里制造歧义，这就是"头文件里别写 `using namespace std;`"的实际原因。

```cpp
#include <iostream>

namespace mine {
    int value = 7;
}

int main() {
    std::cout << mine::value << '\n';   // 限定名，任何时候都能用
    using mine::value;                  // 只引入这一个名字
    std::cout << value + 1 << '\n';
}
```
输出：
```text
7
8
```

### 1.2 四个阶段与两类错误

从 `hello.cpp` 到可执行文件，工具链做了四件事，每件事都可能失败，而失败信息的样子完全不同：

| 阶段 | 做什么 | 产物 | 典型错误 |
| --- | --- | --- | --- |
| 预处理 | 展开 `#include` 和宏，处理 `#if` | 翻译单元 | 找不到头文件：`'xxx.h' file not found` |
| 编译 | 语法分析、类型检查、生成目标代码 | `.o` / `.obj` | `expected ';'`、`use of undeclared identifier` |
| 汇编 | 把汇编代码变成机器码 | 目标文件 | 一般由编译器内部处理，很少单独出错 |
| 链接 | 合并目标文件和库，解析外部符号 | `.exe` / `a.out` | `undefined reference` / `undefined symbol` |

两类错误的本质区别是：**编译错误说明某个源文件内部不合法；链接错误说明每个文件单独看都合法，但拼起来缺东西或重复了**。所以看到 `undefined reference to 'square(int)'`，不要去改调用处的语法，而要找 `square` 的定义在哪个 `.o` 里、有没有被链接进来。本机 Windows 上 clang 使用 lld，报错写作 `ld.lld: error: undefined symbol: square(int)`；Linux 上 GNU ld 写作 `undefined reference to 'square(int)'`，两者说的是同一件事。

本讲义统一用 `clang++ -std=c++20 -Wall -O0 hello.cpp -o hello` 编译。`-std=c++20` 选定语言版本，`-Wall` 打开一组常用警告（未使用变量、可疑的移位、隐式符号比较等），`-O0` 关闭优化，方便调试并保证示例行为与源码一一对应。`-c` 只编译不链接，`-E` 只输出预处理结果，`-S` 输出汇编，`-o` 指定产物名；理解这些开关，就能在考试或作业里单独复现"编译过了但链接失败"的现象。注意 `-Wall` 并不包含所有警告，`-Wextra` 才会补上更多，但本讲义要求 0 warning，示例都按这个标准验证过。

读编译错误有一条铁律：**从第一条 error 看起，后面的错误往往是它的连锁反应**。例如漏写分号，编译器常在下一行报 `expected ';'`，真正要改的是上一行末尾；漏写 `#include` 时，报错是 `use of undeclared identifier 'cout'; did you mean 'std::cout'?`，clang 甚至会把建议直接写出来。模板报错会展开很长的实例化栈，仍然先看第一行和最后一个 "required from here"。

```cpp-fragment
#include <iostream>

int main() {
    cout << "hi\n";        // 错误：没有 using，也没有写 std::
}
```
clang 的原始报错是 `error: use of undeclared identifier 'cout'; did you mean 'std::cout'?`，并附上 `note: 'std::cout' declared here`。改法有两种：写 `std::cout`，或在 `main` 之前写 `using std::cout;`。注意报错行号指向使用处，而不是 `#include` 处。

```cpp-fragment
void main() { }            // 错误：main 必须返回 int
```
报错为 `error: 'main' must return 'int'`。标准要求返回 `int`，因为返回值要交给操作系统当作退出码。

### 1.3 声明、定义与单一定义规则

**声明**（declaration）只告诉编译器"这个名字存在、类型是什么"；**定义**（definition）则真正分配存储或给出函数体。变量 `extern int x;` 是声明，`int x;` 是定义；函数 `int f(int);` 是声明，带函数体的 `int f(int x) { return x; }` 是定义。同一个函数可以声明很多次，但整个程序只能定义一次，这就是单一定义规则（One Definition Rule，ODR）。ODR 允许的例外是 `inline` 函数、模板、类定义和 `constexpr` 变量——它们可以在多个翻译单元里出现相同的定义，链接器负责合并。

于是头文件的正确用法就清楚了：放声明、类定义、模板、`inline` 函数；普通函数的定义放进一个 `.cpp`。头文件还要用 `#pragma once` 或 include guard 防止同一个翻译单元重复包含。下面这个例子故意把 `square` 的定义放到 `main` 之后，靠前面的声明让调用点通过编译：

```cpp
#include <iostream>

int square(int x);          // 声明：先告诉编译器“有这个函数”

int main() {
    std::cout << square(7) << '\n';
}

int square(int x) {         // 定义：整个程序只此一份
    return x * x;
}
```
输出：
```text
49
```

如果没有那行声明，编译器读到 `square(7)` 时还不认识这个名字，会报 `use of undeclared identifier 'square'`。如果反过来只有声明、忘了写定义，编译能过，链接时才报未定义符号——这正是"声明与定义"最典型的考法。

```cpp-fragment
#include <iostream>

int square(int x);          // 只有声明

int main() {
    std::cout << square(3) << '\n';
}
// 没有 square 的定义：编译通过，链接失败
```
链接器报 `undefined symbol: square(int)`。同理，如果把普通函数体写进头文件，再被两个 `.cpp` 包含，每个 `.o` 里都有一份定义，链接器会报 `multiple definition of 'twice(int)'`。这两种错误方向相反：一个缺定义，一个多定义。

### 1.4 cin/cout 与换行陷阱

`std::cout << x` 把 `x` 按类型转换成文本写到标准输出；`std::cin >> x` 先跳过前导空白（空格、制表、换行），再按 `x` 的类型解析。两者都返回流对象，所以能写 `cin >> a >> b`。格式化提取（formatted extraction）在遇到不属于当前类型的字符时停下，**不消费那个字符**，这是后面流状态一节的关键。

最经典的坑是 `>>` 与 `getline` 混用：`>>` 读完数字后把后面的换行符留在缓冲区，紧接着 `getline` 会立刻读到这个换行，返回空串。解决办法是在两者之间调用 `cin.ignore()` 吃掉一个字符（通常是换行），或者干脆全程用 `getline` 再自行解析。下面的程序用 `std::istringstream` 提供固定输入，把"踩坑"和"修正"各演示一遍；真实程序把 `in` 换成 `std::cin` 即可。

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::istringstream in("42 3.5\nhello world\n");

    int n = 0;
    double d = 0.0;
    in >> n >> d;                       // 读完 3.5 就停，'\n' 还在缓冲区
    std::string empty;
    std::getline(in, empty);            // 于是读到空串
    std::cout << "[" << empty << "]\n";

    std::istringstream in2("42 3.5\nhello world\n");
    in2 >> n >> d;
    in2.ignore();                       // 先吃掉那个换行
    std::string line;
    std::getline(in2, line);            // 现在才读到 "hello world"
    std::cout << "[" << line << "]\n";
}
```
输出：
```text
[]
[hello world]
```

`getline` 读一整行、丢掉行尾的 `\n`，但保留行首空格；空行会得到空串，这一点与"读取失败"不同。若输入行以 `\r\n` 结尾（Windows 文本文件），`getline` 会把 `\r` 留在串尾，长度会多 1，比较字符串时可能莫名其妙地不相等；读文件时可以先判断并去掉末尾的 `\r`。`ignore()` 不带参数时只丢一个字符，不要以为它会清空整行；要丢到行尾应写 `in.ignore(std::numeric_limits<std::streamsize>::max(), '\n')`，需要 `<limits>`。另一个技巧是用 `in >> std::ws` 吃掉前导空白（包括换行）再 `getline`，但它会连行首空格一起吃掉，只适合不在意缩进的场景。

### 1.5 读到文件结束与流状态

`cin >> x` 和 `getline` 都返回流本身，而 `istream` 定义了显式的 `operator bool`：读取成功时为 `true`，失败（failbit 或 badbit）时为 `false`。所以 `while (in >> x)` 和 `while (std::getline(in, line))` 是标准写法。流内部用一组状态位记录情况：

| 状态位 | 含义 | 常见触发 |
| --- | --- | --- |
| `goodbit` | 一切正常（值为 0） | 流的初始状态 |
| `eofbit` | 到达输入末尾 | `getline` 或 `>>` 读到 EOF |
| `failbit` | 解析失败或打开失败 | 用 `>>` 把 `"abc"` 读进 `int`；EOF 后继续读 |
| `badbit` | 流本身出错 | 底层 I/O 错误、缓冲区问题 |

`fail()` 在 failbit 或 badbit 时返回真；`eof()` 只反映 eofbit；`good()` 只有在没有任何错误位时才为真。`clear()` 把状态位清零，让流可以继续使用，但它**不清空缓冲区**，失败的那个字符仍在原地，所以清状态后往往还要 `ignore()` 或换一种读法。

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::istringstream in("first line here\nsecond line\n\nfourth\n");

    int lines = 0;
    int words = 0;
    std::string line;
    while (std::getline(in, line)) {    // 流能隐式转 bool，读到 EOF 才停
        ++lines;
        std::istringstream wordsIn(line);
        std::string w;
        while (wordsIn >> w) {          // 以空白切分单词
            ++words;
        }
    }
    std::cout << "lines=" << lines << " words=" << words << '\n';
}
```
输出：
```text
lines=4 words=6
```

注意空行也被 `getline` 计作一行（第 3 行），但贡献 0 个单词。若要区分"正常读完"和"解析失败"，循环结束后再看 `in.eof()`：为真说明是读完；否则多半是格式不对。还有一种常见错误：`while (!in.eof()) { in >> x; ... }`——`eof()` 只有在一次读取失败之后才置位，这种写法会多处理一次旧数据，应该直接用 `while (in >> x)`。

### 1.6 iomanip 格式化输出

`<iomanip>` 提供的操纵符（manipulator）修改的是流对象的状态，不是一次性函数调用。**大多数设置粘性**：`fixed`、`setprecision`、`left`/`right`、`setfill`、`boolalpha` 一旦设置就一直有效；只有 `setw` 是例外，只影响紧接着的那一次输出。

| 操纵符 | 作用 | 是否持续 |
| --- | --- | --- |
| `std::fixed` | 定点表示浮点数 | 是 |
| `std::setprecision(n)` | 配合 `fixed` 时是小数位数；单独用时是有效数字位数 | 是 |
| `std::setw(n)` | 下一次输出的最小宽度（不足补 `setfill` 字符） | 否 |
| `std::left` / `std::right` | 左对齐 / 右对齐（默认右对齐） | 是 |
| `std::setfill(c)` | 宽度不足时的填充字符 | 是 |
| `std::boolalpha` | `bool` 输出 `true`/`false` 而不是 1/0 | 是 |

```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(2) << 3.14159 << '\n';

    std::cout << '[' << std::setw(6) << 42 << "]\n";
    std::cout << '[' << std::left << std::setw(6) << 42 << "]\n";
    std::cout << '[' << std::right << std::setfill('0') << std::setw(5) << 7 << "]\n";

    std::cout << std::setfill(' ') << '[' << std::setw(5) << 1
              << "][" << 2 << "]\n";    // 第二个数没有 setw，不再补空格
}
```
输出：
```text
3.14
[    42]
[42    ]
[00007]
[    1][2]
```

最后一行证明 `setw` 只生效一次。还要注意 `setprecision(3)` 不与 `fixed` 连用时表示**有效数字**位数（`12345.678` 会被写成 `1.23e+04`），与 `fixed` 连用才表示小数点后的位数；`std::defaultfloat` 可以恢复默认格式。`std::endl` 与 `'\n'` 也不同：`endl` 输出换行**并刷新缓冲区**，`'\n'` 只换行。大量输出时用 `'\n'` 更快；只有交互提示、调试输出、程序崩溃前必须看到内容时，才值得用 `endl` 强制刷新。

### 1.7 考试视角与易错点

这一节的考点集中在四类：**入口函数**（返回 `int`、不能重载、不能调用）、**编译与链接错误的区分**（未声明 vs 未定义 vs 重复定义）、**`>>` 与 `getline` 的换行**、**格式化状态是否粘性**。判断口诀：看到"未声明"找头文件和拼写，看到"未定义符号"找函数体，看到"重复定义"找头文件里的函数体；看到 `getline` 读到空行，先想缓冲区里是不是还剩一个换行。

```cpp-fragment
// a.h
int twice(int x) { return 2 * x; }   // 普通函数写进了头文件
```
这个头文件被 `a.cpp` 和 `b.cpp` 各包含一次，两个目标文件都含 `twice` 的定义，链接时报 `multiple definition of 'twice(int)'`。正确做法是头文件只写 `int twice(int x);`，定义放进 `a.cpp`；若确实要在头文件里定义，必须加 `inline`。

```cpp-fragment
#include <iostream>

int main() {
    int n;
    std::cin >> n;          // 输入 "abc" 时解析失败
    std::cout << n << '\n';
}
```
输入不是数字时，`n` 会被写成 0（C++11 起的规定；更早的实现在失败时保持原值），流进入 fail 状态，之后再读都会失败，直到 `clear()`。考试里若问"输出什么"，要同时看 `n` 的值和后续读取是否还执行。真实代码应先判断 `if (std::cin >> n)` 再使用 `n`。

## 2. 基本类型与字面量

C++ 的类型系统在编译期就确定每个对象占多少字节、能表示哪些值。这一节先建立"类型—大小—范围"的对应关系，再讲三条最容易出错的规则：整型提升、浮点精度、隐式转换。考试里"下列程序输出什么""这行代码有什么问题"大多围绕这三条，尤其是溢出、截断和有无符号混用。

### 2.1 类型家族与 sizeof

C++ 的基本类型分四族。布尔型只有 `bool`；字符型包括 `char`、`signed char`、`unsigned char`，以及宽字符 `wchar_t`、`char8_t`、`char16_t`、`char32_t`；整数型是 `short`、`int`、`long`、`long long` 及各自的 `unsigned` 版本；浮点型是 `float`、`double`、`long double`。此外还有 `void` 和 `std::nullptr_t`。注意 `char`、`signed char`、`unsigned char` 是**三种不同的类型**，`char` 是否有符号由实现决定（本机为有符号），所以 `char` 适合存字符，做数值运算时应明确写 `signed char` 或 `unsigned char`。

标准只规定最小宽度和相对顺序，不规定具体字节数：`sizeof(char) == 1`（这里的"1"是一个字节，字节的位数由 `CHAR_BIT` 给出，至少 8），`short` 和 `int` 至少 16 位，`long` 至少 32 位，`long long` 至少 64 位，并且 `sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)`。**本讲义环境是 64 位 Windows，采用 LLP64 模型：`int` 和 `long` 都是 4 字节**；Linux 的 LP64 模型里 `long` 是 8 字节。这是跨平台代码最常踩的差异，考试若问"int 一定占 4 字节吗"，答案是否定的。

`sizeof` 是编译期运算符，结果是 `size_t`（无符号），单位是字节。它**不对表达式求值**：`sizeof(i++)` 不会让 `i` 自增；对数组使用则返回整个数组的字节数，这正是 `sizeof(a) / sizeof(a[0])` 求元素个数的依据。还有两个考试爱考的细节：C++ 中字符字面量 `'a'` 的类型就是 `char`，所以 `sizeof('a') == 1`（C 语言中是 `int`，结果为 4）；字符串字面量 `"abc"` 的类型是 `const char[4]`，末尾多一个 `'\0'`。

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << "CHAR_BIT = " << CHAR_BIT << '\n';
    std::cout << "char " << sizeof(char) << " short " << sizeof(short)
              << " int " << sizeof(int) << " long " << sizeof(long)
              << " long long " << sizeof(long long) << '\n';
    std::cout << "float " << sizeof(float) << " double " << sizeof(double)
              << " bool " << sizeof(bool) << '\n';
    std::cout << "pointer " << sizeof(void*) << '\n';
    std::cout << "sizeof('a') = " << sizeof('a')
              << " sizeof(\"abc\") = " << sizeof("abc") << '\n';
}
```
输出：
```text
CHAR_BIT = 8
char 1 short 2 int 4 long 4 long long 8
float 4 double 8 bool 1
pointer 8
sizeof('a') = 1 sizeof("abc") = 4
```

这段输出只保证在本机（64 位 Windows，LLP64）成立；换到 32 位平台，`pointer` 会变成 4，`long` 在 Linux 上会变成 8。考试中如果题目没有说明平台，涉及具体字节数的选项要谨慎，标准只保证上面那条不等式链和最小值。

既然宽度不固定，为什么还要保留这么多整型？一是历史兼容，二是给实现留出"最自然字长"的空间：`int` 通常对应机器的寄存器宽度，运算最快。写业务代码时按**需要表示的范围**选类型，而不是按习惯全写 `int`：确定要 64 位就用 `std::int64_t`，只存 0 到 255 就用 `unsigned char` 或 `std::uint8_t`。`<cstdint>` 里的 `int8_t`、`uint32_t` 等是"精确宽度"类型，平台不支持对应宽度时它们根本不存在；`int_least16_t`、`int_fast16_t` 则分别保证"至少"和"最快"。这一点在嵌入式与跨平台代码里尤其重要。

### 2.2 范围、溢出与整型提升

n 位无符号整数的范围是 0 到 2^n−1；n 位补码有符号整数的范围是 −2^(n−1) 到 2^(n−1)−1。32 位 `int` 的最大值是 2147483647。**有符号整数溢出是未定义行为（undefined behavior，UB）**：编译器可以假设它不会发生，从而删掉"溢出后回绕"的分支，甚至在常量折叠时给出任意结果；所以不要依赖 `INT_MAX + 1` 变成 `INT_MIN`。相反，无符号整数的加减按 2^n 取模，回绕是标准明确规定的，结果一定落在 0 到 2^n−1。

C++20 之前，标准允许有符号数用原码、反码或补码表示，`INT_MIN` 的确切值因此依赖平台；C++20 起强制使用补码（two's complement），`INT_MIN == -INT_MAX - 1` 才成为可移植的结论。这个改动影响的不只是取值范围，还影响位运算和右移的语义：补码下 `~x == -x - 1`，负数右移才是"算术右移"（补符号位）。老代码里"负数右移实现定义"的警告，在 C++20 之后可以放心一半；但移位涉及符号位时，最好在注释里写明要的是算术右移还是逻辑右移，而不是让读者去猜。

真正让初学者困惑的是**整型提升**（integral promotion）：`char`、`short`、`bool` 等rank 小于 `int` 的类型参与算术运算时，先转换成 `int`（如果 `int` 能表示它的全部值，否则转成 `unsigned int`）。所以两个 `char` 相加得到的是 `int`，不会在 8 位里溢出；把结果存回 `char` 才发生截断。二元运算还会做**通常算术转换**（usual arithmetic conversions）：两边类型不同时统一到公共类型，同 rank 的有符号与无符号相遇，有符号会转成无符号，负数的补码被当成巨大的正数。

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << INT_MAX << ' ' << INT_MIN << ' ' << UINT_MAX << '\n';

    unsigned int u = 0;
    u -= 1;                              // 模 2^32 回绕，有定义
    std::cout << u << '\n';

    unsigned char c = static_cast<unsigned char>(300);  // 300 % 256
    std::cout << static_cast<int>(c) << '\n';

    char a = 100;
    char b = 100;
    int sum = a + b;                     // 先提升为 int，再相加
    std::cout << sum << ' ' << sizeof(a + b) << '\n';

    unsigned int one = 1;
    std::cout << std::boolalpha
              << (static_cast<unsigned int>(-1) < one) << '\n';  // 隐式转换的等价写法
}
```
输出：
```text
2147483647 -2147483648 4294967295
4294967295
44
200 4
false
```

最后一行的本意是演示 `-1 < 1u`：`-1` 被转换成 `UINT_MAX`，所以比较结果为 `false`。示例里写成 `static_cast<unsigned int>(-1)` 是为了在 `-Wall` 下不触发符号比较警告，效果与隐式转换完全相同。`UINT_MAX`、`INT_MAX` 来自 `<climits>`，`<cstdint>` 里的 `std::int32_t`、`std::uint64_t` 则明确给定位宽，写跨平台代码时更可靠。

避免溢出要在运算**之前**检查，而不是算完再看：加法可以写成 `if (b > 0 && a > INT_MAX - b) { /* 溢出 */ }`，乘法可以先判断 `a != 0 && b > INT_MAX / a`。更省事的办法是先用更宽的类型算，例如两个 `int` 相加先用 `long long` 接住，再判断是否落在 `int` 范围内；clang 还提供 `__builtin_add_overflow(a, b, &result)` 这类内建函数，一次完成计算和判断。无符号虽然没有 UB，但"差值"同样危险：`a - b` 在 `a < b` 时会回绕成巨大正数，凡是可能为负的差值都应该先比较再减，或转成有符号类型。

### 2.3 浮点：表示、精度与比较

浮点数按二进制科学计数法存储：符号位、指数、尾数。`float` 通常 4 字节、24 位有效尾数，约 7 位十进制有效数字；`double` 8 字节、53 位尾数，约 15 到 17 位；`long double` 在 x86 上常见为 80 位扩展精度（本机 `sizeof` 为 16）。大多数十进制小数（0.1、0.2、0.3）在二进制下是无限循环小数，只能存成近似值，所以 `0.1 + 0.2 == 0.3` 是 `false`，打印 17 位可以看到真实值 `0.30000000000000004`。

判断浮点相等不能直接用 `==`，应比较差值的绝对值是否小于一个误差限（epsilon）：`std::abs(a - b) < 1e-9`。epsilon 要按数量级选：比较 1 附近的数用绝对误差没问题，比较 1e12 量级的数时 1e-9 太苛刻，应改用相对误差；比较 1e-12 量级的数时 1e-9 又太宽松。能精确表示的二进制小数（0.5、0.25、0.75）可以直接比较，但这属于特例，不应作为习惯。

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
    std::cout << (0.5 + 0.25 == 0.75) << '\n';   // 二进制可精确表示

    double d = 3.99;
    std::cout << static_cast<int>(d) << ' ' << static_cast<int>(-d) << '\n';
}
```
输出：
```text
0.30000000000000004
false
true
true
3 -3
```

浮点转整数是**向零截断**，不是四舍五入：`3.99` 得 3，`-3.99` 得 -3。需要四舍五入用 `std::round`，它返回的仍是浮点类型，还要再转一次。边界上，浮点值超出整数范围时转换是 UB；`double` 转 `float` 可能溢出成无穷大或丢失精度。还有两个特殊值：`0.0 / 0.0` 得到 NaN，任何与 NaN 的比较（包括 `NaN == NaN`）都为假；除以零得到无穷大，这些是 IEEE 754 的行为，但标准并不强制所有平台都支持，考试通常只考"不要用 == 比较浮点"。

`float` 和 `double` 怎么选？`float` 省内存、SIMD 友好，但只有约 7 位有效数字，累加一万个数就可能明显偏差；`double` 是默认选择，字面量 `3.14` 本来就是 `double`。`long double` 的宽度最不统一（本机 16 字节、x86 扩展精度，ARM 上可能等同 `double`），不要用它来赌精度。还要区分**存储精度**和**打印精度**：`setprecision(17)` 只是把已经存在的二进制值如实打印出来，不会让 `float` 变得更精确；反过来，默认 6 位输出会把 `0.30000000000000004` 显示成 `0.3`，看起来"相等"只是显示截断的假象。

### 2.4 字符、转义与 bool

`char` 本质上是 1 字节的整数类型，既可以存字符，也可以参与算术。`'A'` 是字符字面量，类型 `char`，在本机（ASCII 执行字符集）值为 65；`"A"` 是字符串字面量，类型 `const char[2]`，内容是 `'A'` 和 `'\0'`，两者不能混用。字符字面量可以参与运算：`'A' + 1` 的结果是 `int` 66，要得到字符 `'B'` 必须写 `static_cast<char>('A' + 1)`。

常用转义序列：`'\n'` 换行、`'\t'` 制表、`'\\'` 反斜杠、`'\"'` 双引号、`'\''` 单引号、`'\0'` 空字符、`'\x41'` 十六进制、`'\101'` 八进制。十六进制转义会**贪婪地吃掉后面所有十六进制数字**：`"\x41B"` 是一个值为 0x41B 的字符，而不是 `'A'` 后跟 `'B'`，需要拆成 `"\x41" "B"`。八进制转义最多吃三位。

`bool` 只有 `true` 和 `false`，`sizeof(bool)` 由实现定义（本机为 1）。算术和指针值转 `bool` 时，0、空指针转成 `false`，其余转成 `true`；`bool` 参与算术会提升为 1 或 0。默认 `cout` 打印 `bool` 是 1/0，设置 `std::boolalpha` 后才打印 `true`/`false`，而且这个设置会一直有效，直到 `std::noboolalpha`。

```cpp
#include <iostream>

int main() {
    char c = 'A';
    std::cout << c << ' ' << static_cast<int>(c) << '\n';
    std::cout << static_cast<char>(c + 1) << ' ' << (c + 1) << '\n';  // B 66

    std::cout << "tab=" << static_cast<int>('\t')
              << " newline=" << static_cast<int>('\n')
              << " backslash=" << static_cast<int>('\\') << '\n';

    std::cout << "\x41" << "\101" << '\n';   // 两种转义都表示 'A'

    std::cout << std::boolalpha << true << ' ' << false << '\n';
    std::cout << std::noboolalpha << true << ' ' << (7 > 3) << '\n';
}
```
输出：
```text
A 65
B 66
tab=9 newline=10 backslash=92
AA
true false
1 1
```

第二行最能说明"字符也是整数"：`c + 1` 先做整型提升得到 `int`，直接输出是 66；转回 `char` 才按字符输出 `B`。如果平台使用 EBCDIC 之类的非 ASCII 字符集，`'A'` 的数值可能不是 65，但相对顺序和转义规则不变；本讲义按 ASCII 讲。

`char` 的符号性会直接影响比较。若 `char` 有符号，`char c = 0xFF;` 中 `c` 的实际值是 -1，`c == 255` 永远为假；若 `char` 无符号，结果又是 255。所以判断字符类别要用 `<cctype>` 的 `std::isdigit`、`std::isalpha` 等函数，它们的参数是 `int`，传入负值（`EOF` 除外）是 UB，正确写法是 `std::isdigit(static_cast<unsigned char>(c))`。字符串字面量的类型是 `const char[N]`，C++11 起禁止把 `"abc"` 赋给 `char*`；它通常放在只读区，试图修改是 UB，需要可修改的副本就拷贝到 `char buf[]` 或 `std::string`。

### 2.5 字面量、auto 与 const/constexpr

整数字面量的类型由"写法和后缀"共同决定。十进制不带后缀时，依次尝试 `int`、`long`、`long long`；八进制、十六进制、二进制还会在中间插入对应的无符号类型，所以 32 位 `int` 平台上 `0xFFFFFFFF` 的类型是 `unsigned int`，而不是 `int`。后缀 `u` 表示无符号，`l` 表示 `long`，`ll` 表示 `long long`，`f` 表示 `float`；`10` 是 `int`，`10u` 是 `unsigned int`，`10LL` 是 `long long`，`10.0` 是 `double`，`10.0f` 是 `float`。C++14 起支持二进制字面量 `0b1010` 和数字分隔符 `1'000'000`，后者只是给眼睛看的，不改变值。

`auto` 用初始化器推导类型，规则与模板实参推导相同：**丢弃顶层 `const` 和引用**。`const int& r = n; auto a = r;` 得到 `int`；要保留引用写 `auto&`，要保留 const 写 `const auto&`。`decltype(expr)` 则原样保留类型和值类别，`decltype(r)` 是 `const int&`。`const` 表示运行期不可修改，初始化后不能再赋值；`constexpr` 要求编译期可求值，可用于数组大小、模板参数和 `case` 标签，`constexpr` 函数在实参是常量表达式时编译期求值，否则退化为普通函数。

```cpp
#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;
    std::cout << "10 -> int: " << std::is_same_v<decltype(10), int> << '\n';
    std::cout << "10u -> unsigned int: "
              << std::is_same_v<decltype(10u), unsigned int> << '\n';
    std::cout << "10LL -> long long: "
              << std::is_same_v<decltype(10LL), long long> << '\n';
    std::cout << "10.0f -> float: " << std::is_same_v<decltype(10.0f), float> << '\n';
    std::cout << "10.0 -> double: " << std::is_same_v<decltype(10.0), double> << '\n';
    std::cout << "0xFFFFFFFF -> unsigned int: "
              << std::is_same_v<decltype(0xFFFFFFFF), unsigned int> << '\n';
}
```
输出：
```text
10 -> int: true
10u -> unsigned int: true
10LL -> long long: true
10.0f -> float: true
10.0 -> double: true
0xFFFFFFFF -> unsigned int: true
```

最后一行依赖"本机 `int` 为 32 位"这一前提：如果 `int` 是 64 位，`0xFFFFFFFF` 会直接放进 `int`。`std::is_same_v` 来自 `<type_traits>`，是考试里验证"这个表达式到底是什么类型"的利器，比死记规则可靠。

```cpp
#include <iostream>

constexpr int square(int x) {
    return x * x;
}

int main() {
    int n = 5;
    const int& r = n;

    auto a = r;              // int：丢掉引用和顶层 const
    auto& b = r;             // const int&：保留引用
    const auto c = r;        // const int

    constexpr int k = square(6);
    int arr[k] = {};         // k 是编译期常量，可以做数组大小

    std::cout << a << ' ' << b << ' ' << c << '\n';
    std::cout << k << ' ' << sizeof(arr) / sizeof(arr[0]) << '\n';
}
```
输出：
```text
5 5 5
36 36
```

`const int& r = n;` 是常量引用，不能通过 `r` 修改 `n`，但 `n` 自己可以变；`auto b = r` 会拷贝出 `int`，`auto& b = r` 才绑定到原对象。`constexpr int k = square(6)` 在编译期算出 36，因此可以拿来定义数组；如果写成 `int k = square(6)`，即使编译器可能优化，语言层面也不是常量表达式，不能作数组大小。

`const` 与 `constexpr` 的区别可以用一句话概括：**`const` 承诺"不修改"，`constexpr` 承诺"编译期就知道"**。`constexpr` 变量隐含 `const`，反过来不成立；`constexpr` 函数只是"具备编译期求值的能力"，用运行时变量调用时照样在运行时执行。函数里不能只有 `constexpr` 才能调用的限制，但 `constexpr` 函数体内能用的语句在 C++14 之后已大幅放宽（可以有局部变量、循环、分支），C++20 又允许 `constexpr` 虚函数和 `try`。考试一般只考到"数组大小必须是常量表达式"和"`constexpr` 函数在常量上下文里必须能编译期算出结果"这两层。

| 对比项 | `const` | `constexpr` |
| --- | --- | --- |
| 核心含义 | 运行期只读 | 编译期可求值 |
| 初始化时机 | 可以是运行时 | 必须是常量表达式 |
| 典型用途 | 接口约束、防止误改 | 数组大小、模板实参、`case` 标签 |
| 是否隐含另一者 | 不隐含 `constexpr` | 隐含 `const` |

### 2.6 类型转换与常见边界

隐式转换发生在赋值、传参、返回和混合运算中，方向是"往能容纳更多值的类型转"，但窄化时就会丢数据。`double d = 3.99; int i = d;` 得到 3（向零截断）；`int` 转 `char` 只保留低 8 位；有符号数与无符号数混合时按 2.2 的规则统一。列表初始化（花括号）**禁止窄化**：`int i{3.9}` 编译直接报错，这是它比圆括号更安全的原因，也是"统一初始化"被推荐的理由。

显式转换优先用 `static_cast<T>(x)`：它在编译期检查类型关系，意图清楚、容易搜索。C 风格 `(T)x` 会依次尝试 `const_cast`、`static_cast`、`reinterpret_cast` 等多种转换，危险且难以 grep；`reinterpret_cast` 只用于底层字节解释，考试中出现基本都是为了考"这是不安全的"。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3};

    std::cout << std::boolalpha
              << (-1 < static_cast<int>(v.size())) << '\n';  // 先转有符号再比较
    std::cout << (v.size() - 4) << '\n';                     // size_t 下溢
}
```
输出：
```text
true
18446744073709551615
```

`v.size()` 返回 `size_t`（本机为 64 位无符号）。如果直接写 `-1 < v.size()`，`-1` 会被转成 `SIZE_MAX`，结果是 `false`；示例先转成 `int` 才是我们想要的比较。第二行 `3 - 4` 在无符号里回绕成 `SIZE_MAX`，即 18446744073709551615。C++20 提供了 `std::ssize(v)`，返回有符号的 `ptrdiff_t`，是避免这类问题的正解；老代码里则常把 `size()` 先存进 `int n` 再比较。

隐式转换不只出现在算术运算里：函数传参、返回值、`if` 条件、类类型的转换构造函数都会触发。最隐蔽的是 `bool` 上下文：`if (p)` 实际是 `p != nullptr`，`if (x)` 是 `x != 0`，而 `if (x = 0)` 是赋值、结果恒为假。C++11 的 `explicit` 能阻止单参构造函数的隐式转换，`explicit operator bool` 则允许 `if (p)` 却禁止 `int n = p`，这两条会在类与资源管理一节展开。

```cpp-fragment
char c = 300;                 // 300 超出 char 范围：实现定义（本机得 44）
int x = INT_MAX; ++x;         // 有符号溢出：未定义行为，不能依赖回绕
int i{3.9};                   // 错误：列表初始化禁止 double 到 int 的窄化
for (unsigned i = n - 1; i >= 0; --i) { }   // i 永远 >= 0，死循环
```
第一条在 `char` 有符号的平台上结果是实现定义（通常是低 8 位 44），换平台可能不同；第二条是 UB，优化器可以把整个分支删掉；第三条编译报 `type 'double' cannot be narrowed to 'int'`；第四条在 `i` 减到 0 后再减会回绕成 `UINT_MAX`，循环条件永远为真。正确写法分别是：用 `int` 存 300、在加法前检查上界或用无符号计算、用 `static_cast<int>(3.9)`、把循环变量改成 `int` 或写 `for (unsigned i = n; i-- > 0;)`。

考试阅读题里，转换方向的判断可以按一条主线走：**短的向长的看齐，有符号向无符号看齐（在混合运算时），整数向浮点看齐**。`char`、`short`、`bool` 先提升到 `int`，再参与算术转换；`int` 与 `unsigned int` 混合时 `int` 转成 `unsigned int`；`int` 与 `double` 混合时转成 `double`。记住这条主线，`-1 < 1u`、`'A' + 1`、`5 / 2.0` 这类题就能一步步推导，而不是死记结果。

考试视角：看到"输出什么"，先确认每个字面量的类型和每个变量的符号性；看到 `size()`、`sizeof`、下标，先想无符号；看到 `0.1 + 0.2`，先想二进制无法精确表示；看到 `char` 参与算术，先想整型提升。判断口诀：**小类型先提升，混符号转无符号，浮点不比相等，窄化必用显式转换**。

## 3. 运算符与表达式

表达式是 C++ 程序最小的"计算单位"：变量、字面量、函数调用用运算符连起来，就得到一个值，过程中还可能修改对象。考试里与表达式有关的题几乎都围绕三件事：**优先级决定谁先结合，结合性决定同级的左右顺序，求值顺序决定副作用何时发生**。三者互相独立，混为一谈就会在 `a & b == c`、`i++ + i++` 这类题上翻车。

### 3.1 表达式、左值与求值顺序

先建立几个术语。表达式求值后产生一个值，同时可能产生副作用（赋值、自增、输入输出）。**左值（lvalue）**指有名字、有确定存储位置的对象，例如变量 `x`、数组元素 `a[i]`、解引用 `*p`；左值可以取地址，也可以放在赋值号左边。右值（rvalue）是临时结果，例如 `x + 1`、函数返回的 `int`、字面量 `42`，它们没有持久地址，不能赋值。基础阶段记住"能不能被赋值、能不能取地址"这条粗线即可。

优先级和结合性只决定**语法树怎么搭**，不决定**先算哪一边**。例如 `f() + g()` 谁先调用没有规定，结果依赖顺序就不可移植。C++17 起，赋值号右边先于左边，移位左操作数先于右操作数，下标先于数组表达式，成员访问的对象先于成员；`&&`、`||`、`?:`、逗号则一直有明确的短路或先后规则。

真正危险的是**同一对象在两次修改之间没有顺序**。`int i = 0; i = i++ + 1;` 中 `i++` 修改了 `i`，赋值又修改 `i`，两者谁先谁后没有规定，标准直接判为未定义行为（UB），任何输出都"合法"。类似地 `f(i++, i++)`、`a[i] = i++` 都属于这一类。考试中看到同一表达式里对同一变量出现两次 `++`/`--`，第一反应应该是"这题可能没有确定答案"，而不是硬算。

```cpp
#include <iostream>

int main() {
    int i = 0;
    int a = (i = 3, i + 1);          // 逗号：先赋值，再取 i + 1
    std::cout << a << ' ' << i << '\n';

    int b = 0;
    b = (b = 2, b * 3);              // 逗号表达式的结果是右操作数
    std::cout << b << '\n';
}
```
输出：
```text
4 3
6
```

逗号运算符是少数保证"左先右后"的运算符，结果类型和值都来自右操作数。上面第一行先改 `i` 为 3，再算 4；第二行先改 `b` 为 2，再得 6。注意 `f(a, b)` 里的逗号只是参数分隔符，两个实参的求值顺序未指定；只有 `(a, b)` 才是逗号表达式，`int a = 1, b = 2;` 里的逗号也只是声明分隔符。

### 3.2 算术运算符

算术运算符包括二元 `+ - * / %` 和一元 `+ -`。整数除法**向零截断**：`7 / 2` 得 3，`-7 / 2` 得 -3，不是向下取整的 -4。取模 `%` 的结果符号**跟随被除数**：`-7 % 2` 得 -1，`7 % -2` 得 1。`%` 只能用于整数，浮点取余要用 `<cmath>` 的 `std::fmod`。

除法有两个边界。第一，整数除以零是 UB，不是抛异常，也不是得 0；浮点除以零得到无穷大或 NaN（IEEE 754 行为）。第二，`INT_MIN / -1` 溢出。乘法、加法、减法溢出时，有符号数是 UB，无符号数按 2^n 回绕。所以"先算再判断"永远来不及，必须在运算前检查，或改用更宽的类型。`x += y` 与 `x = x + y` 大多数时候等价，区别是复合赋值对左操作数只求值一次：`a[i++] += 2` 只自增一次，而 `a[i++] = a[i++] + 2` 是 UB。

```cpp
#include <iostream>

int main() {
    int a = -7, b = 2;
    std::cout << a / b << '\n';           // 向零截断
    std::cout << a % b << '\n';           // 余数符号随被除数
    std::cout << 7 / 2 << ' ' << 7 % 2 << '\n';
    std::cout << 7 / 2.0 << '\n';         // 混合运算转成 double

    int x = 5;
    x += 3;
    x *= 2;
    std::cout << x << '\n';

    unsigned int u = 0;
    --u;                                  // 无符号回绕是定义良好的
    std::cout << u << '\n';
}
```
输出：
```text
-3
-1
3 1
3.5
16
4294967295
```

第一行 `-7 / 2` 得 -3，说明是向零截断；第二行 `-7 % 2` 得 -1，说明余数跟着 -7 走。第四行 `7 / 2.0` 中 `2.0` 是 `double`，`7` 先转成 `double`，结果是 3.5。第五、六行演示复合赋值：`5 + 3 = 8`，再乘 2 得 16。最后一行对值为 0 的无符号数减一，得到 `UINT_MAX`，这是标准规定的回绕，不是 UB。

### 3.3 位运算

位运算直接操作整数的二进制位，包括按位与 `&`、按位或 `|`、按位异或 `^`、按位取反 `~`、左移 `<<`、右移 `>>`。它们只接受整数操作数，结果也是整数；`bool` 会被提升成 `int` 再算。按位与常用来取位或测位，按位或用来置位，异或用来翻转，取反用来生成掩码。位编号从最低位 0 开始，`1u << k` 就是只有第 k 位为 1 的掩码。

移位有两条硬规则。第一，**移位位数必须大于等于 0 且小于操作数类型的位宽**，否则是 UB；`1 << 32` 在 32 位 `int` 上不是"得 0"，而是未定义。第二，左移有符号负数、或左移结果不能由有符号类型表示，都是 UB；右移负数在 C++20 之前是实现定义，C++20 起规定为算术右移（补符号位）。因此写掩码时优先用无符号类型：`1u << 31` 合法，`1 << 31` 则踩在边界上。

优先级方面，移位低于加减、高于关系运算。所以 `1 << 2 + 3` 等价于 `1 << (2 + 3)` 得 32，而不是 `(1 << 2) + 3` 得 7。这类表达式 clang 的 `-Wall` 会给出 `-Wshift-op-parentheses`、`-Wparentheses` 警告，看到警告就加括号，不要和编译器斗智。位运算还不能替代逻辑运算：`&` 和 `|` 没有短路，两边都会求值。

```cpp
#include <iostream>

int main() {
    unsigned int flags = 0;
    flags |= 1u << 2;                     // 置第 2 位
    flags |= 1u << 4;                     // 置第 4 位
    std::cout << flags << '\n';

    std::cout << (flags & (1u << 2)) << '\n';   // 测试第 2 位
    flags &= ~(1u << 2);                        // 清第 2 位
    std::cout << flags << '\n';

    std::cout << (flags ^ (1u << 4)) << '\n';   // 翻转第 4 位
    std::cout << (~0u) << '\n';                 // 全 1
    std::cout << (1 << 3) << '\n';

    int x = -8;
    std::cout << (x >> 2) << '\n';              // 算术右移
    std::cout << (x & 0xFF) << '\n';            // 取低 8 位
}
```
输出：
```text
20
4
16
0
4294967295
8
-2
248
```

前两行把第 2 位（值 4）和第 4 位（值 16）置起来，得到 20。`flags & (1u << 2)` 取出第 2 位，结果是 4。`flags &= ~(1u << 2)` 用"第 2 位为 0、其余为 1"的掩码清零，结果 16。再异或第 4 位得到 0。`~0u` 把所有位取反，得到 4294967295。`x = -8` 的补码低 8 位是 11111000，算术右移两位得 -2；与 `0xFF` 相与得到 248。

### 3.4 逻辑运算符与短路求值

逻辑运算符有三个：`&&`（与）、`||`（或）、`!`（非）。操作数按"是否非零"转换成 `bool`，结果类型是 `bool`，但 `std::cout` 默认把它打印成 0/1；加上 `std::boolalpha` 才打印 `true`/`false`。`!` 的优先级高于 `&&` 和 `||`，`&&` 又高于 `||`，所以 `!a && b || c` 等价于 `((!a) && b) || c`。

短路求值是逻辑运算符最重要的语义：`a && b` 先算 `a`，若 `a` 为假，`b` **根本不求值**，整个表达式直接为假；`a || b` 先算 `a`，若 `a` 为真，`b` 也不求值。好处是可以写守卫：`if (p != nullptr && p->value > 0)` 在 `p` 为空时不会解引用；`if (d != 0 && n / d > 1)` 在除数为零时不会触发除法。坏处是右边的副作用可能被跳过：`flag = f() || g();` 中 `g()` 是否执行取决于 `f()` 的返回值，读代码时不能假设两个函数都调用了。

按位 `&`、`|` 与逻辑 `&&`、`||` 是两组完全不同的运算符。`a & b` 先求值两边再做逐位与，没有短路；`a && b` 先看左边，可能根本不看右边。对 `bool` 使用 `&` 虽然能算出相同真假，但失去了短路保护，性能和行为都可能不同，代码审查时通常被要求改掉。另一个常见错误是数学式 `a < b < c`：它先算 `a < b` 得到 `bool`，再拿 0 或 1 与 `c` 比较，并不表示"b 在 a、c 之间"；正确写法是 `a < b && b < c`。

```cpp
#include <iostream>

int calls = 0;
bool positive(int x) { ++calls; return x > 0; }
int bits(int x) { ++calls; return x; }

int main() {
    int zero = 0;
    if (zero != 0 && 10 / zero > 1) {     // 右边被短路，不会除零
        std::cout << "unreachable\n";
    }
    std::cout << "guard ok\n";

    calls = 0;
    bool r1 = positive(5) || positive(-1);   // 第二个不调用
    bool r2 = positive(-1) && positive(5);   // 第二个不调用
    std::cout << r1 << ' ' << r2 << ' ' << calls << '\n';

    calls = 0;
    int r3 = bits(5) & bits(-1);     // 位与不短路：两个都调用
    std::cout << r3 << ' ' << calls << '\n';

    bool t = true, f = false;
    std::cout << (t && f) << ' ' << (t || f) << ' ' << (!t) << '\n';
}
```
输出：
```text
guard ok
1 0 2
5 2
0 1 0
```

第一段是短路守卫：`zero != 0` 为假，`10 / zero` 没被求值，程序不会除零。第二段 `r1` 跳过 `positive(-1)`，`r2` 跳过 `positive(5)`，`calls` 共 2。第三段用位与，两个 `bits` 都执行，`calls` 为 2，5 & -1 得 5。最后一行三个结果分别是 0、1、0。

### 3.5 自增与自减

`++` 和 `--` 把变量加一或减一，有前缀、后缀两种写法。**前缀** `++i` 先修改再返回新值，结果是左值；**后缀** `i++` 先保存旧值，再修改，返回旧值，结果是右值。所以 `int a = i++;` 让 `a` 拿到旧值、`i` 加一；`int a = ++i;` 让 `a` 和 `i` 都变成新值。对内置类型两者通常优化成一样；对迭代器，后缀要先复制旧对象，前缀通常更快，这是"循环里优先用 `++it`"的来源。

自增自减的操作数必须是**可修改的左值**：`++5`、`++(x + 1)` 都编译错误；`const int c = 0; ++c;` 也错误。`bool` 在 C++17 之后不再支持 `++`，因为 `true + 1` 再转回 `bool` 的语义已被删除，写 `b++` 会编译失败。指针的 `++` 按元素大小移动，不是按字节。

最需要警惕的是在同一表达式中多次修改同一对象。`i = i++ + 1;`、`i++ + i++`、`cout << i++ << i++` 都是 UB，因为两次修改之间没有顺序。即使某台编译器给出"先取旧值再自增"的稳定结果，也不能写进答案。C++17 虽然规定了赋值右侧先于左侧、移位左侧先于右侧，但并没有把 `i = i++` 变成合法；它仍然是"修改与读取之间无顺序"。考试中遇到这类表达式，正确回答是"未定义行为，结果不可预测"，而不是某个具体数字。

```cpp
#include <iostream>

int main() {
    int i = 5;
    std::cout << i++ << ' ' << i << '\n';    // 后缀：先输出 5，再变 6

    std::cout << ++i << ' ' << i << '\n';    // 前缀：先变 7，再输出 7

    int j = 5;
    int a = j++;
    int b = ++j;
    std::cout << a << ' ' << b << ' ' << j << '\n';

    int k = 10;
    k -= 3;
    k *= 2;
    std::cout << k << '\n';

    int m = 0;
    m = m + 1;                               // 等价但安全的写法
    std::cout << m << '\n';
}
```
输出：
```text
5 6
7 7
5 7 7
14
1
```

第一行 `i++` 返回旧值 5，`i` 变成 6；第二行 `++i` 先变 7 再返回 7；第三段 `a = j++` 得 5、`b = ++j` 得 7。想表达加一时，`m = m + 1` 与 `++m` 等价，且不引入序列问题；复杂表达式宁可拆成多条语句。

### 3.6 赋值、逗号与条件运算符

赋值运算符 `=` 是右结合的，`a = b = c` 先算 `b = c`，再把结果赋给 `a`；整个赋值表达式的值是左操作数的新值，类型是左操作数的引用。复合赋值 `+= -= *= /= %=` 以及位运算、移位版本，都是"读左值、算、写回"，左操作数只求值一次。不要把它和 `==` 混淆：`if (a = b)` 是赋值，条件值是 `b` 转换后的真假，编译器通常给出 `-Wparentheses` 警告。

逗号运算符前面已经出现过：它保证左操作数先求值，丢弃其结果，返回右操作数的值和类型；优先级最低，比赋值还低。声明里的逗号是分隔符；`x = 1, y = 2;` 才是先赋值再取 2。逗号适合写在 `for` 的初始化或步进里：`for (int i = 0, j = n; i < j; ++i, --j)`，但不要为了"一行写完"而滥用。

条件运算符 `?:` 是 C++ 中唯一的三元运算符：`cond ? a : b` 先算 `cond`，为真只求值 `a`，为假只求值 `b`，因此也有"短路"效果。它的优先级高于赋值、低于逻辑或，右结合，可以嵌套。两个分支的类型会做统一：`cond ? 1 : 2.5` 的结果是 `double`，即使走 `1` 的分支也会得到 `1.0`；如果两个分支一个是左值、一个不是，结果通常是右值。`a > b ? a : b` 常用来取较大值；注意只求值一个分支，不要写成带副作用的 `f() > g() ? f() : g()`。

```cpp
#include <iostream>

int main() {
    int a = 0, b = 0, c = 0;
    a = b = c = 4;                       // 右结合，从右往左赋
    std::cout << a << b << c << '\n';

    int x = 5, y = 2;
    x += y * 3;                          // 先乘后加，再复合赋值
    std::cout << x << '\n';

    int m = 7, n = 9;
    int& ref = (m > n ? m : n);          // 两个分支都是左值，结果仍是左值
    ref = 100;
    std::cout << m << ' ' << n << '\n';

    int k = (m = 1, m + 2);              // 逗号：先赋值，再求和
    std::cout << k << ' ' << m << '\n';
}
```
输出：
```text
444
11
7 100
3 1
```

第一段 `a = b = c = 4` 从右往左执行，三个变量都变成 4，连续输出 "444"。第二段 `x += y * 3` 先算 `2 * 3 = 6`，再 `5 + 6 = 11`。第三段 `m > n` 为假，条件表达式选 `n`；因为 `m`、`n` 都是 `int` 左值，整个表达式也是左值，`ref` 绑定到 `n`，把 `ref` 改成 100 就是改 `n`，输出 "7 100"。

### 3.7 优先级与结合性

优先级表不需要背全，但要知道大致分层。从高到低可以记成：**后缀（`()`、`[]`、`.`、`->`、`++`/`--`）→ 前缀（`!`、`~`、`++`/`--`、一元 `+`/`-`、`*`、`&`、`sizeof`）→ 乘除取模 → 加减 → 移位 → 关系 → 相等 → 按位与 → 按位异或 → 按位或 → 逻辑与 → 逻辑或 → 条件 → 赋值 → 逗号**。同级运算符里，只有赋值和条件运算符是右结合，其余二元运算符都是左结合。

| 优先级 | 运算符 | 结合性 | 记忆点 |
| --- | --- | --- | --- |
| 最高 | `()` `[]` `.` `->` `++` `--`（后缀） | 左 | 成员、下标、调用先算 |
| 高 | `!` `~` `++` `--`（前缀） `+` `-` `*` `&` `sizeof` | 右 | 一元运算符贴近操作数 |
| 中高 | `*` `/` `%` | 左 | 乘除高于加减 |
| 中 | `+` `-` | 左 | 先乘除后加减 |
| 中 | `<<` `>>` | 左 | 低于加减、高于关系 |
| 中低 | `<` `<=` `>` `>=` | 左 | 关系 |
| 低 | `==` `!=` | 左 | 相等低于关系 |
| 低 | `&` `^` `|` | 左 | 位运算低于相等 |
| 低 | `&&` | 左 | 逻辑与 |
| 低 | `||` | 左 | 逻辑或 |
| 最低之一 | `?:` | 右 | 条件 |
| 最低之一 | `=` `+=` 等 | 右 | 赋值 |
| 最低 | `,` | 左 | 逗号 |

三个高频陷阱都来自"位运算和移位比比较运算符低"。`a & b == c` 实际是 `a & (b == c)`；`x & 1 == 0` 实际是 `x & (1 == 0)`，结果恒为 0；`1 << 2 + 3` 实际是 `1 << 5`。clang 的 `-Wall` 会分别给出 `& has lower precedence than ==` 和 `operator '<<' has lower precedence than '+'` 警告，并建议加括号。另一个陷阱是 `!x == y`：`!` 只作用于 `x`，实际是 `(!x) == y`，而不是 `!(x == y)`；`-Wlogical-not-parentheses` 也会提示。

```cpp
#include <iostream>

int main() {
    int a = 2, b = 3, c = 4;
    std::cout << a + b * c << '\n';          // 先乘后加
    std::cout << (a + b) * c << '\n';        // 括号改变结合
    std::cout << -a * b << '\n';             // 一元负号先于乘法

    std::cout << (a < b && b < c) << '\n';   // 关系高于逻辑与
    std::cout << (a > b || c > a) << '\n';   // 关系高于逻辑或

    int p, q;
    p = q = 7;                               // 赋值右结合
    std::cout << p << q << '\n';

    int big = a > b ? a : b;
    std::cout << big << '\n';

    std::cout << (~0) << '\n';               // 按位取反
}
```
输出：
```text
14
20
-6
1
1
77
3
-1
```

前两行对比乘法与括号：无括号得 14，加括号得 20。第三行一元负号先作用于 `a` 再乘 `b`，得 -6。第四、五行验证关系运算符高于 `&&`、`||`，结果都是 1。第六行 `p = q = 7` 从右往左，输出 "77"。第七行条件运算符选出 3。最后 `~0` 把所有位取反，得到 -1。

### 3.8 表达式中的类型转换

算术表达式里，操作数类型不同就要先转换。规则可以概括为三步：**先做整型提升**（`bool`、`char`、`short` 提升到 `int`），**再做寻常算术转换**（向能表示更多值的类型看齐：`int` 与 `long` 转 `long`，整数与 `double` 转 `double`，`int` 与 `unsigned int` 转 `unsigned int`），**最后按结果类型运算**。所以 `char c = 'A'; c + 1` 的类型是 `int`，值为 66，要得到字符必须转回 `char`。

有符号与无符号混合是最容易失分的地方。`int` 与 `unsigned int` 运算时，`int` 先转成 `unsigned int`，负数会变成巨大的正数；`-1 < 1u` 因此为假。`size_t` 在 64 位平台上比 `int` 宽，`int` 与 `size_t` 比较时转成 `size_t`，负的 `int` 同样回绕。这类比较在 `-Wall` 的 `-Wsign-compare` 下会报警，考试时先看警告，再按 2.2 的规则推结果。

浮点与整数混合时，整数转成浮点；浮点赋给整数则向零截断，超出范围是 UB。窄化转换（`double` 到 `int`）不会自动检查，列表初始化 `int i{3.9}` 则会直接编译错误。表达式的结果类型还会影响重载决议和 `sizeof`：`sizeof(c + 1)` 是 `sizeof(int)`，不是 `sizeof(char)`。考试里问"这行输出什么"，先写出每个子表达式的类型，再算值，比直接猜更稳。

```cpp
#include <iostream>

int main() {
    char c = 'A';
    std::cout << c + 1 << '\n';                     // 提升为 int，输出 66
    std::cout << static_cast<char>(c + 1) << '\n';  // 转回 char，输出 B

    std::cout << (5 / 2) << ' ' << (5 / 2.0) << '\n';

    unsigned int u = 4294967295u;
    std::cout << u + 1u << '\n';                    // 无符号回绕

    unsigned short s = 65535;
    s = s + 1;                                      // 65536 按模 65536 存回
    std::cout << s << '\n';

    double d = 3.99;
    int i = d;                                      // 向零截断
    std::cout << i << '\n';

    std::cout << (static_cast<unsigned int>(-1) < 1u) << '\n';
}
```
输出：
```text
66
B
2 2.5
0
0
3
0
```

第一行 `c + 1` 提升为 `int` 66；第二行转回 `char` 才输出 B。第三行对比整数除法与浮点除法：2 和 2.5。第四行 `u + 1u` 回绕成 0；第五行 65536 按 2^16 取模存回 `unsigned short` 得 0；第六行 `3.99` 向零截断得 3。最后一行说明"负数小于 1"的直觉在无符号里不成立。

### 3.9 考试视角：读表达式的顺序

读复杂表达式可以按四步走：**定类型**（有没有 `char`、`short`、`unsigned`、`size_t` 参与）、**看优先级**（先找括号，再按"后缀→一元→乘除→加减→移位→关系→相等→位运算→逻辑→条件→赋值→逗号"分层还原语法树）、**看结合性**（赋值、条件右结合，其余二元左结合）、**看求值顺序与副作用**（短路、重复修改、除零或溢出）。

按这个顺序，`a & b == c` 是 `a & (b == c)`，`1 << 2 + 3` 是 `1 << 5`，`-1 < 1u` 先把 `-1` 转成 `UINT_MAX`。凭"从左往右算"几乎必错。问"编译器会警告什么"时，答案通常是 `-Wparentheses`、`-Wsign-compare`、`-Wshift-count-overflow` 这几类。

下面这些片段是反面教材，**不要编译运行**，重点是看懂它们错在哪：

```cpp-fragment
if (a & b == c) { }            // 实际是 a & (b == c)，-Wparentheses
int x = 1 << 2 + 3;            // 实际是 1 << 5，-Wshift-op-parentheses
int y = !a == b;               // 实际是 (!a) == b，-Wlogical-not-parentheses
```

```cpp-fragment
int i = 0;
i = i++ + 1;                   // UB：i 在表达式中被修改两次，且无顺序
int x = i++ + i++;             // UB：同上，任何结果都不能依赖
int n = 1 << 32;               // UB：移位位数不小于 int 的宽度
int z = 1 / 0;                 // UB：整数除以零
```

```cpp-fragment
int m = INT_MAX;
++m;                           // UB：有符号整数溢出
int q = -7 % 2;                // 合法，但结果是 -1，不是 1
unsigned int r = 3;
r -= 5;                        // 无符号回绕成 4294967294，不是 -2
```

第一条说明位运算比相等低，要么写 `(a & b) == c`，要么写 `a & (b == c)`；第二条说明加法比移位紧，想先移位必须加括号；第三条说明 `!` 只作用于紧跟的操作数，想比较再取反要写 `!(a == b)`。第四、五条是序列问题：同一对象两次修改之间没有顺序，正确做法是拆成多条语句。第六条是移位越界，不是"移空了得 0"；第七条整数除零是 UB，不是异常；第八条有符号溢出是 UB；第九条合法，但余数为 -1；第十条无符号减法回绕定义良好，结果不是 -2。

最后一条经验：**拿不准就加括号，拿不准就拆语句**。括号只改优先级，不改求值顺序，也不会让 UB 变合法；拆语句能消除大部分序列问题。读题先看四类信号：同一表达式两次 `++`、`unsigned` 与 `int` 混比、`<<` 与 `+` 相邻、`&` 与 `==` 相邻。

## 4. 控制流

程序默认从上到下顺序执行，控制流语句改变这个顺序。选择结构让程序"走哪条路"，循环结构让程序"走多少遍"，跳转语句让程序"提前离开"。考试考控制流，很少考语法本身，考的是三件事：**绑定**（这个 `else` 到底属于哪个 `if`）、**贯穿**（这个 `case` 会不会漏下去）、**边界**（循环到底执行几次、最后一次取什么值）。下面每一节都围绕这三件事展开。

### 4.1 if 语句与悬垂 else

`if` 的语法是 `if (条件) 语句`，可以带 `else`。条件按语境转换成 `bool`：整数非零为真，指针非空为真，浮点数非零为真。`else if` 并不是独立语法，它只是"`else` 后面跟了一条 `if` 语句"的写法，所以一条 `if ... else if ... else` 链本质上是一层层嵌套。

判断成绩等级是典型的多分支：

```cpp
#include <iostream>
#include <string>

std::string grade(int score) {
    if (score >= 90) {
        return "A";
    } else if (score >= 80) {
        return "B";
    } else if (score >= 60) {
        return "C";
    } else {
        return "F";
    }
}

int main() {
    std::cout << grade(95) << ' ' << grade(85) << ' '
              << grade(60) << ' ' << grade(59) << '\n';
}
```

输出：

```text
A B C F
```

这段代码的关键不是 `if` 的写法，而是**条件的顺序**。链式判断从上往下短路，第一个为真的分支直接返回。如果把 `score >= 60` 放到最前面，95 分会先命中它，输出 `C`，后面的 `>= 90`、`>= 80` 永远不可达。所以多分支条件的排列规则是：范围更窄、要求更高的条件放前面，兜底条件放最后。边界值也要逐个核对：`>= 90` 表示 90 分算 A，`> 90` 则 90 分落到 B；差一个等号，答案就不同。

真正让初学者栽跟头的是**悬垂 else**（dangling else）。C++ 的配对规则只有一句话：`else` 与**最近的、尚未配对的 `if`** 结合。缩进、换行、注释都不参与配对。看下面的程序：

```cpp
#include <iostream>

int main() {
    int a = 0, b = 1;

    std::cout << "first:[";
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdangling-else"
    if (a)
        if (b) std::cout << "inner";
        else std::cout << "inner-else";
#pragma clang diagnostic pop
    std::cout << "]\n";

    std::cout << "second:[";
    if (a) {
        if (b) std::cout << "inner";
    } else {
        std::cout << "outer-else";
    }
    std::cout << "]\n";
}
```

输出：

```text
first:[]
second:[outer-else]
```

第一段没有花括号，`else` 属于内层 `if (b)`，而不是外层 `if (a)`；因为 `a` 为 0，外层条件为假，整条内层 `if ... else ...` 被整体跳过，所以方括号里什么也没有。第二段用花括号把内层 `if` 包成一个整体，`else` 只能与外层 `if (a)` 配对；此时 `a` 为假，于是打印 `outer-else`。两段缩进看起来差不多，行为却完全不同，这就是"缩进会骗人"的经典例子。这段演示用 `#pragma clang diagnostic` 临时关掉了 `-Wdangling-else`，因为它的目的正是展示这个警告；真实代码应加花括号，而不是关警告。

避免这类错误的规则很朴素：**只要 `if` 或 `else` 的语句体里还有 `if`，就加花括号**，哪怕只有一行。花括号不改变逻辑，只固定配对关系，让后来加代码的人不会意外改变 `else` 的归属。考试中若问"输出什么"，先按"最近未配对"规则把 `else` 连到正确的 `if` 上，再代入数值，不要按缩进猜。

条件本身也有两个常见坑。第一，`if (p)` 等价于 `if (p != nullptr)`，`if (n)` 等价于 `if (n != 0)`，不要画蛇添足写 `if (flag == true)`，更不要写 `if (flag = true)`。第二，赋值表达式可以作为条件，但几乎总是笔误：

```cpp-fragment
int x = 5;
if (x = 0) {              // 赋值表达式的值是 0，条件恒假
    std::cout << "never";
}
```

编译器通常会给 `-Wparentheses` 警告（"using the result of an assignment as a condition"），看到这个警告就应该把 `=` 改回 `==`，或者给赋值加上一层显式括号并说明理由。`if (x == 0)` 与 `if (x = 0)` 只差一个字符，后果却是"判断"和"清空"的区别。

### 4.2 switch 语句

`switch` 把一个表达式的值与一组常量比较，跳到匹配的 `case` 继续执行。条件必须是整型或枚举类型（`char`、`bool` 也行），不能是 `std::string`、浮点数或数组；`case` 后面必须是常量表达式，且同一 `switch` 内不能有重复的常量。`default` 可选，位置任意，习惯放最后。

`switch` 的语义和 `if` 链有一个本质区别：**匹配到 `case` 之后，程序会一直往下执行，直到遇到 `break` 或 `switch` 结束**，这叫贯穿（fallthrough）。因此每个 `case` 末尾通常要写 `break`；如果确实想共用一段代码，可以把多个标签叠在一起，或者有意贯穿。

下面的例子用分组标签把元音和数字归到一起：

```cpp
#include <iostream>

void classify(char c) {
    switch (c) {
        case 'a': case 'e': case 'i': case 'o': case 'u':
            std::cout << c << ":vowel\n";
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            std::cout << c << ":digit\n";
            break;
        default:
            std::cout << c << ":other\n";
            break;
    }
}

int main() {
    classify('e');
    classify('7');
    classify('x');
}
```

输出：

```text
e:vowel
7:digit
x:other
```

五个元音标签共享同一段代码：`c` 等于其中任何一个都会落到 `std::cout` 那一行，然后 `break` 退出。数字标签同理。`default` 处理所有没列出的字符。如果删掉某个 `break`，比如元音分支的 `break`，那么 `'e'` 会先打印 `e:vowel`，再贯穿到数字分支打印 `7:digit` 之类的内容——不是你想要的结果。考试里"少写一个 `break`，输出什么"是最常见的送分题，也是最快的送命题：拿笔时在每个 `case` 末尾画个勾。

有时贯穿是故意的。比如按等级累加权益：

```cpp
#include <iostream>

int main() {
    int level = 2;
    switch (level) {
        case 3:
            std::cout << "gold ";
            [[fallthrough]];
        case 2:
            std::cout << "silver ";
            [[fallthrough]];
        case 1:
            std::cout << "bronze";
            break;
        default:
            std::cout << "none";
            break;
    }
    std::cout << '\n';
}
```

输出：

```text
silver bronze
```

`level == 2` 从 `case 2` 进入，打印 `silver`，然后有意贯穿到 `case 1` 打印 `bronze`。`[[fallthrough]];` 是 C++17 的属性，它告诉编译器和读者"这里往下走是故意的"，开启相应警告时不会报"隐式贯穿"。如果没有这个属性，代码行为一样，但维护者会怀疑你漏写了 `break`。C++ 里没有"自动 break"的开关，写 `switch` 时要把"是否需要贯穿"当成设计决策，而不是事后补丁。

`switch` 还有两个边界。第一，`case` 标签只是跳转目标，本身不形成独立作用域；如果在某个 `case` 里定义变量，而后面还有别的 `case`，编译器可能报"jump to case label crosses initialization"，因为跳转可能绕过变量初始化。解决办法是用花括号把该分支包起来，把变量限制在块内。第二，`switch` 只能比较相等，不能写范围；判断 `score >= 90` 这类条件要用 `if`，不要试图用 `switch` 硬凑。

一个经典反例是把字符串塞给 `switch`：

```cpp-fragment
std::string s = "ok";
switch (s) {              // 编译错误：switch 的条件必须是整型或枚举
    case "ok": break;
}
```

字符串不是整型，`case "ok"` 也不是整型常量，两处都会报错。要按字符串分派，只能用 `if` 链或查表。考试中给一段"看起来能跑"的 `switch`，先检查条件类型，再检查 `case` 是否常量、是否重复，最后检查 `break`。

### 4.3 while、do-while 与 for

三种循环的区别只在"条件什么时候判断"和"初始化、更新写在哪里"。

`while (条件) 语句` 是**先判断、后执行**，条件一开始就为假时循环体一次也不执行。它适合"不知道要循环多少次，只知道继续的条件"的场景，比如逐位拆数字。写 `while` 最常见的错误是忘记在循环体里更新循环变量，导致条件永远为真；另一个错误是更新语句被 `continue` 跳过，下面 4.5 节会看到。

`do 语句 while (条件);` 是**先执行、后判断**，循环体至少执行一次，末尾必须有分号。它适合"至少要做一次"的任务。统计整数位数是标准用例：

```cpp
#include <iostream>

int digits(unsigned int n) {
    int count = 0;
    do {
        ++count;
        n /= 10;
    } while (n != 0);
    return count;
}

int main() {
    std::cout << digits(0u) << ' ' << digits(7u) << ' '
              << digits(12345u) << '\n';
}
```

输出：

```text
1 1 5
```

如果这里用 `while (n != 0)`，`digits(0)` 会返回 0，但 0 明明有一位数字，必须额外特判。`do-while` 的"先做一次"正好覆盖这个边界。参数用 `unsigned int`，是因为负数取绝对值在 `INT_MIN` 上会溢出；把范围限制为非负，函数就只需要处理 0 和正数。

`for (初始化; 条件; 更新) 语句` 把循环的三要素集中在一行，最适合"次数明确"的循环。三段都可以为空：`for (;;)` 是死循环，`for (; i < n;)` 等价于 `while`。循环变量在 C++ 中的作用域仅限于 `for` 语句本身，循环结束后不能再访问。

边界是 `for` 最容易错的地方：

```cpp
#include <iostream>

int main() {
    int sum = 0;
    for (int i = 1; i <= 10; ++i) {
        sum += i;
    }
    std::cout << sum << '\n';

    for (int i = 3; i >= 1; --i) {
        std::cout << i;
        if (i > 1) std::cout << ' ';
    }
    std::cout << '\n';
}
```

输出：

```text
55
3 2 1
```

第一段想累加 1 到 10，所以条件是 `i <= 10`；如果写成 `i < 10`，总和是 45，少了 10。第二段倒序打印 3、2、1，条件是 `i >= 1`，最后执行的一次是 `i == 1`，再减到 0 时条件为假，循环结束。这里的 `i` 是 `int`，减到 0 再减也没关系；如果换成无符号类型，`i >= 0` 永远为真，循环会失控。

再看一个常见的符号比较问题：`std::vector` 的 `size()` 返回 `size_t`（本机 64 位无符号），若写 `for (int i = 0; i < v.size(); ++i)`，编译器会警告"comparison of integer expressions of different signedness"。要么把下标声明成 `std::size_t`，要么直接用范围 for。考试中看到 `i <= v.size()` 更要警惕：下标最大只能是 `size() - 1`，`<=` 会多访问一个元素，属于越界。

### 4.4 范围 for

范围 for 的写法是 `for (声明 : 范围) 语句`，它依次把范围中的每个元素绑定给声明，自动处理开始和结束，不需要下标。范围可以是数组、`std::vector`、`std::string` 等支持 `begin()`/`end()` 的对象，也可以是 `{1, 2, 3}` 这样的初始化列表。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};

    int sum = 0;
    for (const auto& x : v) {
        sum += x;
    }
    std::cout << sum << '\n';

    for (auto& x : v) {
        x *= 2;
    }
    for (const auto& x : v) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    std::vector<int> w{1, 2, 3, 4, 5, 6};
    for (int x : w) {
        if (x % 2 == 0) continue;
        if (x > 4) break;
        std::cout << '[' << x << ']';
    }
    std::cout << '\n';
}
```

输出：

```text
6
2 4 6
[1][3]
```

第一个循环用 `const auto&`，只读且不拷贝；`x` 是元素的引用，但加 `const` 后不能修改。第二个循环用 `auto&`，拿到可写引用，`x *= 2` 真正改到 `v` 里，所以再遍历时输出 2、4、6。如果写成 `for (auto x : v)`，`x` 是副本，改它不影响容器——这是范围 for 最隐蔽的错误：

```cpp-fragment
std::vector<int> v{1, 2, 3};
for (auto x : v) {
    x *= 2;               // 改的是副本，v 仍然是 1 2 3
}
```

第三个循环演示 `continue` 和 `break`：偶数跳过，大于 4 的数直接结束整个循环，所以只打印 `[1][3]`。注意这里的声明是 `int x`，拷贝一份也无所谓，因为只读。

范围 for 的边界有三条。第一，**遍历时不要改变容器的大小**：在循环里 `push_back`、`erase` 可能让迭代器失效，行为未定义；要增删元素，用下标循环并重新计算 `size()`，或者先收集再处理。第二，范围 for 的 `auto` 默认是拷贝，大对象（比如长字符串、大 `vector`）要写 `const auto&` 避免无谓复制。第三，需要下标或需要同时访问相邻元素时，普通 `for` 更直接，不要为了"看起来现代"硬套范围 for。

### 4.5 break、continue 与嵌套循环

`break` 立即结束**最近的** `switch` 或循环，跳到它后面继续执行；`continue` 跳过本次循环剩下的语句，直接进入下一次迭代。对 `for` 来说，`continue` 之后仍会执行"更新"那一段，所以 `++i` 不会丢；对 `while` 来说，如果更新写在 `continue` 后面，就会被跳过，循环变量永远不变，直接死循环。这是 `while` 比 `for` 更容易踩的坑。

`break` 只退出最内层。嵌套循环中想一次退出两层，常见做法是用一个标志变量，把标志写进外层条件：

```cpp
#include <iostream>

int main() {
    bool found = false;
    for (int i = 0; i < 3 && !found; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i * 3 + j == 4) {
                std::cout << i << ',' << j << '\n';
                found = true;
                break;
            }
        }
    }
    std::cout << (found ? "found" : "missing") << '\n';
}
```

输出：

```text
1,1
found
```

内层找到目标后把 `found` 置真并 `break`，只退出内层；外层条件 `i < 3 && !found` 下一轮为假，于是外层也结束。另一种写法是把嵌套循环封装成函数，找到就 `return`，用返回值告诉调用者；`goto` 也能跳出多层，但它会破坏结构化控制流，本课程只要求能读懂，不要求使用。

`continue` 在筛选型循环里很顺手：不满足条件的元素直接跳过，剩下的都是要处理的。但要注意它和 `break` 的区别：`continue` 只是"这次不做了"，`break` 是"以后都不做了"。考试中常把两者放进同一个循环，问最终输出，按迭代顺序一步步模拟即可。

### 4.6 作用域、static 局部变量与遮蔽

变量从声明点开始可见，到所在块结束为止，这叫块作用域。`for` 的初始化变量属于循环自己的作用域；`if`、`while`、`switch` 的花括号也各自形成作用域。内层块可以声明与外层同名的变量，这时内层变量**遮蔽**（shadow）外层变量，离开内层后外层变量恢复可见。遮蔽本身合法，但容易让读者误以为在改外层变量，实际改的是新变量。

```cpp
#include <iostream>

int counter = 0;

int next_id() {
    static int id = 0;
    return ++id;
}

int main() {
    std::cout << next_id() << ' ' << next_id() << ' ' << next_id() << '\n';

    ++counter;
    {
        int counter = 10;
        std::cout << counter << '\n';
    }
    std::cout << counter << '\n';
}
```

输出：

```text
1 2 3
10
1
```

`next_id` 里的 `static int id = 0;` 是**静态局部变量**：它在第一次执行到声明时初始化一次，之后函数返回也不销毁，下次调用继续用上一次的值，所以三次调用返回 1、2、3。普通局部变量每次调用都会重新创建，不可能记住上一次的值。静态局部变量的生命周期贯穿整个程序，初始化只发生一次；如果没写初值，它会被零初始化，这一点和普通局部变量（值不确定，读取是未定义行为）不同。

`main` 里的 `counter` 是全局变量，`++counter` 后为 1；内层块又声明了一个 `int counter = 10;`，它在块内遮蔽全局变量，所以打印 10；离开块后全局变量恢复可见，打印 1。这段代码能编译，但两个 `counter` 指的不是同一个对象，读代码时很容易看错。实际工程中应尽量避免同名遮蔽，编译器开启 `-Wshadow` 时也会提示。

全局变量和静态局部变量都有"隐藏状态"的问题：函数的输出不只取决于参数，还取决于之前调用过几次，测试和调试都更麻烦。初学阶段可以用静态局部变量做计数器、缓存这类小例子，但接口设计上更推荐把状态作为参数或对象成员显式传递。还要注意，全局变量的初始化顺序在多个编译单元之间没有完全统一的约定，跨文件依赖全局变量初始化是另一个深坑，这里不展开。

### 4.7 考试视角

控制流题目可以按"先结构、后边界、再类型"三步检查。

第一步看结构。`if`/`else` 的配对只看花括号和"最近未配对"规则，不看缩进；`switch` 的每个 `case` 先问"有没有 `break`"、"是不是故意贯穿"；循环先问"条件在入口还是出口判断"、"循环体至少执行几次"。

第二步看边界。循环的初始值、终止条件、更新方式三者要配套：想跑 1 到 n 就用 `i = 1; i <= n`，想跑 0 到 n-1 就用 `i = 0; i < n`；倒序且变量无符号时，`i >= 0` 永远成立，应改用 `i > 0` 配合先减，或直接用有符号类型。数组/`vector` 的下标上界是 `size() - 1`，写 `<= size()` 一定越界。`do-while` 至少执行一次，`while` 可能一次都不执行，题目问"循环体执行几次"时先分清是哪种。

第三步看类型。`switch` 的条件只能是整型或枚举，字符串、浮点都不行；`case` 必须是常量且不能重复；在 `case` 中定义变量又被后面的 `case` 跳过，会触发"crosses initialization"编译错误，需要加花括号。`for (int i = 0; i < v.size(); ++i)` 的符号比较警告不是错误，但说明类型不匹配，考试中可能被当作"隐患"来问。

最后记住两个高频反例：无符号倒序循环

```cpp-fragment
for (unsigned i = 3; i >= 0; --i) {   // i 减到 0 后再减，回绕成 UINT_MAX
    std::cout << i << ' ';
}
```

以及范围 for 改副本

```cpp-fragment
std::vector<int> v{1, 2, 3};
for (auto x : v) {
    x *= 2;               // v 不变，输出仍是 1 2 3
}
```

前者是死循环（无符号回绕有定义，但结果不是"到 -1 停止"），后者是"看起来改了、其实没改"。把这两段和 4.1 的悬垂 `else`、4.2 的漏写 `break` 放在一起复习，控制流的主要陷阱就齐了。

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

## 9. 字符、字符串与 string

C++ 没有内建的"字符串"类型。字符串在这个语言里是三层东西叠起来的：最底层 `char` 是一个 1 字节的整数；中间层是"一段连续 `char` 加一个结尾 `'\0'`"的 C 风格约定；最上层 `std::string` 用这个约定包出一个会自动管理内存的类。三层混在一起，正是期末卷面上"输出是什么"类题目失分最集中的地方，所以这一节按"从字节到对象"的顺序讲：先确认 `char` 只是小整数，再看 C 字符串的约定为什么危险，最后看 `std::string` 用哪三个字段把这个约定管住。

### 9.1 char 是整数，不是"字符"

`char` 是整数类型。标准只保证 `sizeof(char) == 1`，而 1 字节至少 8 位——这是 C++ 里唯一被硬性规定的 `sizeof` 值，其它类型的字节数都可以随平台变。字符字面量 `'A'` 的类型就是 `char`，值等于它在执行字符集里的编码；x86-64 上是 ASCII 兼容编码，所以 `'A'` 就是 65。换句话说，C++ 里根本没有"字符"这个独立的值，只有"约定用来表示字符的小整数"。

必须背下来的四段 ASCII：

| 范围 | 十进制 | 性质 |
| --- | --- | --- |
| `'0'`–`'9'` | 48–57 | 连续，`c - '0'` 得到数值 |
| `'A'`–`'Z'` | 65–90 | 连续 |
| `'a'`–`'z'` | 97–122 | 连续，与对应大写恒差 32 |
| `'\0'` | 0 | 字符串结束符，也叫 NUL |

背它的理由不是考试要默写，而是所有"字符 ↔ 数值"的代码都是减法：`c - '0'` 把数字字符变成数值，`c - 'a' + 'A'` 把大写变小写。一旦记错，代码照样编译、照样运行，只是结果是错的。

顺带说明类型家族：`char` 是"一个字节"，`wchar_t` 是"一个宽字符"（Windows 上 2 字节，Linux 上 4 字节），`char8_t`（C++20）专门用来装 UTF-8 码元，`char16_t`/`char32_t` 对应 UTF-16/UTF-32。它们之间**不会隐式转换**，`std::string` 只装 `char`，装 UTF-8 没问题（UTF-8 的码元就是一个字节），装 UTF-16 就得用 `std::u16string`。这一节只讨论 `char`，因为日常代码九成以上是它。

第二个必须知道的事实：**`char` 到底有没有符号是实现定义的**。x86-64 上的 clang/gcc 默认 `signed char`，取值范围 −128…127，所以把 200 这样的字节存进 `char` 会变成负数，`if (c > 0)` 之类的判断在碰到非 ASCII 字节时就错了。处理原始字节（编码转换、二进制协议、哈希）时应当显式用 `unsigned char` 或 `std::uint8_t`。`<cctype>` 里的 `isdigit`/`islower`/`toupper` 也有同样的要求：它们的参数必须是 `unsigned char` 能表示的值或 `EOF`，直接把负的 `char` 传进去是未定义行为。

```cpp
#include <cctype>
#include <iostream>
#include <string>

int main() {
    char c = 'A';
    std::cout << c << ' ' << int(c) << '\n';        // 字符与码值
    std::cout << ('a' - 'A') << '\n';               // 大写与小写的固定间距
    std::cout << char(c + ('a' - 'A')) << '\n';     // 用算术转小写

    char d = '7';
    std::cout << d << ' ' << int(d) << ' ' << (d - '0') << '\n';
    std::cout << (d == 7) << (d == '7') << '\n';    // 与数字 7 无关，与 '7' 相等

    std::string mixed = "a1B2c";
    int digits = 0, lowers = 0;
    for (char ch : mixed) {
        unsigned char u = static_cast<unsigned char>(ch);   // ctype 只接受无符号字节
        if (std::isdigit(u)) ++digits;
        if (std::islower(u)) ++lowers;
    }
    std::cout << digits << ' ' << lowers << '\n';
}
```

输出：
```text
A 65
32
a
7 55 7
01
2 2
```

第 4 行三个值分别是：字符 `7`、码值 55、`d - '0'` 得到的数值 7。第 5 行输出 `01`，说明 `d == 7` 为假而 `d == '7'` 为真——把 `'0'` 当成 0 是这一节最高频的错误，编译器不会给任何提示。

**考试视角**：`char c = '7'; int n = c;` 的答案永远是 55 而不是 7。判断口诀："字符是码值，减 `'0'` 才是数值。"

### 9.2 字面量、数组与 `'\0'` 约定

字符串字面量 `"abc"` 的类型是 `const char[4]`——三个可见字符加一个结尾的 `'\0'`。它有**静态存储期**（static storage duration），也就是说这块内存在程序启动时就分配好、一直活到程序结束，所以 `const char* p = "abc";` 把地址存下来、过很久再用是安全的。这也解释了为什么函数里 `return "abc";` 合法，而 `return 局部数组;` 不合法。

由此得到两条铁律，考试反复考：

- `strlen` 数的是 `'\0'` **之前**的字符数，不含结尾，复杂度 O(n)；
- `sizeof` 数的是整个数组占的字节数，含结尾，编译期就算好。

所以 `sizeof("abc") == 4` 而 `strlen("abc") == 3`；对 `char buf[8] = "ab";` 则 `sizeof(buf) == 8`（剩下的 5 个字节被自动补 0），`strlen(buf) == 2`。这里已经能看出一条重要的分界线：**数组类型自带长度，指针不带**。

数组在大多数表达式里会**退化**（decay）成指向首元素的指针：传参、参与算术、赋给指针时，长度信息就丢了。`void f(char s[8])` 与 `void f(char* s)` 是同一个函数，`sizeof(s)` 得到的是指针大小而不是 8。所以 C 风格接口必须再额外传一个长度参数，`strlen` 只能靠扫描 `'\0'` 来找长度——这就是所有 C 字符串函数都 O(n) 且不检查边界的原因：它们根本不知道缓冲区有多大。

还有两个语法细节经常出现在选择题里。第一，相邻的字面量会在编译期拼接：`"abc" "def"` 就是 `"abcdef"`，类型是 `const char[7]`，这是唯一一处"两个字符串相加"不需要函数的地方。第二，字面量是 `const` 的，所以 `"abc"[0] = 'x'` 编译不过；而 `char a[] = "abc";` 是**另一件事**——它用字面量的内容去初始化一个可写的栈数组，`a[0] = 'x'` 完全合法。区分标准是有没有发生拷贝：`const char*` 指向字面量本体，`char[]` 是复制出来的一份。

```cpp
#include <cstring>
#include <iostream>

int main() {
    const char* lit = "abc";
    std::cout << sizeof("abc") << ' ' << std::strlen(lit) << '\n';

    char buf[8] = "ab";                          // 剩余字节补 0
    std::cout << sizeof(buf) << ' ' << std::strlen(buf) << '\n';

    std::strcat(buf, "cd");
    std::cout << buf << ' ' << sizeof(buf) << ' ' << std::strlen(buf) << '\n';

    std::cout << (std::strcmp("abc", "abd") < 0) << '\n';   // 只看符号，不看具体值

    char raw[4] = {'a', 'b', 'c', 'd'};          // 没有 '\0'，不是字符串
    std::cout << sizeof(raw) << ' ' << raw[3] << '\n';
}
```

输出：
```text
4 3
8 2
abcd 8 4
1
4 d
```

`raw` 这一组是关键对照：它是个合法的 `char[4]`，但因为没有 `'\0'`，任何把它当字符串用的函数（`strlen`、`strcmp`、`operator<<`）都会一路读到数组外面去。**数组本身不是字符串，加上 `'\0'` 才是。**

❌ 下面两种写法都是教科书级别的错误：

```cpp-fragment
char* p = "abc";        // C++11 起 ill-formed：字面量是 const char[4]
p[0] = 'A';             // 即便用 const_cast 骗过编译器，也是写只读内存 → UB

char buf[4];
std::strcpy(buf, "hello");   // 6 字节写进 4 字节 → 越界写，UB
```

第一种在旧教材里常见，C++11 之后直接把 `const char[4]` 转成 `char*` 是非法的，现代编译器会报错；但用 `const_cast` 硬转过去仍然能编译，运行时可能段错误、也可能悄悄改掉别的常量。第二种更阴险：`strcpy` 不知道 `buf` 有多大，它会老老实实写 6 个字节，覆盖掉栈上相邻的变量——这类溢出是大量安全漏洞的源头，也是 `std::string` 存在的根本理由。

### 9.3 std::string 里到底存了什么

`std::string` 的本质是"把指针、长度、容量三个字段打包，并维护一条不变式"。概念上的内存布局：

```
栈上的 string 对象                     堆上的字符缓冲区
┌──────────────────┐
│ ptr  ────────────┼──────────────────> ┌───┬───┬───┬───┬───┬────┬─────────┐
│ size_  = 5       │                    │ H │ e │ l │ l │ o │ \0 │ 未使用  │
│ cap_   = 15      │                    └───┴───┴───┴───┴───┴────┴─────────┘
└──────────────────┘                     ↑ 有效字符 0..4   ↑ 标准保证这里一定是 '\0'
                                          └────────── capacity 个字节 ─────────┘
```

不变式有三条：`size_ <= cap_`；`[0, size_)` 是有效字符；`data()[size_]` 一定是 `'\0'`（C++11 起标准强制要求）。这三条直接决定了三件事：

1. `size()` 是 O(1)——读一个字段，不用扫描。这是 `std::string` 相对 `strlen` 最重要的性能优势。
2. **扩容会让指针、引用、迭代器全部失效**。`append` 发现 `size_ == cap_` 时会另分配一块更大的内存、把字符搬过去、释放旧的，原来的 `ptr` 就成了悬垂指针。所以不能把 `s.c_str()` 存起来跨越任何可能修改 `s` 的操作。
3. `std::string` 可以包含 `'\0'`，因为它用独立的 `size_` 记长度，`'\0'` 只是普通字符（9.5 展开）。

不要打印 `capacity()` 的具体数值：标准没有规定增长策略，libstdc++、libc++、MSVC 的实现各不相同，同一份代码在不同标准库上会给出不同的数。能依赖的只有"`capacity() >= size()`"和"`reserve(n)` 之后 `capacity() >= n`"这两条标准保证。

迭代器失效规则值得单独记一句：`std::string` 的 `push_back`/`append` 在没有触发扩容时只失效"末尾之后"的迭代器，一旦扩容就**全部**失效；`insert`/`erase` 则会让插入/删除点之后的迭代器失效。写循环时一边遍历一边 `+=`，是最常见的悬垂来源。真正的修法是先算好再改，或者用下标而不是迭代器。

另外多数实现都做了 **SSO**（small string optimization，小字符串优化）：在对象内部留一小块缓冲（常见 15 或 22 字节），短字符串不分配堆内存。所以"短字符串的 `data()` 指向对象内部、长字符串指向堆"是实现细节，不要拿地址做任何判断。

```cpp
#include <iostream>
#include <string>

int main() {
    std::string s(5, 'a');                       // "aaaaa"
    std::cout << s.size() << ' ' << (s.capacity() >= s.size()) << '\n';

    s.reserve(100);                              // 只影响容量，不改内容
    std::cout << s.size() << ' ' << (s.capacity() >= 100) << '\n';

    s.append(3, 'b');
    std::cout << s << ' ' << s.size() << ' ' << (s.capacity() >= s.size()) << '\n';

    std::string t = s;
    t += '!';
    std::cout << (s.size() < t.size()) << (s == t) << '\n';   // 深拷贝：各自独立
}
```

输出：
```text
5 1
5 1
aaaaabbb 8 1
10
```

`reserve(100)` 之后 `size` 仍是 5，说明容量和长度是两回事：容量是"已经买下的地"，长度是"真正种了东西的范围"。最后一行 `10` 是 `(s.size() < t.size())` 与 `(s == t)` 两个布尔值连着打印，证明 `string` 的拷贝是深拷贝——`t` 追加字符完全不影响 `s`，这与第 11 节讲的"自带资源的类必须深拷贝"是同一个道理。

### 9.4 常用操作与 npos

| 操作 | 写法 | 说明 |
| --- | --- | --- |
| 构造 | `std::string s = "hi";` | 从字面量拷贝 |
| 拼接 | `s += "x";`、`a + b` | 自动扩容，返回新串 |
| 长度 | `s.size()`、`s.empty()` | O(1) |
| 查找 | `s.find("x")` | 找不到返回 `npos` |
| 截取 | `s.substr(pos, len)` | 返回**新** string |
| 比较 | `s1 == s2`、`s1 < s2` | 按字节字典序 |
| 取字符 | `s[i]` / `s.at(i)` | 前者不检查，后者越界抛异常 |
| 转 C 串 | `s.c_str()` | 只读视图，不转移所有权 |

`npos` 的类型是 `std::string::npos`，值是 `size_t` 的最大值（本机为 18446744073709551615），它表示"没有这个位置"。之所以不用 -1，是因为 `size_t` 是无符号的，任何合法下标都不可能是最大值，用它当哨兵不会与真实位置冲突。

`substr(pos, len)` 的两个边界不一样，这是考点：

- `len` 超过剩余长度时**自动截断**，不报错；
- `pos > size()` 会抛 `std::out_of_range`，`pos == size()` 合法，返回空串。

```cpp
#include <iostream>
#include <string>

int main() {
    std::string s = "Hello";
    s += ", world";
    std::cout << s << '|' << s.size() << '\n';

    std::cout << s.substr(7, 5) << '\n';
    std::cout << s.substr(7, 100) << '\n';          // len 过长：截断
    std::cout << '[' << s.substr(s.size()) << ']' << '\n';   // 末尾：空串

    std::size_t p = s.find("world");
    std::cout << p << ' ' << (p != std::string::npos) << '\n';
    std::cout << (s.find("xyz") == std::string::npos) << '\n';

    std::cout << (std::string("abc") < std::string("abd")) << '\n';
    std::cout << (std::string("abc") < std::string("abcd")) << '\n';   // 前缀更短者小
}
```

输出：
```text
Hello, world|12
world
world
[]
7 1
1
1
1
```

最后两行说明了字典序的两条规则：逐字节比较，第一个不同的字节决定大小；若一个串是另一个的前缀，则短的更小。

❌ 最典型的 `npos` 错误：

```cpp-fragment
int n = s.find("x");          // ❌ find 返回 size_t，转 int 后 -1
if (n == std::string::npos)   // 永远不成立：-1 转成 size_t 变成最大值... 恰好相等？
```

这段代码在 64 位上是"碰巧能跑"的：`find` 返回 `SIZE_MAX` 后被截断成 `int` 的 -1，再和 `npos` 比较时 -1 又被转回 `SIZE_MAX`，于是恰好相等。但换到 `int n = s.find(...)` 返回一个大于 `INT_MAX` 的位置、或者把 `n` 存进 `long long` 再比较，结果就错了。正确写法只有一种：用 `std::size_t` 或 `auto` 接返回值，并且**直接和 `npos` 比**，不要和 -1 比。

### 9.5 长度与 `'\0'` 是两套边界

`std::string` 用 `size_` 决定"我有多长"，C 字符串函数用 `'\0'` 决定"我读到哪停"。两套边界在纯 ASCII 文本上恰好重合，一旦字符串内部出现 `'\0'` 就立刻分家。

```cpp
#include <cstring>
#include <iostream>
#include <string>

int main() {
    std::string s("a\0b", 3);                 // 显式长度：把 '\0' 当普通字符
    std::cout << s.size() << ' ' << s[0] << s[2] << '\n';

    std::cout << std::strlen(s.c_str()) << '\n';   // C 函数只认第一个 '\0'

    std::string t = "abc";
    t.push_back('\0');
    t += "de";
    std::cout << t.size() << ' ' << int(t[3]) << ' ' << t[4] << t[5] << '\n';
    std::cout << (t == "abc") << (t.size() == 3) << '\n';
}
```

输出：
```text
3 ab
1
6 0 de
00
```

`s.size()` 是 3、`s[2]` 是 `b`，而 `strlen(s.c_str())` 只数到 1——同一块内存，两个函数给出两个长度。`t` 的 `size()` 是 6，`t[4]`、`t[5]` 分别是 `d`、`e`，但 `t == "abc"` 为假（长度 6 对 3），说明 `string` 的比较也是按长度走的，不是按 `'\0'`。

这条性质的实际用途是承载二进制数据（比如读进来的图片字节流）：只要全程用 `std::string` 和带长度的接口，`'\0'` 完全无害；一旦调用 `.c_str()` 交给 `printf("%s")`、`strlen`、`strcmp`，就只剩第一段。**`c_str()` 只能在"立刻传给 C 函数"的场合用，不能长期保存**——它返回的指针会在下一次非 const 操作后失效（9.3 的不变式 2）。

### 9.6 string_view：不拥有的只读视图

`std::string_view` 只有两个字段：`const char* data_` 和 `size_`。它拷贝时只拷贝这两个字段（16 字节），不拷贝字符、不分配内存。因此它天生适合做**只读参数**：

```cpp-fragment
void print(std::string_view s);   // 既能接 std::string，也能接字面量，且零拷贝
```

如果参数写成 `const std::string&`，传字面量时会临时构造一个 `std::string`（一次堆分配）；写成 `const char*` 又丢了长度、还得处理 `'\0'`。`string_view` 两头都占。

代价是它**不拥有**数据，生命周期完全靠调用者保证。这是 C++ 里"视图类型"的共同特征：视图便宜，但必须比它指向的数据活得短。

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
    std::cout << first_word(line) << '|' << first_word("literal") << '\n';

    std::string_view sv = line;
    sv.remove_prefix(6);                      // 只移动起点，不复制字符
    std::cout << sv << ' ' << sv.size() << '\n';

    sv.remove_suffix(6);                      // 只缩小长度
    std::cout << sv << ' ' << sv.size() << '\n';

    std::cout << (sv.data() == line.data() + 6) << '\n';   // 同一个缓冲区，没有拷贝
}
```

输出：
```text
alpha|literal
beta gamma 10
beta 4
1
```

最后一行证明 `string_view` 只是"指针 + 长度"：`sv.data()` 恰好是 `line.data() + 6`。`remove_prefix`/`remove_suffix` 改的只是这两个字段，字符一个都没动。

两个限制必须记住：

1. `string_view` **没有 `c_str()`**。因为它常常是某个长字符串的一段，后面紧跟的不是 `'\0'`，直接传给 C 函数会读过界。需要 C 字符串时先转成 `std::string`。
2. `string_view` 不能保证所指内容可写，也**不能延长**所指对象的寿命。

❌ 悬垂的两种写法：

```cpp-fragment
std::string_view bad = std::string("temp");   // ❌ 临时 string 在语句结束就销毁
std::cout << bad;                             // 读已释放内存，UB

std::string_view pick() {
    std::string local = "abc";
    return local;                             // ❌ 返回后 local 析构，视图悬垂
}
```

这两种都能编译（`-Wall` 对第一种通常不报警），运行时可能打印出乱码、也可能"看起来正常"——这是最坏的一类 bug。判断标准只有一条：**视图的寿命必须严格短于被看对象的寿命**。

### 9.7 读输入：`>>` 与 `getline` 的分工

`std::cin >> s` 的算法是"跳过前导空白 → 一直读到下一个空白"，所以它天然读不了带空格的整行；`std::getline(in, line)` 读到 `'\n'` 为止，`'\n'` 被吃掉、不存进结果。两者混用有一个经典坑：`cin >> n` 之后输入流停在数字后面，换行符还留在缓冲区里，紧接着的 `getline` 立刻遇到它，返回**空串**。

修法两种：中间插一句 `std::cin.ignore()` 吃掉一个字符，或者写 `std::getline(std::cin >> std::ws, line)` 先跳过前导空白。下面用 `std::istringstream` 提供固定输入，这样不需要人工敲键盘、输出也能逐字复现：

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::istringstream in("42 hello world\nsecond line\n");

    int n = 0;
    in >> n;                                    // 停在 "42" 之后、'\n' 之前
    std::string rest;
    std::getline(in, rest);                     // 读到行尾，前导空格保留
    std::cout << n << '[' << rest << ']' << '\n';

    std::string line;
    std::getline(in, line);                     // 读完剩下的那一行
    std::cout << '[' << line << ']' << '\n';

    std::istringstream words("a b c");
    std::string w;
    int cnt = 0;
    while (words >> w) {                        // 流在耗尽后转 false，循环自然结束
        ++cnt;
        std::cout << w << '|';
    }
    std::cout << cnt << '\n';
}
```

输出：
```text
42[ hello world]
[second line]
a|b|c|3
```

第一行 `[ hello world]` 里那个前导空格就是证据：`>>` 只吃掉了 `42` 前面的空白，行内空格原样留给 `getline`。第三行演示 `while (in >> w)` 这个惯用法——`operator>>` 返回流的引用，流在 `bool` 上下文里表示"上一次读取是否成功"，读到结尾就变 false，不需要预先知道有几个词。

### 9.8 综合：按词切分并归一化

把前面几块拼起来：`istringstream` 分词、`string` 存词、`tolower` 归一化、`string_view` 做只读参数。

```cpp
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

std::string lower_copy(std::string_view sv) {
    std::string out;
    out.reserve(sv.size());
    for (char ch : sv) {
        out += char(std::tolower(static_cast<unsigned char>(ch)));
    }
    return out;
}

int main() {
    std::istringstream in("The Quick brown FOX jumps");
    std::string w, longest;
    int count = 0;
    while (in >> w) {
        std::string norm = lower_copy(w);
        ++count;
        if (norm.size() > longest.size()) longest = norm;   // 严格大于：保留先出现者
    }
    std::cout << count << ' ' << longest << ' ' << longest.size() << '\n';
}
```

输出：
```text
5 quick 5
```

输出是 `quick` 而不是 `brown` 或 `jumps`，因为比较用的是 `>`：三个长度都是 5，只有第一个被记下来。把 `>` 写成 `>=` 就会变成最后一个 `jumps`——这类"边界差一个等号"的题在卷面上极常见。

三种字符串表示的分工，可以用一张表收尾：

| 维度 | `char[]` / `const char*` | `std::string` | `std::string_view` |
| --- | --- | --- | --- |
| 拥有数据 | 视情况（栈数组/字面量） | 是，自动管理 | 否 |
| 长度 | `strlen` 每次 O(n) | `size()` O(1) | `size()` O(1) |
| 拷贝成本 | 取决于你怎么写 | 深拷贝全部字符 | 只拷贝指针+长度 |
| 能否含 `'\0'` | 否，会提前截断 | 能 | 能（但没 `c_str`） |
| `==` 语义 | 比地址（指针时） | 比内容 | 比内容 |
| 典型用途 | 与 C 接口交互、定长缓冲 | 拥有并修改字符串 | 只读参数、解析 |

**考试视角**：这一节的选择题几乎都落在四个点上——`sizeof` 含 `'\0'` 而 `strlen` 不含；`'0'` 是 48 不是 0；`find` 的返回值必须和 `npos` 比；`c_str()`/`string_view` 都是"借来的视图"，不能活得比原对象长。记住一句话：**C 字符串靠 `'\0'` 定边界，`std::string` 靠长度定边界，`string_view` 靠别人定边界。**

## 10. 结构体与类基础

`struct` 和 `class` 是把"一组数据"和"操作这组数据的函数"绑在一起的机制。但 C++ 的类真正区别于 C 结构体的地方不是成员函数，而是**构造与析构**：对象在创建和销毁时会自动执行代码，于是"申请资源"和"释放资源"可以绑在对象的生命周期上，这就是 RAII（Resource Acquisition Is Initialization）。这一节把零件逐个拆开：先看访问控制和内存布局，再看 `this`、构造析构的时机、初始化列表、`static` 与 `const`，最后用一个 RAII 例子收口。

### 10.1 struct 与 class 只差两个默认值

| 关键字 | 默认成员访问 | 默认继承方式 | 习惯用法 |
| --- | --- | --- | --- |
| `struct` | `public` | `public` | 纯数据聚合、POD |
| `class` | `private` | `private` | 有不变式、有资源 |

除了这两个默认值，两者在语言层面**完全等价**：`struct` 也能有成员函数、构造函数、虚函数、继承。选哪个纯粹是表达意图——看到 `struct` 就预期"这是可以随便读写的数据"，看到 `class` 就预期"这里有需要维护的规则"。

三种访问控制的分工：

| 说明符 | 谁能访问 | 典型用途 |
| --- | --- | --- |
| `public` | 任何人 | 对外接口 |
| `protected` | 本类与派生类 | 留给子类用的实现细节（第 12 节） |
| `private` | 仅本类成员与友元 | 数据成员、内部辅助函数 |

关键认知：`private` 是**编译期**的名字可见性检查，不是运行期的保护，更不改变内存布局。对象里那些字节一个不少，只是你在类外没法用名字碰它们。这也解释了为什么"私有成员不能被派生类访问"和"私有成员在内存里就在那"可以同时成立——访问控制是给编译器看的，内存布局是给 CPU 看的。

### 10.2 对象在内存里长什么样

一个类对象的内存 = 所有非静态数据成员（按**声明顺序**依次排布）+ 对齐填充（padding）+ 可能的虚表指针（有虚函数时才有，第 12 节）。编译器必须满足两条规则：每个成员的偏移是该成员对齐要求的整数倍；整个对象的大小是其最大对齐要求的整数倍（这样数组里每个元素都对齐）。

于是 `char` 后面跟 `int` 时，中间会插 3 个字节的空洞：

```
struct A { char c; int i; };            // 本机 x86-64 实测 sizeof(A) == 8

偏移   0     1     2     3     4     5     6     7
     ┌─────┬─────┬─────┬─────┬───────────────────────┐
     │  c  │填充 │填充 │填充 │        i（4 字节）      │
     └─────┴─────┴─────┴─────┴───────────────────────┘
       ↑ 成员 c 只占 1 字节，但它后面必须空到偏移 4，i 才能 4 字节对齐
```

把成员顺序换一下，同样的两个成员就能省下填充：

```
struct B { int i; char c; };            // sizeof(B) == 8：4 + 1 + 3 尾部填充
struct C { char a, b, c; };             // sizeof(C) == 3：全是 1 字节对齐，无填充
struct D { char a; char b; double d; }; // sizeof(D) == 16：1 + 1 + 6 填充 + 8
```

`B` 的大小和 `A` 一样是 8，但填充从"中间"挪到了"尾部"——尾部填充是为了让 `B[1]` 的 `i` 依然对齐。这也是一条实用的优化经验：**把大的成员放前面、小的放后面，通常能减少填充**。不过别为此打乱可读性，除非这个结构体真的被大量创建。

还有两个常考的特殊值：空类 `sizeof` 是 1（标准要求每个对象有唯一地址，不能是 0）；含虚函数的类会多一个指针大小（第 12 节）。注意所有这些具体数值都依赖平台（指针 8 字节、`int` 4 字节、`double` 8 字节对齐），下面例子里的数字是 x86-64 + clang 的实测结果，换平台要重新量。

```cpp
#include <cstddef>
#include <iostream>

struct A { char c; int i; };
struct B { int i; char c; };
struct C { char a, b, c; };
struct D { char a; char b; double d; };
struct E {};

int main() {
    std::cout << sizeof(A) << ' ' << offsetof(A, i) << '\n';
    std::cout << sizeof(B) << ' ' << offsetof(B, c) << '\n';
    std::cout << sizeof(C) << '\n';
    std::cout << sizeof(D) << ' ' << offsetof(D, d) << '\n';
    std::cout << sizeof(E) << '\n';
}
```

输出：
```text
8 4
8 4
3
16 8
1
```

`offsetof` 只能用于**标准布局**（standard-layout）类型：没有虚函数、所有非静态成员同一种访问权限、没有基类（或只有标准布局基类）。给多态类型用它，编译器会拒绝或给出实现定义的结果。

### 10.3 成员函数与 this

成员函数在可执行文件里**只有一份**，不属于任何对象。编译器给它加了一个隐藏参数 `this`，指向调用它的那个对象。所以 `p.size()` 在语义上约等于 `size_of_Point(&p)`：

```
class Point { int x_, y_; int sum() const; };

Point a, b;                     内存布局（每个对象 8 字节）
┌──────────┐  ┌──────────┐
│ x_ │ y_  │  │ x_ │ y_  │      sum 的代码只有一份，在代码段
└──────────┘  └──────────┘
     ↑              ↑
     └── this ──────┘           a.sum() → sum(&a)；b.sum() → sum(&b)
```

三条必记的性质：

- 普通成员函数里，`this` 的类型是 `Point* const`——指针本身不能改指向，但 `*this` 可以改。
- `const` 成员函数里，`this` 的类型是 `const Point*`，所以不能修改成员（见 10.8）。
- `static` 成员函数**没有** `this`，因此不能访问任何非静态成员（见 10.7）。

`this` 最常见的三个用途：区分同名参数（`this->x_ = x_;`）、返回自身引用以支持链式调用（`return *this;`）、在 `operator=` 里判断自赋值（第 11 节）。

因为成员函数只有一份代码，它在概念上等价于"第一个参数是 `T*` 的普通函数"，所以定义在类内的成员函数默认是 `inline` 的（只是建议，编译器可以拒绝），这也解释了为什么在头文件里写成员函数体不会导致重复定义。反过来，`&Point::sum` 这种成员函数指针并不是地址意义上的函数指针，调用时必须配一个对象，这正是"隐藏的 `this` 参数"在类型系统里的体现。

### 10.4 对象的一生：构造与析构的时机

构造函数与类同名、没有返回类型、可以重载；析构函数写作 `~T()`、无参数、无返回、不能重载。它们都不是"你调用"的，而是编译器在特定位置自动插入调用。一个对象的完整生命周期：

```
1. 分配 sizeof(T) 字节          ← 此时内容是随机的（栈上）或未构造的（堆上）
2. 成员按声明顺序构造            ← 无基类时从这里开始
3. 构造函数体执行                ← 到这里对象才真正可用
      ... 使用对象 ...
4. 析构函数体执行                ← 从最外层开始
5. 成员按声明逆序析构
6. 内存回收
```

构造顺序与析构顺序**严格镜像**，这不是约定而是标准规定，目的是保证"析构派生类时基类部分还活着"（第 12 节会看到基类排在最前、最后析构）。

析构的三个触发点：局部对象离开作用域（正常离开和提前 `return` 都算）、`delete` 一个指针、临时对象所在的完整表达式结束。这就是为什么清理代码应该写在析构函数里——只要对象被正确创建，无论从哪条路径离开作用域，清理都会执行。

临时对象的寿命值得单独说一句：`std::string("a") + "b"` 产生的临时串活到**整个完整表达式**结束（分号处），所以 `const std::string& r = a + b;` 这种绑定是安全的，生命周期被延长到 `r` 的作用域；但 `const std::string& r = (a + b).c_str();` 就不行了——`c_str()` 返回的是指针，临时串在分号处照样销毁，指针悬垂。区分点在于绑定的是"临时对象本身"还是"从临时对象里取出来的指针/引用成员"。

```cpp
#include <iostream>

struct Part {
    Part(char tag, int n) : tag_(tag), n_(n) { std::cout << "  ctor " << tag_ << n_ << '\n'; }
    ~Part() { std::cout << "  dtor " << tag_ << n_ << '\n'; }
    char tag_;
    int n_;
};

struct Machine {
    Part first{'A', 1};        // 声明在前的成员先构造
    Part second{'B', 2};
    Machine() { std::cout << "  ctor Machine\n"; }
    ~Machine() { std::cout << "  dtor Machine\n"; }
};

int main() {
    std::cout << "enter\n";
    {
        Machine m;
        std::cout << "  in scope\n";
    }
    std::cout << "exit\n";
}
```

输出：
```text
enter
  ctor A1
  ctor B2
  ctor Machine
  in scope
  dtor Machine
  dtor B2
  dtor A1
exit
```

注意 `Machine()` 的函数体打印在 `A1`、`B2` 之后：成员在进入函数体之前就已经构造完了。析构则完全倒过来，`Machine` 先走、成员后走、`B2` 先于 `A1`。

### 10.5 初始化列表：初始化，不是赋值

语法是构造函数名后接冒号，用逗号分隔的"成员(初值)"列表：

```cpp-fragment
Point(int x, int y) : x_(x), y_(y) { }
```

它与函数体内写 `x_ = x;` 有本质区别：

| 写法 | 实际发生的事 |
| --- | --- |
| `: x_(x)` | 用 `x` 直接**构造** `x_`，一步到位 |
| `{ x_ = x; }` | 先**默认构造** `x_`（类类型要构造一次），再调用 `operator=` |

对 `int` 只差一点效率，但对下面三类成员，**函数体内赋值根本无法编译**：

1. `const` 成员：没有赋值运算符，只能在初始化时给值；
2. 引用成员：必须在创建时绑定，之后不能重新绑定；
3. 没有默认构造函数的类类型成员（以及基类子对象）。

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
    const int id_;
    int& total_;
};

int main() {
    int alive = 0;
    {
        Sensor a(1, alive);
        Sensor b(2, alive);
        std::cout << "alive=" << alive << '\n';
    }
    std::cout << "after=" << alive << '\n';
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

`alive` 是两个对象共享的计数器，通过引用绑定：两个对象都加到同一个变量上，析构时没有减它，所以最后是 2。

⚠️ **顺序陷阱**：成员按**声明顺序**初始化，与你在列表里写的顺序无关。下面这种写法看起来"先算 b 再算 a"，实际 `a_` 声明在前、先初始化：

```cpp-fragment
struct Bad {
    int a_;                                 // 声明在前 → 先初始化
    int b_;
    Bad(int x) : b_(a_ * 2), a_(x) { }      // ❌ 读未初始化的 a_，UB
};
```

`clang++ -Wall` 会对"列表顺序与声明顺序不一致"给出 `-Wreorder-ctor`，对"用未初始化成员初始化另一个成员"给出 `-Wuninitialized`。但只有开了警告才看得见，所以规则得自己记住：**列表里的顺序照抄声明顺序**，需要依赖关系时宁可写进函数体。

### 10.6 默认构造与零初始化

**只要声明了任何一个构造函数，编译器就不再隐式生成默认构造函数。** 这是初学者遇到最多的编译错误：

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

补救两种写法：`Point() = default;`（显式要回编译器生成的版本），或者给参数加默认值 `Point(int x = 0, int y = 0)`（这个构造函数同时充当默认构造）。注意 `= default` 放在类内是"我要编译器生成的版本"，它仍然受成员默认初始化器影响。

另一个必须分清的区别：

| 写法 | 对类类型 | 对内建类型成员 |
| --- | --- | --- |
| `T t;` | 调用默认构造 | **不初始化**，值不确定 |
| `T t{};` | 调用默认构造 | 零初始化 |
| `new T` | 调用默认构造 | 不初始化 |
| `new T()` | 调用默认构造 | 零初始化 |

"值不确定"不是"等于 0"：读它是未定义行为，可能读到栈上残留的任何东西，而且同一份代码在 `-O0` 和 `-O2` 下结果可能不同。这是"看起来能跑"的最坏情况。

```cpp
#include <iostream>

struct Plain { int a; int b; };

class Point {
public:
    Point() = default;                      // 显式要回默认构造
    Point(int x, int y) : x_(x), y_(y) {}
    int x() const { return x_; }
    int y() const { return y_; }
private:
    int x_ = 0;                             // 默认成员初始化器
    int y_ = 0;
};

int main() {
    Plain p{};
    std::cout << p.a << ' ' << p.b << '\n';

    Plain* q = new Plain();                 // 值初始化：清零
    std::cout << q->a << ' ' << q->b << '\n';
    delete q;

    Point a;                                // 走 = default 的默认构造
    Point b(3, 4);
    std::cout << a.x() << a.y() << ' ' << b.x() << b.y() << '\n';
}
```

输出：
```text
0 0
0 0
00 34
```

`Point a;` 的两个成员是 0，靠的是默认成员初始化器 `= 0`，不是 `= default` 本身——`= default` 只是"生成那个什么都不做的构造函数"，是初始化器把值填上的。

### 10.7 static 成员

`static` 数据成员属于**类**而不属于对象：整类只有一份，所有对象共享，`sizeof(对象)` 里不算它。它必须在类外定义（分配存储），C++17 起可以用 `inline static` 直接在类内定义。

`static` 成员函数没有 `this`，因此不能访问非静态成员，也不能声明为 `const`（`const` 修饰的是 `this` 指向的对象，而它根本没有 `this`）。调用时写 `Counter::live()` 或 `obj.live()` 都行，但前者更能表达"这与某个具体对象无关"。

```cpp
#include <iostream>

class Counter {
public:
    Counter() { ++created_; ++live_; }
    Counter(const Counter&) { ++created_; ++live_; }
    ~Counter() { --live_; }
    static int created() { return created_; }
    static int live() { return live_; }
private:
    static int created_;       // 声明
    static int live_;
};

int Counter::created_ = 0;     // 定义，全程序只能有一处
int Counter::live_ = 0;

int main() {
    std::cout << Counter::created() << ' ' << Counter::live() << '\n';
    {
        Counter a, b;
        Counter c = a;                       // 拷贝构造同样计数
        std::cout << Counter::created() << ' ' << Counter::live() << '\n';
    }
    std::cout << Counter::created() << ' ' << Counter::live() << '\n';
}
```

输出：
```text
0 0
3 3
3 0
```

`created` 只增不减、`live` 随作用域归零——这两个计数器正好对应"累计创建"和"当前存活"两个概念，也是后面讲拷贝/移动时判断有没有多删、漏删的常用手段。

### 10.8 const 成员函数与 mutable

在参数表后面加 `const`，作用是把 `this` 变成 `const T*`，向调用者承诺"这个函数不修改对象状态"：

```cpp-fragment
int size() const { return size_; }
```

两个作用：`const` 对象、`const` 引用、`const` 指针**只能**调用 `const` 成员函数；同时编译器会替你检查承诺——在 `const` 函数里改成员直接编译错。所以 `const` 不是注释，是可执行的契约。

`const` 与非 `const` 版本可以重载，编译器按对象的 const 性质选择。这常用来实现"只读访问返回只读引用、可写访问返回可写引用"：

```cpp
#include <iostream>
#include <string>
#include <utility>

class Text {
public:
    explicit Text(std::string s) : s_(std::move(s)) {}
    char& at(std::size_t i) { std::cout << "non-const "; return s_[i]; }
    const char& at(std::size_t i) const { std::cout << "const "; return s_[i]; }
    std::size_t size() const { return s_.size(); }
private:
    std::string s_;
};

int main() {
    Text t("abc");
    t.at(0) = 'X';                            // 非 const 对象 → 非 const 版本
    std::cout << t.at(1) << '\n';             // 仍然是非 const 版本

    const Text& ct = t;
    std::cout << ct.at(1) << ' ' << ct.size() << '\n';
}
```

输出：
```text
non-const non-const b
const b 3
```

`t.at(1)` 明明只读，仍然选中了非 const 版本——重载决议只看对象的 const 性质，不看你要不要改。如果确实需要在 `const` 函数里修改某个成员（缓存、命中计数、惰性求值），把它声明为 `mutable`：

```cpp-fragment
class Cache {
public:
    int get() const { ++hits_; return value_; }   // const 函数里改 mutable 成员：合法
private:
    int value_ = 42;
    mutable int hits_ = 0;
};
```

`mutable` 是"逻辑 const、物理不 const"的逃生口，但它破坏 const 的可推理性和线程安全，只在缓存这类"对外不可见"的成员上用。

### 10.9 聚合与指定初始化器

**聚合**（aggregate）大致要求：所有非静态数据成员都是 `public`、没有用户声明的构造函数、没有虚函数、没有 `private`/`protected` 基类。对聚合可以按声明顺序用花括号初始化，缺的成员用默认值补：

```cpp-fragment
struct Point { int x; int y; };
Point p{1, 2};
```

C++20 的**指定初始化器**（designated initializer）允许写成员名，可读性更好，但有硬性限制：必须按声明顺序、不能跳过中间成员再回头、不能与位置初始化混用。违反任何一条都是编译错误。

```cpp
#include <iostream>
#include <string>

struct Endpoint {
    std::string host;
    int port = 80;
};

struct Config {
    std::string name;
    Endpoint ep;
    bool verbose = false;
};

int main() {
    Config a{"server", {"example.com", 8080}, true};
    Config b{};
    Config c{.name = "db", .ep = {"localhost", 5432}};

    std::cout << a.name << ' ' << a.ep.host << ':' << a.ep.port << ' ' << a.verbose << '\n';
    std::cout << '[' << b.name << "] " << b.ep.host << ':' << b.ep.port << ' ' << b.verbose << '\n';
    std::cout << c.name << ' ' << c.ep.host << ':' << c.ep.port << ' ' << c.verbose << '\n';
}
```

输出：
```text
server example.com:8080 1
[] :80 0
db localhost:5432 0
```

`b{}` 得到空名字、空主机、端口 80——这证明 `{}` 不是"把内存清零"，而是"每个成员各自用默认值初始化"：`std::string` 默认构造为空串，`int port = 80` 用默认成员初始化器，`bool` 用 `= false`。

### 10.10 常见坑与 RAII 收尾

**坑 1：返回局部对象的引用或指针。**

```cpp-fragment
const std::string& bad() {
    std::string local = "tmp";
    return local;              // ❌ 函数返回后 local 已析构
}
```

返回引用/指针的前提是**被引用对象的生命周期长于调用者**：返回成员、返回 `static` 变量、返回参数传进来的对象都可以；返回局部变量一定错。`-Wall` 会给 `-Wreturn-stack-address`，但这个警告只在直接返回 `local` 时出现，经过一层函数调用或存进成员就未必报。

**坑 2：读未初始化的成员。** `struct S { int a; }; S s; std::cout << s.a;` 输出不确定值。`S s{};`、`int a = 0;`、构造函数里初始化，三选一。

**坑 3：以为列表顺序决定初始化顺序**（见 10.5）。**坑 4：在构造函数里调用虚函数**，此时派生类部分还没构造，只会调到当前层的版本（第 12 节展开）。

最后把这一节的零件拼成一个 RAII 例子——资源在构造时获取、在析构时释放，中间无论从哪条路径离开作用域都不会漏：

```cpp
#include <iostream>

class Tracer {
public:
    explicit Tracer(const char* name) : name_(name) { std::cout << "+" << name_ << '\n'; }
    ~Tracer() { std::cout << "-" << name_ << '\n'; }
    Tracer(const Tracer&) = delete;              // 不允许复制：一个对象管一份资源
    Tracer& operator=(const Tracer&) = delete;
private:
    const char* name_;
};

int run(int mode) {
    Tracer t("run");
    if (mode == 0) return 10;                    // 提前 return：仍然析构
    Tracer u("extra");
    return 20;
}

int main() {
    std::cout << run(0) << '\n';
    std::cout << run(1) << '\n';
}
```

输出：
```text
+run
-run
10
+run
+extra
-extra
-run
20
```

`run(0)` 里只有 `t` 被构造，提前 `return` 时它照样析构；`run(1)` 里 `u` 后构造先析构。如果把清理代码写在函数末尾而不是析构函数里，`mode == 0` 那条路径就会漏掉——这正是 RAII 的价值：**把"一定执行"交给语言规则，而不是交给每一条 return 路径**。

**考试视角**：这一节的题集中在四组判断上——`sizeof` 里有没有填充（按对齐规则算，不是成员大小相加）；构造/析构的先后顺序（构造正序、析构逆序，成员按声明顺序）；`T t;` 与 `T t{};` 的差别（后者零初始化）；`const` 成员函数能不能被 `const` 对象调用（能，反过来不行）。口诀：**构造从里到外、析构从外到里；列表是初始化、体内是赋值；声明顺序说了算。**

## 11. 拷贝、移动与运算符重载

一个对象从"另一个同类型对象"被创建（拷贝构造）或被赋值（拷贝赋值）时，编译器默认会生成逐成员的版本。成员全是值时这是完全正确的；但只要类里有一个"拥有资源"的裸指针，默认版本就会让两个对象指向同一块内存，析构两次就崩。这一节按"默认行为 → 为什么错 → 怎么修 → 怎么更快"的顺序讲：先看深浅拷贝与三/五法则，再看移动语义，最后看运算符重载的返回值约定。

### 11.1 编译器替你生成的成员函数

如果你什么都不写，编译器会隐式生成（并 `public`、`inline`）：

| 函数 | 签名 | 默认行为 |
| --- | --- | --- |
| 默认构造 | `T()` | 对每个成员调用默认构造 |
| 析构 | `~T()` | 对每个成员调用析构 |
| 拷贝构造 | `T(const T&)` | 对每个成员调用**拷贝构造** |
| 拷贝赋值 | `T& operator=(const T&)` | 对每个成员调用**拷贝赋值** |
| 移动构造 | `T(T&&)` | 对每个成员调用移动构造（C++11 起） |
| 移动赋值 | `T& operator=(T&&)` | 对每个成员调用移动赋值 |

"逐成员"这三个字是理解一切的关键：对 `int`、`double` 就是复制值；对 `std::string` 这类自己写好了深拷贝的类，就是调用它正确的拷贝构造；对 `int*`，**只复制指针的值**——也就是地址，不复制它指向的内容。编译器不知道这个指针是"拥有的资源"还是"借来的观察者"，它只会做最机械的事。

所以"要不要自己写"的判断标准不是"有没有指针"，而是"这个类是否**拥有**资源、析构函数是否要释放它"。拥有资源的类必须自己定义拷贝语义，因为默认语义与析构函数的 `delete` 冲突。

### 11.2 浅拷贝、深拷贝与 double free

```
浅拷贝（编译器默认：逐成员）                深拷贝（手写拷贝构造）

a ┌───────────┐                            a ┌───────────┐
  │ n = 5     │                              │ n = 5     │
  │ p ────────┼──┐                           │ p ────────┼──> ┌─┬─┬─┬─┬─┬──┐
  └───────────┘  │                           └───────────┘    │h│e│l│l│o│\0│
b ┌───────────┐  │                           b ┌───────────┐    └─┴─┴─┴─┴─┴──┘
  │ n = 5     │  │                             │ n = 5     │
  │ p ────────┼──┘  ← 指向同一块                │ p ────────┼──> ┌─┬─┬─┬─┬─┬──┐
  └───────────┘                                └───────────┘    │h│e│l│l│o│\0│
                                                               └─┴─┴─┴─┴─┴──┘
· 改 b 的内容，a 跟着变                       · 改 b 的内容，a 不变
· 两个对象析构时 delete 同一个地址 → double free（UB）   · 各删各的，互不影响
```

double free 之所以是未定义行为而不是"稳定崩溃"，是因为堆分配器在每块内存**前面**存了管理信息（块大小、链表指针、是否空闲）。第一次 `free` 把这些信息改了，第二次 `free` 同一地址时分配器读到的是已经被改写过的元数据，于是它可能：

- 立刻 `abort`（glibc 的 "double free or corruption"）；
- 把同一块内存又放回空闲链表，之后两次 `new` 拿到同一个地址，两个对象互相踩；
- 什么都不发生，直到程序退出或下一次分配才崩——崩的地方和真正的错误毫无关系。

第三种最可怕：程序"看起来正常"，一换编译器、一开优化、一加日志就崩。它是 UB，标准不承诺任何行为，所以"在我机器上能跑"不能作为正确性证据。

```cpp
#include <iostream>

struct Shallow {
    explicit Shallow(int v) : p(new int(v)) {}
    // 没有自己写拷贝构造、析构 → 逐成员拷贝：两个对象存同一个地址
    int* p;
};

int main() {
    Shallow a(7);
    Shallow b = a;                       // 浅拷贝：只复制指针值
    std::cout << (a.p == b.p) << '\n';
    *b.p = 9;
    std::cout << *a.p << '\n';           // 改 b 影响 a：同一块内存
    delete a.p;                          // 只删一次，避免真的 double free
}
```

输出：
```text
1
9
```

这个程序刻意**没有**给 `Shallow` 写析构函数：一旦写上 `~Shallow() { delete p; }`，`a` 和 `b` 会各删一次同一个地址，程序在 `main` 结束时进入 UB。所以真实的 bug 形态是"加了一个看起来天经地义的析构函数，程序就开始随机崩"，而不是"用了指针就崩"。

### 11.3 Rule of Three：三个函数要写就一起写

**如果你需要自己写析构函数（说明你在管理资源），那么拷贝构造和拷贝赋值几乎肯定也需要自己写。** 反过来，如果三个都不需要，就一个都别写。这叫 Rule of Three（三法则），因为它来自 C++98 的三个特殊成员函数。

| 函数 | 签名 | 何时被调用 |
| --- | --- | --- |
| 拷贝构造 | `T(const T& other)` | `T b = a;`、按值传参、按值返回（未省略时） |
| 拷贝赋值 | `T& operator=(const T& other)` | `b = a;`（`b` 已存在） |
| 析构 | `~T()` | 生命周期结束、`delete` |

两个必须记住的细节：

1. 拷贝构造的参数**必须是引用**。写成 `T(T other)` 会因为按值传参又要拷贝一次，无限递归，标准直接把这种签名判定为错误（编译器报 "copy constructor must pass its first argument by reference"）。
2. 拷贝赋值要返回 `T&`，指向 `*this`，这样才能写 `a = b = c`（右结合，先算 `b = c` 得到 `b` 的引用，再赋给 `a`）。返回 `void` 或 `T` 都不会编译错，但前者不能链式、后者多一次拷贝。

自赋值是 Rule of Three 里最容易漏的分支：`a = a;` 看着荒谬，但 `v[i] = v[j]` 在 `i == j` 时就发生了。如果赋值运算符写成"先 `delete` 旧资源，再从参数拷贝"，自赋值时参数指向的正是刚被删掉的内存——读到已释放内存，UB。两种修法：

```cpp-fragment
// 修法一：开头判自赋值
Str& operator=(const Str& o) {
    if (this == &o) return *this;
    ...
}

// 修法二：先分配新的，成功后再释放旧的（顺带获得强异常安全）
char* np = new char[o.n_ + 1];   // 这一步失败会抛 bad_alloc，但自己没有变
std::memcpy(np, o.p_, o.n_ + 1);
delete[] p_;
p_ = np;
```

修法二的含义值得展开：`new` 抛异常时，对象仍指向原来的缓冲区，状态完好，这满足**强异常安全保证**（要么成功，要么回滚）。而"先 delete 再 new"的写法一旦 `new` 失败，对象就处在一个"指针已释放、长度还在"的破碎状态，析构时会再删一次。

```cpp
#include <cstring>
#include <iostream>

class Str {
public:
    explicit Str(const char* s = "") : n_(std::strlen(s)), p_(new char[n_ + 1]) {
        std::memcpy(p_, s, n_ + 1);
        ++live_;
    }
    Str(const Str& o) : n_(o.n_), p_(new char[o.n_ + 1]) {     // 深拷贝
        std::memcpy(p_, o.p_, n_ + 1);
        ++live_;
        std::cout << "copy " << p_ << '\n';
    }
    Str& operator=(const Str& o) {
        if (this == &o) return *this;                          // 自赋值：直接返回
        char* np = new char[o.n_ + 1];                         // 先分配，后释放
        std::memcpy(np, o.p_, o.n_ + 1);
        delete[] p_;
        p_ = np;
        n_ = o.n_;
        std::cout << "assign " << p_ << '\n';
        return *this;
    }
    ~Str() { delete[] p_; --live_; }

    static int live() { return live_; }
    const char* c_str() const { return p_; }
    std::size_t size() const { return n_; }
    void set_first(char c) { p_[0] = c; }
private:
    std::size_t n_;
    char* p_;
    static int live_;
};
int Str::live_ = 0;

void assign_to(Str& dst, const Str& src) { dst = src; }        // 让自赋值"隐身"

int main() {
    Str a("hello");
    Str b = a;
    b.set_first('J');
    std::cout << a.c_str() << ' ' << b.c_str() << '\n';

    {
        Str c("x");
        c = a;
        std::cout << c.c_str() << ' ' << c.size() << '\n';
        assign_to(c, c);                                       // 两个引用指向同一对象
        std::cout << c.c_str() << '\n';
    }
    std::cout << Str::live() << '\n';
}
```

输出：
```text
copy hello
hello Jello
assign hello
hello 5
hello
2
```

`hello Jello` 是深拷贝的证据：改 `b` 的第一个字符，`a` 完全没变。如果漏写拷贝构造，这一行会变成 `Jello Jello`。`live()` 最后是 2，说明确实只漏掉了 `a`、`b` 两个对象，没有多删——加上 `live_` 这类计数是调试资源类最有效的手段。

### 11.4 左值、右值与 std::move 的真相

C++11 用**值类别**（value category）区分"有名字、能取地址的对象"和"马上就要销毁的临时对象"：

| 类别 | 例子 | 能否取地址 | 绑定到 |
| --- | --- | --- | --- |
| 左值（lvalue） | 变量 `a`、`*p`、`arr[0]` | 能 | `T&` |
| 将亡值（xvalue） | `std::move(a)`、`static_cast<T&&>(a)` | 能 | `T&&` |
| 纯右值（prvalue） | `1`、`a + b`、`T()` | 不能 | `const T&` 或 `T&&` |

右值引用 `T&&` 的作用是让重载决议能区分"源对象可以被掏空"的情况。移动构造 `T(T&&)` 比拷贝构造 `T(const T&)` 更匹配右值，于是临时对象、`return` 的局部变量、显式 `std::move` 的变量都会走到移动版本。

`std::move(x)` **不移动任何东西**。它的实现就是一个 `static_cast<T&&>(x)`：把左值标记成右值，好让重载决议选中移动版本。真正搬资源的是移动构造/移动赋值里的代码。所以：

- `std::move(a);` 单独一行什么也没发生，`a` 原封不动；
- `const T a; T b(std::move(a));` 会**静默退化成拷贝**——因为 `const T&&` 不能绑定到 `T&&`，只能匹配 `const T&`；
- 对内置类型（`int`、指针）用 `std::move` 就是一次拷贝，没有收益。

还有一个会让"拷贝次数"对不上的语言规则：**复制省略**（copy elision）。`T b = a;` 里 `a` 是左值，必须真正调用拷贝构造；但 `return T(...)`、`T b = T(...)`、按值传参传临时对象这些场合，编译器可以直接在目标位置构造，连移动都不发生。C++17 起其中一部分是**强制**的（prvalue 直接初始化目标对象），所以数拷贝次数的题目要先判断有没有省略，不能想当然地按"一次构造 + 一次拷贝"去数。

移动之后源对象处于"**有效但未指定**"（valid but unspecified）状态：可以析构、可以重新赋值、可以调用不依赖具体值的成员（比如 `clear()`），但不要读它的值来推断业务含义。自己实现的移动操作通常把源置成"空"（`p = nullptr`、`n = 0`），这样源析构时 `delete nullptr` 是安全的——标准保证 `delete nullptr` 什么也不做。

```
移动前                                  移动后

a ┌────────────────┐                    a ┌────────────────┐
  │ n = 5          │                      │ n = 0          │
  │ p ──> [int×5]  │                      │ p = nullptr    │
  └────────────────┘                    └────────────────┘

b ┌────────────────┐                    b ┌────────────────┐
  │ n = 0          │                      │ n = 5          │
  │ p = nullptr    │                      │ p ──> [int×5]  │
  └────────────────┘                    └────────────────┘

没有复制任何元素，只是两个字段换了主人；a 变成"空壳"，只能析构或重新赋值。
```

```cpp
#include <iostream>
#include <utility>

class Buf {
public:
    explicit Buf(std::size_t n) : n_(n), p_(new int[n]) {
        for (std::size_t i = 0; i < n_; ++i) p_[i] = int(i);
        std::cout << "ctor n=" << n_ << '\n';
    }
    Buf(Buf&& o) noexcept : n_(o.n_), p_(o.p_) {          // 只搬字段，没有 new
        o.n_ = 0;
        o.p_ = nullptr;
        std::cout << "move-ctor n=" << n_ << '\n';
    }
    Buf& operator=(Buf&& o) noexcept {
        if (this == &o) return *this;
        delete[] p_;                                       // 先释放自己原有的
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
    Buf&& ref = std::move(a);                 // 只是类型转换：a 一个字节都没动
    std::cout << "after cast a=" << a.size() << '\n';

    Buf b(std::move(ref));                    // 这里才真正搬走
    std::cout << "a=" << a.size() << " b=" << b.size() << '\n';

    Buf c(1);
    c = std::move(b);
    std::cout << "b=" << b.size() << " c=" << c.size() << '\n';
}
```

输出：
```text
ctor n=4
after cast a=4
move-ctor n=4
a=0 b=4
ctor n=1
move-assign n=4
b=0 c=4
```

`after cast a=4` 是整节最重要的一行：`std::move` 之后 `a` 依然完整，直到移动构造被调用才被掏空。`a=0 b=4` 说明资源确实转移了；移动构造里**没有** `new`，那块缓冲区自始至终只分配过一次。

### 11.5 noexcept、vector 扩容与 Rule of Five

移动操作应当标 `noexcept`，这不只是文档性质，而是有硬性后果的：`std::vector` 扩容时要把旧元素搬到新缓冲区，它用 `std::move_if_noexcept` 决定策略——**如果移动构造可能抛异常、而拷贝构造存在，就退回去用拷贝**，因为只有拷贝能提供强异常安全保证（搬一半失败时旧数组还完好）。忘了 `noexcept`，你的"移动"在 `vector` 里会静默变成拷贝，性能优化完全失效。

加上移动构造和移动赋值就得到 Rule of Five（五法则）：

| 函数 | 典型实现要点 |
| --- | --- |
| 析构 | 释放资源 |
| 拷贝构造 | 新分配 + 复制内容，源不变 |
| 拷贝赋值 | 判自赋值、先分配后释放、返回 `*this` |
| 移动构造 | 偷指针、把源置空、`noexcept` |
| 移动赋值 | 释放自己、偷指针、置空源、判自赋值、`noexcept` |

`= default` 表示"我要编译器生成的那个版本"，`= delete` 表示"明确禁用"。被删除的函数**参与重载决议**，所以调用时报的是"使用了被删除的函数"，比"找不到匹配函数"清楚得多。

🚨 **隐式删除规则**是这一节最容易翻车的地方。声明某些特殊成员会让编译器**不再生成**其它的：

| 你声明了 | 不再生成 | 后果 |
| --- | --- | --- |
| 拷贝构造 | 移动构造、移动赋值 | 移动退化成拷贝 |
| 移动构造 | 拷贝构造、拷贝赋值（定义为 `= delete`） | 对象只能移动，不能拷贝 |
| 析构函数 | 移动构造、移动赋值 | 移动退化成拷贝 |
| 拷贝赋值 | 移动构造、移动赋值 | 同上 |

注意第二行：只要写了移动构造（哪怕 `= default`），拷贝就被**删除**了，"我只想加个移动"的类会突然不能拷贝，报错信息还很绕。所以五件套要么全写清楚，要么一个都别碰。

```cpp
#include <iostream>
#include <utility>
#include <vector>

class Owner {
public:
    explicit Owner(int v) : v_(v) {}
    Owner(const Owner&) = delete;                  // 资源唯一，禁止拷贝
    Owner& operator=(const Owner&) = delete;
    Owner(Owner&& o) noexcept : v_(o.v_) { o.v_ = -1; }
    Owner& operator=(Owner&& o) noexcept {
        v_ = o.v_;
        o.v_ = -1;
        return *this;
    }
    int value() const { return v_; }
private:
    int v_;
};

int main() {
    std::vector<Owner> v;
    v.reserve(4);                                  // 预留后不会重分配
    v.push_back(Owner(1));
    v.push_back(Owner(2));
    std::cout << v[0].value() << v[1].value() << '\n';

    Owner a(5);
    Owner b(std::move(a));                         // 只能移动
    std::cout << a.value() << ' ' << b.value() << '\n';
}
```

输出：
```text
12
-1 5
```

`Owner` 是只移类型（move-only type），`std::unique_ptr`、`std::thread` 都是这个套路。移后 `a.value()` 是 -1，这是我们自己约定的"空状态"，不是标准规定的值——移动构造对源对象做什么由实现者决定，只要求"有效"。

反过来，如果一个类的成员全是 `std::string`、`std::vector` 这类自己管好资源的类型，那么**一个特殊成员函数都不用写**，编译器生成的版本全对，这叫 Rule of Zero（零法则）。它才是现代 C++ 的默认选择：把资源交给已经写对五件套的成员，自己只做组合。

```cpp
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Document {
public:
    explicit Document(std::string title) : title_(std::move(title)) {}
    void add(const std::string& line) { lines_.push_back(line); }
    std::size_t size() const { return lines_.size(); }
    const std::string& title() const { return title_; }
    const std::string& at(std::size_t i) const { return lines_[i]; }
private:
    std::string title_;
    std::vector<std::string> lines_;
};

int main() {
    Document d("note");
    d.add("alpha");
    d.add("beta");

    Document e = d;                    // 编译器生成：逐成员深拷贝
    e.add("gamma");
    std::cout << d.size() << ' ' << e.size() << '\n';

    Document f = std::move(e);         // 编译器生成：逐成员移动
    std::cout << f.size() << ' ' << f.title() << ' ' << f.at(2) << '\n';
}
```

输出：
```text
2 3
3 note gamma
```

`Document` 里没有一个特殊成员函数，拷贝、移动、析构全部正确——因为 `std::string` 和 `std::vector` 各自管好了自己的资源。

### 11.6 运算符重载：成员还是非成员

| 运算符 | 建议形式 | 返回类型 | 理由 |
| --- | --- | --- | --- |
| `+=`、`-=` | 成员 | `T&` | 左操作数就是对象，且要能链式 |
| `+`、`-` | 非成员 | `T`（新值） | 左右对称，允许两边隐式转换 |
| `==`、`<` | 非成员（或 `<=>`） | `bool` | 对称 |
| `<<`、`>>` | **必须**非成员 | `std::ostream&` | 左操作数是流，不是你 |
| `=`、`[]`、`()`、`->` | 必须成员 | — | 语言规定 |

三条判据：

1. **会不会修改左操作数？** 会 → 返回 `T&`（如 `+=`）；不会 → 返回新值 `T`（如 `+`）。
2. **需不需要对称？** `a + 1` 和 `1 + a` 都该能编译，所以 `+` 写成非成员（或 `friend`），让两边都能隐式转换。成员版本的左操作数不做隐式转换。
3. **左操作数是不是标准库类型？** `cout << v` 的左操作数是 `std::ostream`，你没法给它加成员函数，只能写自由函数。

`operator<<` 必须返回 `std::ostream&`（就是传进来的那个流），否则 `cout << a << b` 无法链式——`<<` 是左结合的，`(cout << a) << b` 要求前一步的结果还是个流。

C++20 起可以只写一个 `<=>`（三路比较，返回 `std::strong_ordering` 等），编译器会自动合成 `==`、`!=`、`<`、`<=`、`>`、`>=`；但 `<=>` 的返回类型决定了语义强弱，手写 `bool operator<` 仍然是最不容易出错的选择。对只需要"相等"的类，写 `bool operator==` 加 C++20 的默认化（`= default`）也够用。

❌ 不要重载 `&&`、`||`、`,`：重载后**失去短路求值**（`&&` 两边都会被求值）和规定的求值顺序，代码会静默改变行为。这是少数"能写但永远不该写"的地方。

```cpp
#include <iostream>

class Vec2 {
public:
    Vec2(double x = 0, double y = 0) : x_(x), y_(y) {}
    Vec2& operator+=(const Vec2& o) {          // 成员、返回引用
        x_ += o.x_;
        y_ += o.y_;
        return *this;
    }
    Vec2 operator-() const { return Vec2(-x_, -y_); }   // 一元负号：返回新值
    double x() const { return x_; }
    double y() const { return y_; }
private:
    double x_, y_;
};

Vec2 operator+(Vec2 a, const Vec2& b) { return a += b; }   // 按值收左操作数
bool operator==(const Vec2& a, const Vec2& b) { return a.x() == b.x() && a.y() == b.y(); }
std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << '(' << v.x() << ',' << v.y() << ')';
}

int main() {
    Vec2 a(1, 2), b(3, 4);
    std::cout << a + b << ' ' << -a << ' ' << (a == b) << (a == Vec2(1, 2)) << '\n';
    a += b;
    std::cout << a << '\n';
}
```

输出：
```text
(4,6) (-1,-2) 01
(4,6)
```

`operator+` 按值收第一个参数是刻意的：这样 `Vec2(1,2) + b` 和 `1 + b`（若存在隐式转换）都能工作，函数体内直接复用 `+=` 也就地修改这个副本，省一次构造。

前置/后置自增的区别是另一个高频考点：**后置版本靠一个 `int` 哑参数与前置版本区分**，并且必须按值返回旧值（返回引用会指向已经改过的对象）。

```cpp
#include <iostream>

class Counter {
public:
    explicit Counter(int v = 0) : v_(v) {}
    Counter& operator++() { ++v_; return *this; }                  // 前置：返回引用
    Counter operator++(int) { Counter old = *this; ++v_; return old; }  // 后置：返回值
    int value() const { return v_; }
private:
    int v_;
};

int main() {
    Counter c(5);
    std::cout << (++c).value() << ' ' << c.value() << '\n';
    Counter d = c++;
    std::cout << d.value() << ' ' << c.value() << '\n';
}
```

输出：
```text
6 6
6 7
```

性能上后置版本必然多一次拷贝（要保存旧值），所以循环里习惯写 `++i` 而不是 `i++`——对 `int` 无所谓，对迭代器就是实打实的开销。

### 11.7 copy-and-swap 与常见坑

把"拷贝"和"赋值"合并成一个惯用法：赋值运算符**按值**收参数，函数体里只做 `swap`。按值收参时，实参会被拷贝构造出一个临时对象（这一步已经完成了新资源的分配），交换之后临时对象带着旧资源析构。

```cpp
#include <algorithm>
#include <iostream>
#include <utility>

class Buffer {
public:
    explicit Buffer(std::size_t n, int fill) : n_(n), p_(new int[n]) {
        std::fill(p_, p_ + n_, fill);
    }
    Buffer(const Buffer& o) : n_(o.n_), p_(new int[o.n_]) {
        std::copy(o.p_, o.p_ + n_, p_);
    }
    Buffer& operator=(Buffer o) {                 // 按值收参：拷贝已经发生
        swap(o);
        return *this;
    }
    ~Buffer() { delete[] p_; }

    void swap(Buffer& o) noexcept {
        std::swap(n_, o.n_);
        std::swap(p_, o.p_);
    }
    int at(std::size_t i) const { return p_[i]; }
    std::size_t size() const { return n_; }
private:
    std::size_t n_;
    int* p_;
};

void assign_from(Buffer& dst, const Buffer& src) { dst = src; }

int main() {
    Buffer a(3, 7);
    Buffer b(2, 1);
    b = a;
    std::cout << a.size() << ' ' << b.size() << ' ' << b.at(2) << '\n';

    assign_from(b, b);                            // 自赋值
    std::cout << b.at(0) << ' ' << b.size() << '\n';
}
```

输出：
```text
3 3 7
7 3
```

copy-and-swap 一次解决三个问题：自赋值（临时对象是自己的一份拷贝，交换后内容不变）、异常安全（拷贝在进函数前完成，失败时 `this` 没被碰过）、代码重复（不用在赋值里再写一遍分配和复制）。代价是每次赋值多一次拷贝，对追求极致性能的类型可以手写，但**正确性优先**。

❌ 这一节最常见的三个错误：

```cpp-fragment
// 1. 浅拷贝 + 析构里 delete：double free
class Bad {
public:
    explicit Bad(int v) : p(new int(v)) {}
    ~Bad() { delete p; }            // 默认拷贝构造让两个对象共用一个 p
private:
    int* p;
};

// 2. 移动构造忘了把源置空：源析构时把已经交出去的资源删掉
Buf(Buf&& o) noexcept : n_(o.n_), p_(o.p_) { }   // ❌ 缺 o.p_ = nullptr

// 3. 对 const 对象 std::move：静默退化为拷贝
const Buf a(4);
Buf b(std::move(a));                // 调用的是拷贝构造，不是移动
```

第 1 种就是 11.2 讲的 double free；第 2 种的表现是"移动之后程序在析构阶段崩"，因为两个对象都认为自己拥有那块内存；第 3 种不报错、不崩溃，只是性能没提升，属于最难发现的一类。

**考试视角**：这一节的题几乎都在这几个点上——"哪个函数被调用"（看实参的值类别和是否已构造）、"输出顺序是什么"（构造/拷贝/赋值/析构的打印顺序）、"这段代码有什么问题"（浅拷贝、自赋值、忘置空、非虚析构）。判断口诀：**默认逐成员、指针只拷地址；有资源三件套，能偷就偷、偷完置空。**

## 12. 继承与多态

继承让派生类复用基类的接口与实现；多态让"通过基类指针或引用调用"在运行时分派到对象的实际类型。这两件事是面向对象的基础，也是卷面上错误率最高的部分——因为它的坑几乎全是"编译通过、运行结果不对"：少写一个 `virtual`、漏一个 `const`、按值传一次参数，程序都照常跑，只是行为悄悄变了。

### 12.1 公有继承表达"是一种"

`class Derived : public Base` 表达的是 is-a 关系：`Derived` **就是**一个 `Base`，所以任何接受 `Base&` / `Base*` 的地方都能传 `Derived`。三种继承方式改变的是"基类的公有成员在派生类里以什么身份出现"：

| 继承方式 | 基类 `public` 成员在派生类中 | 外部能否把 `Derived*` 当 `Base*` |
| --- | --- | --- |
| `public` | `public` | 能 |
| `protected` | `protected` | 不能 |
| `private` | `private` | 不能 |

`class` 的默认继承是 `private`，`struct` 是 `public`。所以写 `class Dog : Animal` 得到的是"实现继承"：`Dog` 内部能用 `Animal` 的成员，但外部**不能**把 `Dog*` 转成 `Animal*`，多态完全失效。这个错误编译器只在真正转换时报一句"基类不可访问"，看起来和语法错误差不多，很容易被当成别的问题。**永远显式写 `public`。**

还有一条设计原则：继承是为了"接口兼容"，不是为了复用代码。`Stack` 想复用 `Vector` 的实现，但栈不是向量（栈不能按下标随便改），正确做法是**组合**——把 `Vector` 作为成员，只暴露 `push`/`pop`。公有继承会把基类的全部公有接口漏给使用者，一旦基类后来加了新函数，派生类的语义可能就被破坏了。

`protected` 是给继承准备的中间层：派生类能访问，外部不能。它的问题是，一旦把成员设成 `protected`，就等于把这个成员写进了对所有派生类的承诺里——以后想改它的表示（比如把 `int size_` 换成 `std::size_t`）会同时影响所有派生类。所以现代 C++ 的建议是数据成员一律 `private`，需要给派生类用的行为通过 `protected` 成员函数或公有的虚函数暴露。

继承还会带来编译期耦合：基类的布局一变，所有派生类都要重新编译；基类虚函数表的顺序一变，二进制兼容就断了。这也是"继承层次要浅"的实际理由——三层以上通常意味着设计出了问题，组合加接口类往往更灵活。

### 12.2 对象布局与构造析构顺序

派生类对象在内存里**包含一个完整的基类子对象**，通常排在对象开头（有虚函数时 `vptr` 在最前面），然后是派生类自己的成员：

```
class Base    { int b_; virtual void f(); };
class Derived : public Base { int d_; };

Derived 对象（本机 x86-64 示意）
偏移 0        ┌──────────────────┐
              │ vptr ────────────┼──> Derived 的虚函数表
偏移 8        │ Base::b_         │  ← 基类子对象
偏移 12       │ Derived::d_      │  ← 派生类新增成员
              └──────────────────┘
```

这个布局决定了构造顺序：必须先有基类子对象，才能构造派生类。顺序是**基类 → 成员（按声明顺序）→ 自己的函数体**；析构严格镜像：**自己的函数体 → 成员逆序 → 基类**。

```
new Derived()
 1. Base()                       ← 基类先构造（基类无默认构造时必须在列表里显式调）
 2. Derived 的成员按声明顺序构造
 3. Derived() 的函数体
       ... 使用对象 ...
 4. ~Derived() 的函数体
 5. Derived 的成员逆序析构
 6. ~Base()                      ← 基类最后析构
```

构造函数不能被继承（它不是普通成员函数），但 C++11 起可以 `using Base::Base;` 把基类的构造函数"引入"派生类。派生类构造函数若没显式调用基类构造，编译器会尝试调用 `Base()`；基类没有默认构造时**必须**写 `Derived(args) : Base(args) {}`，否则编译错。

单继承下基类子对象位于偏移 0，所以 `Derived*` 转 `Base*` 不需要调整地址，两个指针指向同一处。多继承时不同基类子对象位于不同偏移，转换就会做指针调整——这也是为什么类层次之间的转换要用 `static_cast`/`dynamic_cast`，绝不能用 `reinterpret_cast`。

空基类（没有非静态数据成员、没有虚函数）作为基类时可以不占空间，这叫空基类优化（EBO），`sizeof(Derived)` 可能正好等于派生类成员的大小。它是"实现允许"而非强制的优化，不要写依赖它的代码，也不要拿 `sizeof` 去"证明"继承没有开销。

```cpp
#include <iostream>

struct Base {
    Base() { std::cout << "1 Base()\n"; }
    ~Base() { std::cout << "6 ~Base()\n"; }
};

struct Member {
    Member() { std::cout << "2 Member()\n"; }
    ~Member() { std::cout << "5 ~Member()\n"; }
};

struct Derived : Base {
    Member m;
    Derived() { std::cout << "3 Derived()\n"; }
    ~Derived() { std::cout << "4 ~Derived()\n"; }
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
1 Base()
2 Member()
3 Derived()
-- in scope --
4 ~Derived()
5 ~Member()
6 ~Base()
-- out of scope --
```

编号就是执行顺序。这条规则的实际意义：派生类析构函数里可以安全使用基类的成员（基类部分这时还活着），反过来在基类析构里访问派生类成员一定错（那部分已经没了）。

### 12.3 虚函数：早绑定与晚绑定

普通成员函数的调用在**编译期**就确定了：`p->f()` 生成一条"调用 `Base::f`"的指令，因为编译器只看 `p` 的静态类型。这叫早绑定（early binding）。`virtual` 函数则推迟到**运行期**，按对象的实际类型选择，叫晚绑定（late binding）。

实现方式是每个多态对象里藏一个 `vptr`（虚表指针），指向该类的**虚函数表**（vtable，一张函数指针数组）：

```
Animal 对象                    Animal 的 vtable
┌───────────────┐             ┌────────────────────┐
│ vptr ─────────┼────────────>│ &Animal::speak     │
│ ...           │             │ &Animal::~Animal   │
└───────────────┘             └────────────────────┘

Dog 对象                       Dog 的 vtable
┌───────────────┐             ┌────────────────────┐
│ vptr ─────────┼────────────>│ &Dog::speak        │  ← 覆盖后换成派生类地址
│ ...           │             │ &Dog::~Dog         │
└───────────────┘             └────────────────────┘

Animal* p = new Dog;
p->speak()  →  取 p 指向对象的 vptr → 查第 k 项 → 调用 Dog::speak
```

三件事由此推出：

1. 多态**只对指针和引用生效**。值传递时对象被拷贝/切片，`vptr` 也一起被换成基类的（12.7）。
2. 每个多态对象多一个指针的开销；每个类多一张表；每次调用多一次间接跳转，且通常无法内联。所以纯数据类不要无脑加虚函数。
3. 构造函数执行时 `vptr` 还指向**当前正在构造的那一层**，所以构造/析构里调用虚函数不会分派到派生类（12.9）。

`final` 除了禁止覆盖，还能帮编译器做去虚化（devirtualization）：如果调用的对象静态类型已知、函数又标了 `final`，编译器可以直接生成直接调用，省掉查表那一跳。反过来，如果一个类根本不打算被继承，把它标成 `final` 是有价值的：它同时告诉读者"这里没有多态"，也让编译器放心优化。但不要为了省这点开销把设计搞乱——虚函数调用的开销在绝大多数程序里可以忽略。

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    virtual ~Animal() = default;
    std::string name() const { return "animal"; }          // 非虚：静态绑定
    virtual std::string sound() const { return "..."; }    // 虚：动态绑定
};

class Dog : public Animal {
public:
    std::string name() const { return "dog"; }             // 只是隐藏，不是覆盖
    std::string sound() const override { return "woof"; }
};

int main() {
    Dog d;
    Animal& r = d;
    std::cout << r.name() << ' ' << r.sound() << '\n';
    std::cout << d.name() << ' ' << d.sound() << '\n';
}
```

输出：
```text
animal woof
dog woof
```

同一个对象、同一个函数名，通过基类引用调到 `animal`、直接调到 `dog`——这就是"虚不虚"的唯一区别。判断口诀：**看静态类型决定能不能调，看动态类型决定调哪个版本；只有虚函数才看动态类型。**

### 12.4 override 与 final

`override` 不是必需的，但**必须写**，因为它让编译器检查"我确实在覆盖一个基类虚函数"。漏写 `const`、参数类型写错、基类那个函数忘了 `virtual`——这些错误在没有 `override` 时会变成"定义了一个新函数、悄悄隐藏基类版本"，程序照常编译，行为错得莫名其妙。加了 `override`，编译器直接报"没有可覆盖的虚函数"。

| 写法 | 效果 |
| --- | --- |
| `virtual void f();` | 声明虚函数，派生类可覆盖 |
| `void f() override;` | 覆盖，并让编译器校验签名完全一致 |
| `void f() final;` | 禁止再被覆盖（`final` 也可以修饰整个类） |
| `virtual void f() = 0;` | 纯虚，派生类必须实现 |

签名一致包括：参数类型、`const` 限定、引用限定、返回类型（协变除外）。`std::string name()` 与 `std::string name() const` 是**两个不同的函数**，前者不会覆盖后者——这是最高频的"少写一个 const"事故。

### 12.5 纯虚函数与抽象类

`= 0` 的虚函数叫纯虚函数，含纯虚函数的类是**抽象类**：不能创建对象，只能作为接口被继承。派生类必须实现全部纯虚函数才能实例化，否则它自己也是抽象类。

抽象类的价值是把"接口"和"实现"彻底分开：调用方只依赖 `Shape&`，不需要知道有多少种图形，新增一种图形也不用改调用方的代码。这就是"面向接口编程"在 C++ 里的形态。

抽象类不是"没有实现"的类：它可以有数据成员、普通成员函数、构造函数（供派生类在初始化列表里调用），只是不能直接实例化。纯虚函数也可以有函数体（在类外定义），派生类能用 `Base::f()` 显式调用这份默认实现——这是"接口 + 可选默认实现"的写法。只含纯虚函数和虚析构函数的抽象类叫接口类（interface class），是 C++ 里模拟"接口"的标准手法。

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;                 // 纯虚 → Shape 是抽象类
    virtual const char* name() const = 0;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    const char* name() const override { return "rect"; }
private:
    double w_, h_;
};

class Square : public Shape {
public:
    explicit Square(double s) : s_(s) {}
    double area() const override { return s_ * s_; }
    const char* name() const override { return "square"; }
private:
    double s_;
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Rect>(2, 3));
    shapes.push_back(std::make_unique<Square>(2.5));

    double total = 0;
    for (const auto& p : shapes) {                   // 引用：不切片
        std::cout << p->name() << ' ' << p->area() << '\n';
        total += p->area();
    }
    std::cout << total << '\n';
}
```

输出：
```text
rect 6
square 6.25
12.25
```

`std::unique_ptr<Shape>` 存的是基类指针，`->area()` 却调到了两个不同派生类的实现——多态与所有权管理在这里正好配合：`unique_ptr` 负责释放，虚析构负责释放**正确的那一层**。

### 12.6 虚析构：为什么基类析构必须是虚的

`delete p;` 时如果 `p` 的静态类型是 `Base*` 而 `~Base` **不是**虚函数，编译器只会调用 `~Base`：派生类那部分成员根本没有析构。标准里这是**未定义行为**，实践中表现为派生类持有的资源泄漏，或者更糟——派生类的析构里做的事（关闭文件、解锁、写日志）全部跳过。

```cpp-fragment
class Bad {
public:
    ~Bad() {}                          // ❌ 非虚
};
class BadDerived : public Bad {
public:
    ~BadDerived() { delete[] buf_; }   // 永远不会被调用
private:
    int* buf_ = new int[100];
};

Bad* p = new BadDerived;
delete p;                              // UB：只执行 ~Bad，buf_ 泄漏
```

规则：**一个类只要可能被通过基类指针 `delete`，析构函数就必须是 `virtual`。** 基类析构写成虚的之后，派生类的析构自动是虚的（不需要再写 `virtual`，但建议写 `override` 让编译器检查）。代价仍然是每个对象多一个 `vptr`，所以"纯数据、不会被多态删除"的类不要加。

```cpp
#include <iostream>
#include <memory>

class Base {
public:
    virtual ~Base() { std::cout << "~Base\n"; }
    virtual void hello() const { std::cout << "Base hello\n"; }
};

class Derived : public Base {
public:
    ~Derived() override { std::cout << "~Derived\n"; }
    void hello() const override { std::cout << "Derived hello\n"; }
};

int main() {
    std::unique_ptr<Base> p = std::make_unique<Derived>();
    p->hello();
    std::cout << "-- end of scope --\n";
}
```

输出：
```text
Derived hello
-- end of scope --
~Derived
~Base
```

`p` 的静态类型是 `Base*`，作用域结束时却能先调 `~Derived` 再调 `~Base`，靠的就是虚析构。把 `~Base` 的 `virtual` 去掉，`~Derived` 那一行就不会出现——但程序不会报错，这就是它危险的地方。

### 12.7 对象切片

把派生类对象**按值**赋给基类对象（或按值传参、按值返回）时，只复制基类子对象，派生类新增的成员被丢弃，`vptr` 也被换成基类的。这叫切片（object slicing）：

```
Derived d                        Base sliced = d;
┌──────────────────┐            ┌──────────────────┐
│ vptr → Derived   │            │ vptr → Base      │  ← 换成基类虚表
│ Base 部分         │  ──拷贝──>  │ Base 部分         │
│ Derived::d_      │            └──────────────────┘
└──────────────────┘             Derived::d_ 被丢弃，无法恢复
```

切片是**合法且静默**的：编译器不会报警（部分编译器对按值传参有 `-Wslice` 类警告，但不默认开），因为从类型系统看这只是一次普通的基类拷贝构造。

```cpp
#include <iostream>
#include <string>

class Base {
public:
    virtual ~Base() = default;
    virtual std::string who() const { return "Base"; }
};

class Derived : public Base {
public:
    std::string who() const override { return "Derived"; }
};

void byValue(Base b) { std::cout << "byValue  " << b.who() << '\n'; }
void byRef(const Base& b) { std::cout << "byRef    " << b.who() << '\n'; }

int main() {
    Derived d;
    std::cout << "object   " << d.who() << '\n';

    Base sliced = d;
    std::cout << "sliced   " << sliced.who() << '\n';

    byValue(d);
    byRef(d);

    Base* p = &d;
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

`sliced` 的 `who()` 返回 `Base`，因为它的 `vptr` 已经指向 `Base` 的虚表；最后两行是 `dynamic_cast` 的判据——`p` 指向的确实是 `Derived`，转换成功；`&sliced` 只是一块 `Base`，转换返回空指针。**多态只能通过指针或引用获得，值传递一定丢类型。**

容器是切片的另一个重灾区：`std::vector<Base> v; v.push_back(d);` 存进去的是一份 `Base` 子对象，`d` 的派生部分在 `push_back` 那一刻就没了，之后怎么取都恢复不了。要存多态对象，就存 `std::unique_ptr<Base>`（拥有所有权）或 `std::reference_wrapper<Base>`（不拥有，要求原对象还活着）。`std::vector<Base*>` 也能编译，但所有权和生命周期得自己管，容易泄漏。

顺带说明 `dynamic_cast`：它只对**多态类型**（有虚函数的类）有效，运行时查 RTTI 判断真实类型。指针版失败返回 `nullptr`，引用版失败抛 `std::bad_cast`。它比 `static_cast` 安全，但比虚函数分派慢，而且通常意味着设计上缺少一个虚函数——能用虚函数解决的问题不要用它。

### 12.8 名字隐藏与 using

派生类里声明一个与基类**同名**的函数，会**隐藏**基类那一个名字下的**全部重载**。注意这跟虚函数是两套机制：这里讲的是普通的名字查找，与动态分派无关——即使基类那个函数是虚的，只要签名不同，也会被隐藏。

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
    d.f(1);
    double v = 2.5;
    d.f(v);                           // Base::f(double) 被隐藏 → 隐式转 int

    Derived2 d2;
    d2.f(1);
    d2.f(v);
}
```

输出：
```text
Derived::f(int) 1
Derived::f(int) 2
Derived2::f(int) 1
Base::f(double) 2.5
```

`d.f(v)` 打印 `2` 而不是 `2.5`：`Base::f(double)` 被隐藏了，`2.5` 只能隐式转成 `int` 去匹配 `Derived::f(int)`，**静默的精度丢失**。加一行 `using Base::f;` 就恢复正常重载决议。这条规则与虚函数无关：名字查找先于类型检查，找到 `Derived::f` 这个"唯一的名字"之后就不再往基类找。

### 12.9 构造/析构中的虚调用与 RTTI

构造函数执行到 `Base()` 时，派生类部分还没构造完，此时对象的 `vptr` 指向的是 `Base` 的虚表；析构函数同理，`~Base()` 执行时派生类部分已经销毁。所以在这两个地方调用虚函数，**只会分派到当前正在构造/析构的那一层**。标准明确规定这一点的目的很直接：不能让你在对象还没建好时去访问还不存在的成员。

```cpp
#include <iostream>
#include <string>

class Base {
public:
    Base() { std::cout << "Base ctor: " << name() << '\n'; }
    virtual ~Base() { std::cout << "Base dtor: " << name() << '\n'; }
    virtual std::string name() const { return "Base"; }
};

class Derived : public Base {
public:
    Derived() { std::cout << "Derived ctor: " << name() << '\n'; }
    ~Derived() override { std::cout << "Derived dtor: " << name() << '\n'; }
    std::string name() const override { return "Derived"; }
};

int main() {
    Derived d;
    std::cout << "-- use --\n";
}
```

输出：
```text
Base ctor: Base
Derived ctor: Derived
-- use --
Derived dtor: Derived
Base dtor: Base
```

第一行和最后一行都是 `Base`。所以"在构造函数里调用虚函数做初始化"永远拿不到派生类的行为，正确做法是把这类逻辑移到派生类构造函数体里，或者用两阶段初始化。更严重的变体是在基类构造里调用**纯虚函数**——因为纯虚函数没有实现（除非你给了定义），运行时通常是链接错误或直接崩溃，属于 UB。

### 12.10 常见坑与考试视角

1. **基类析构非虚 + 通过基类指针 `delete`** → 派生析构不执行，UB（12.6）。
2. **对象切片** → 多态静默失效，编译器不报警（12.7）。容器里存 `Base` 值而不是 `Base*`/`reference_wrapper` 是重灾区。
3. **忘记 `override`** → 签名差一个 `const` 就变成"新函数"，不多态也不报错（12.4）。
4. **构造函数/析构函数里调用虚函数** → 只到当前层（12.9）。
5. **默认继承是 private**（`class D : B`）→ 外部无法隐式转换（12.1）。
6. **纯虚函数仍可以有定义** → `virtual void f() = 0;` 之后在类外写 `void Base::f() {}` 是合法的，派生类可以显式 `Base::f()` 调用；但通常没必要，别在考试里把它当成"纯虚不能有函数体"的反例。
7. **不要为了复用代码而公有继承** → 不满足 is-a 时用组合，否则基类全部接口都会漏给使用者。

```cpp-fragment
// 1. 非虚析构：delete 基类指针时派生析构不执行
class B { public: ~B() {} };
class D : public B { public: ~D() { delete[] p_; } int* p_ = new int[8]; };
B* p = new D;
delete p;                                  // ❌ UB

// 2. 漏写 const，签名不匹配 → 变成隐藏而非覆盖
class Base2 { public: virtual int size() const { return 0; } };
class D2 : public Base2 { public: int size() { return 1; } };   // ❌ 不是 override

// 3. 按值传参：切片
void f(Base2 b);                           // ❌ 派生部分丢失
```

**考试视角**：这一节的选择题几乎都在问"输出是什么"和"哪里错了"。判断顺序固定为三步——先看**静态类型**决定能不能调这个函数；再看函数是不是 **virtual** 决定用哪个版本；最后看是**指针/引用还是值**决定有没有切片。口诀：**虚函数决定"调用谁"，虚析构决定"析构谁"，两者都只在指针或引用上生效。**

## 13. 模板与泛型

模板（template）把"类型"或"编译期常量"提升为参数，让同一份逻辑适配多种类型。编译器在**实例化**（instantiation）时为每组实参生成一份真实代码，所以模板是**编译期多态**：没有虚表、没有运行时分发，代价是编译变慢、报错变长、二进制变大。学模板要抓住三条主线：`T` 怎么被推导出来、代码什么时候被生成、名字在哪个阶段被查找。

### 13.1 泛型的四种实现方式

没有模板时，想让 `max` 同时支持 `int` 和 `double`，只有三条路：写多个重载、用 `void*` 加转换、用宏。它们和模板、虚函数的对比如下：

| 方案 | 绑定时机 | 类型检查 | 运行时开销 | 典型问题 |
| --- | --- | --- | --- | --- |
| 宏 `#define MAX(a,b) ((a)>(b)?(a):(b))` | 预处理 | 无 | 无 | 参数被求值两次；无类型；难调试 |
| 重载 `int max(int,int)` / `double max(double,double)` | 编译期（调用点） | 有 | 无 | 类型多了要手写 N 份 |
| `void*` + 函数指针 | 运行期 | 弱 | 间接调用、难内联 | 丢失类型安全，容易传错 |
| 模板 | 编译期（实例化） | 有 | 无（可内联） | 编译时间、报错、代码膨胀 |
| 虚函数 | 运行期 | 有（基类接口） | 虚表间接调用 | 需要继承体系，值语义受限 |

模板和重载的差别只在"谁写这份代码"：重载是人手写，模板是编译器按模式生成。模板和宏的差别是本质的：模板参与类型系统、遵守作用域、实参只求值一次。模板和虚函数的差别在绑定时机：模板在编译期就把类型定死，虚函数在运行期才查表，所以模板能做到零开销，但不能在运行时换实现。

### 13.2 函数模板与类型推导

基本形式是 `template <class T> T mymax(T a, T b);`。`class` 与 `typename` 在模板参数列表里完全等价，`typename` 更常出现在"依赖类型名"的位置。不写 `<...>` 时，编译器拿实参去匹配形参模式，这套规则叫模板实参推导（template argument deduction）。考试只考四种形参：

| 形参写法 | 实参 `int x; const int cx; int a[3];` 推出的 T | 要点 |
| --- | --- | --- |
| `T` | `int` / `int` / `int*` | 按值：顶层 const 丢弃，数组与函数退化 |
| `T&` | `int` / `const int` / `int[3]` | 左值引用：const 保留，数组不退化 |
| `const T&` | `int` / `int` / `int[3]` | 只读绑定，数组不退化 |
| `T&&` | 左值 → `int&`；右值 → `int` | 转发引用：引用折叠后可能仍是左值引用 |

三条口诀：按值传参一定发生拷贝，数组一定退化成指针；引用传参不拷贝，`T&` 会把 const 一起推出来；`T&&` 只有在 `T` 参与推导时才是转发引用，写成 `std::vector<T>&&` 就只是普通右值引用。显式指定实参（`mymax<double>(3, 4.5)`）会关闭该参数的推导，实参按普通函数规则隐式转换。返回值可以用 `auto`（按值推导，数组与 const 都会退化）或 `decltype(auto)`（保留引用），要返回引用必须显式写 `-> T&` 或用 `decltype(auto)`。

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

最后一行 `mymax('a','z')` 推成 `char`，`<` 比较的是字符编码，`'z'` 更大，所以输出 `z`。如果想比较字符串字面量的"字典序"，必须先把至少一个实参变成 `std::string`，否则 `T` 会被推成 `const char*`，比较的是指针地址——这是下一节反例的原型。

推导规则光看表容易混，下面用 `std::is_same_v` 把每种情形打印出来。`byValue` 接收 `T`，`byRef` 接收 `T&`，同一组实参分别喂给它们：

```cpp
#include <iostream>
#include <type_traits>

template <class T>
void byValue(T) {
    std::cout << std::is_same_v<T, int> << std::is_same_v<T, const int>
              << std::is_same_v<T, int*> << '|';
}

template <class T>
void byRef(T&) {
    std::cout << std::is_same_v<T, const int> << std::is_same_v<T, int[3]> << '|';
}

int main() {
    int x = 1;
    const int cx = 2;
    int a[3] = {1, 2, 3};

    byValue(x);
    byValue(cx);
    byValue(a);
    std::cout << '\n';

    byRef(x);
    byRef(cx);
    byRef(a);
    std::cout << '\n';
}
```

输出：
```text
100|100|001|
00|10|01|
```

第一行 `100|100|001|` 对应 `byValue`：`x` 推出 `int`；`cx` 的顶层 const 被丢掉，仍是 `int`；数组 `a` 退化成 `int*`，所以第三个位置是 `001`。第二行对应 `byRef`：`x` 推出 `int`，两个判断都为假，输出 `00`；`cx` 推出 `const int`；数组不退化，推出 `int[3]`。把 `byRef` 换成 `const T&`，`cx` 会推出 `int` 而不是 `const int`——const 被引用本身的 const 吸收了。

❌ 最常见的一类报错是"一个 `T` 被推成两种类型"：

```cpp-fragment
template <class T>
T mymax(T a, T b);

mymax(3, 4.5);   // T 同时被推成 int 和 double
```

clang++ 的报错（去掉路径与列号）：

```text
error: no matching function for call to 'mymax'
note: candidate template ignored: deduced conflicting types for parameter 'T' ('int' vs. 'double')
```

第一条 error 只说"调用点没有可行函数"，真正的原因是紧跟的 note。修法有两种：显式指定 `mymax<double>(3, 4.5)`，让 `3` 隐式转换；或先把实参统一成同一类型。不要指望"编译器会帮我选一个更宽的类型"，推导不做任何类型提升。

### 13.3 类模板

类模板的每个成员函数都可以定义在类外，但模板头必须重写，限定名必须带实参列表：

```cpp-fragment
template <class T, std::size_t N>
T FixedArray<T, N>::sum() const { /* ... */ }
```

漏掉 `template <class T, std::size_t N>` 或漏掉 `<T, N>` 都是编译错误。类内定义则隐式 inline，写法短，但每个实例化都会生成一份。

类模板里有两类名字：不依赖模板参数的名字在**定义时**查找；依赖模板参数的名字留到**实例化时**再查，这叫两阶段查找（two-phase lookup）。因此派生类模板访问基类成员时，只写 `x` 编译器在定义阶段找不到（基类还没确定），必须写 `this->x` 或 `Base<T>::x`。同理，依赖类型名前面要加 `typename`，如 `typename T::value_type`；不加时编译器默认把它当值。

C++17 起有类模板实参推导（CTAD）：`std::pair p{1, 2.5}` 能自动推出 `std::pair<int,double>`，`std::vector v{1,2,3}` 推出 `std::vector<int>`。CTAD 只对构造函数可用，且不如函数模板推导灵活，初学阶段建议显式写实参。

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
    T sum() const;
private:
    T data_[N]{};
};

// 类外定义成员函数：模板头要重写，限定名要带 <T, N>
template <class T, std::size_t N>
T FixedArray<T, N>::sum() const {
    T s{};
    for (std::size_t i = 0; i < N; ++i) s += data_[i];
    return s;
}

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

`T data_[N]{}` 的 `{}` 让数组零初始化，所以不赋值也不会读到栈上的垃圾。`size()` 标了 `constexpr`，`a.size()` 就是编译期常量，可以直接当另一个模板的实参。`FixedArray<int,3>` 与 `FixedArray<int,4>` 是**两个毫不相干的类型**，`is_same_v` 为假，不能互相赋值——这是非类型参数参与类型身份的直接后果。

❌ 派生类模板里访问依赖基类的成员却不加 `this->`：

```cpp-fragment
template <class T>
struct Base { int value = 1; };

template <class T>
struct Derived : Base<T> {
    int get() const { return value; }   // 定义阶段找不到 value
};
```

clang++ 的报错只有一行：`error: use of undeclared identifier 'value'`。改成 `return this->value;` 或 `return Base<T>::value;` 即可。注意这不是"编译器太笨"，而是两阶段查找的标准行为：定义 `Derived` 时 `Base<T>` 还没实例化，编译器不知道它有没有 `value`。

### 13.4 非类型模板参数

非类型模板参数（non-type template parameter，NTTP）把"值"变成模板的一部分。C++20 允许的类型包括：整型、枚举、指针（指向有链接的对象/函数）、左值引用、`nullptr_t`、浮点数，以及满足"结构化类型"的字面量类类型。实参必须是**编译期常量表达式**，运行时变量不行。

`std::array<T,N>` 是标准库里的原型：`N` 参与类型身份，`array<int,3>` 和 `array<int,4>` 不能互转；`N` 是编译期常量，所以对象可以完全放在栈上，没有堆分配。函数模板也能推导 `N`：

```cpp-fragment
template <std::size_t N>
std::size_t countOf(int (&a)[N]) { return N; }   // 数组引用，N 自动推出
```

注意 `int (&a)[N]` 的括号不能省：`int& a[N]` 是"引用的数组"，非法。C++20 的浮点 NTTP 让 `template <double Scale> struct Unit;` 成为可能，但浮点相等与跨平台表示仍有坑，工程上用整数比例更稳。

### 13.5 特化与偏特化

主模板给通用实现，特化给特定实参"换一套代码"：

| 形式 | 写法 | 匹配范围 | 函数模板支持？ |
| --- | --- | --- | --- |
| 全特化 | `template <> struct X<int>` | 唯一一组实参 | 支持 |
| 偏特化 | `template <class T> struct X<T*>` | 一族实参（所有指针） | **不支持** |
| 变量模板偏特化 | `template <class T> constexpr bool v<T*> = true;` | 一族实参 | 支持（C++14 起） |

函数模板只能全特化，不能偏特化。想让函数模板"对某类类型特判"，优先用**重载**或 `if constexpr`，而不是全特化：全特化不参与重载决议，且特化声明必须出现在首次使用之前，漏写不报错，程序会静默地走主模板。

`if constexpr`（C++17）在编译期丢弃不成立的分支，是替代函数特化的现代写法：分支里可以有只对某些类型合法的代码，只要它所在的条件为假，就不会被实例化。标准库的 `std::is_integral`、`std::hash`、`std::tuple_size` 都是"主模板 + 偏特化"的组合。

```cpp
#include <iostream>
#include <string>
#include <type_traits>

template <class T>
struct Printer {
    static void print(const T& v) { std::cout << "value: " << v << '\n'; }
};

template <>
struct Printer<bool> {
    static void print(bool v) { std::cout << (v ? "true" : "false") << '\n'; }
};

template <class T>
struct Printer<T*> {
    static void print(T* p) { std::cout << "pointer -> " << *p << '\n'; }
};

template <class T>
void printSmart(const T& v) {
    if constexpr (std::is_same_v<T, bool>) {
        std::cout << (v ? "true" : "false") << '\n';
    } else if constexpr (std::is_pointer_v<T>) {
        std::cout << "pointer -> " << *v << '\n';
    } else {
        std::cout << "value: " << v << '\n';
    }
}

int main() {
    Printer<int>::print(42);
    Printer<bool>::print(true);
    int x = 7;
    Printer<int*>::print(&x);
    Printer<std::string>::print("ok");

    printSmart(true);
    printSmart(&x);
    printSmart(3.5);
}
```

输出：
```text
value: 42
true
pointer -> 7
value: ok
true
pointer -> 7
value: 3.5
```

`Printer<bool>` 是全特化，`Printer<T*>` 是偏特化；`Printer<int*>` 同时匹配主模板和偏特化，偏特化更"特化"，胜出。`printSmart` 用 `if constexpr` 达到同样效果，且不需要为每个类型写一个新版式。注意三个分支都必须语法完整，但只有被选中的分支会被实例化，所以 `*v` 出现在指针分支里不会影响 `printSmart(3.5)`。

❌ 特化声明写在首次使用之后：

```cpp-fragment
template <class T> struct Tag { static const char* name() { return "generic"; } };
const char* a = Tag<int>::name();   // 这里已经隐式实例化了主模板

template <> struct Tag<int> { static const char* name() { return "int"; } };
const char* b = Tag<int>::name();   // 仍然是 "generic"
```

标准规定：显式特化必须在"首次导致隐式实例化的使用"之前声明，否则程序**无诊断要求地非良构**（IFNDR）。clang 通常不报错，只是继续用主模板，这类 bug 极难查。把特化放到所有使用之前，或改用重载/`if constexpr`。

### 13.6 可变参数模板与完美转发

`class... Ts` 是类型包，`const Ts&... ts` 是函数参数包，`sizeof...(Ts)` 取元素个数。包必须被**展开**：C++17 的折叠表达式（fold expression）把包展开成一条表达式，四种形式必须记牢：

| 写法 | 名称 | 展开结果 | 空包 |
| --- | --- | --- | --- |
| `(ts + ...)` | 一元右折叠 | `t1 + (t2 + t3)` | 非法 |
| `(... + ts)` | 一元左折叠 | `(t1 + t2) + t3` | 非法 |
| `(0 + ... + ts)` | 二元左折叠 | `((0 + t1) + t2) + t3` | 得 `0` |
| `((std::cout << ts << ' '), ...)` | 逗号折叠 | 从左到右依次求值 | 合法，什么也不做 |

一元折叠对空包非法，二元折叠有初值所以空包合法。`sumAll` 用 `(0 + ... + ts)`，空包返回 `int` 的 `0`；如果初值写 `0.0`，返回类型就变成 `double`。

完美转发（perfect forwarding）是 `make_unique`、`emplace_back` 的底层：形参写 `Args&&...`，转发时写 `std::forward<Args>(args)...`。`Args&&` 在推导语境下是转发引用，左值实参推成 `T&`，右值推成 `T`；`std::forward` 再按这个结果还原值类别。忘记 `std::forward` 会把右值当成左值，多一次拷贝；忘记 `...` 则只转发第一个。

```cpp
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

template <class... Ts>
void printAll(const Ts&... ts) {
    ((std::cout << ts << ' '), ...);
    std::cout << "[n=" << sizeof...(Ts) << "]\n";
}

template <class... Ts>
auto sumAll(const Ts&... ts) {
    return (0 + ... + ts);
}

struct Point {
    int x;
    int y;
    Point(int x_, int y_) : x(x_), y(y_) {}
};

template <class T, class... Args>
std::unique_ptr<T> makeThing(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

int main() {
    printAll(1, 2.5, "hi");
    printAll();
    std::cout << sumAll(1, 2, 3) << '\n';
    std::cout << sumAll(1.5, 2.5) << '\n';

    auto p = makeThing<Point>(3, 4);
    std::cout << p->x << ',' << p->y << '\n';

    auto s = makeThing<std::string>(3, 'x');
    std::cout << *s << '\n';
}
```

输出：
```text
1 2.5 hi [n=3]
[n=0]
6
4
3,4
xxx
```

`printAll()` 的空包走逗号折叠，什么也不打印，只输出 `[n=0]`。`makeThing<Point>(3,4)` 把两个 `int` 原样转发给 `Point` 的构造函数；`makeThing<std::string>(3,'x')` 转发给 `string(size_t,char)`，得到 `xxx`。C++17 之后不要再写递归展开（`head, tail...`），折叠表达式更短也更不容易栈溢出。

### 13.7 concepts 入门

模板最大的抱怨是报错：类型不满足要求时，错误发生在实例化深处，层层模板回溯几十行。concepts（C++20）把"对类型的要求"写进签名，让不满足的调用在**调用点**被拒绝，并给出"because ... does not satisfy ..."的原因链。

三种等价写法：

```cpp-fragment
template <std::integral T> T half(T x);                    // 约束模板参数
template <class T> requires std::floating_point<T>         // requires 子句
T half(T x);
void show(std::integral auto x);                           // 简写函数模板
```

`requires` 表达式用来描述"类型必须支持哪些表达式"，可以检查四类要求：简单要求（`a + b;`）、类型要求（`typename T::value_type;`）、复合要求（`{ a + b } -> std::convertible_to<int>;`）、嵌套要求（`requires std::integral<T>;`）。标准概念在 `<concepts>` 里：`std::integral`、`std::floating_point`、`std::same_as`、`std::convertible_to`、`std::totally_ordered`、`std::invocable`、`std::ranges::range` 等。

约束参与重载决议，并且可以**偏序**：两个候选都可行时，约束更强（能推出另一个）的那个优先。因此 `half(7)` 选整型版本，`half(7.5)` 选浮点版本，不需要写 `enable_if`。概念本身是编译期布尔常量，可以放进 `static_assert`。

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

`half(7)` 走整型版本，`7/2` 是整数除法得 `3`；`half(7.5)` 走浮点版本得 `3.75`。`twice` 只要求 `a + b` 合法，`std::string` 满足，所以输出 `abab`。`static_assert` 证明概念是编译期常量。

❌ 用概念约束后仍传入不满足要求的类型：

```cpp-fragment
template <std::integral T>
T half(T x);

half("x");   // const char* 不是整型
```

clang++ 的报错（节选）：

```text
error: no matching function for call to 'half'
note: candidate template ignored: constraints not satisfied [with T = const char *]
note: because 'const char *' does not satisfy 'integral'
note: because 'is_integral_v<const char *>' evaluated to false
```

对比 13.2 的 `deduced conflicting types`：概念错误会直接告诉你缺哪个约束，而不是让你去猜函数体里哪一行炸了。读这类报错的方法是从上往下找第一条 `because`，那就是根因。

### 13.8 模板报错阅读与工程实践

模板报错要**从上往下读第一条 error，再看紧跟它的 note**；后面成片的 `candidate ... not viable` 是候选枚举，通常可以跳过。

| 关键词 | 含义 | 处理 |
| --- | --- | --- |
| `no matching function for call to` | 没有可行重载，或推导失败 | 看下面第一条 note |
| `deduced conflicting types for parameter 'T'` | 同一个 T 被推成两种类型 | 显式指定 `f<double>(...)`，或先转换实参 |
| `constraints not satisfied` | concept 不满足 | 继续读 `because ...`，确认缺哪个操作 |
| `required from here` | 实例化调用链：谁触发了它 | 顺着链找到自己写的那行 |
| `undefined reference to ...` | 编译通过，链接找不到定义 | 模板定义没放头文件，或漏了显式实例化 |
| `static_assert failed` | 编译期断言失败 | 断言消息一般直接写了原因 |
| `substitution failure` / `no type named ...` | SFINAE 把候选剔除了 | 实参不具备该模板要求的成员/类型 |

模板的定义必须对**使用点**可见：编译器要在调用处实例化，只有声明不够。所以模板通常整个放头文件——标准允许模板定义出现在多个翻译单元（隐式 inline 语义），不会重复定义错误。想隐藏实现，用显式实例化把"哪些类型会被用到"钉死：

```cpp-fragment
// fixed_array.hpp
template <class T, std::size_t N>
class FixedArray { /* 声明和定义都在这里 */ };

extern template class FixedArray<int, 4>;   // 声明：这个实例在别处生成

// fixed_array.cpp
template class FixedArray<int, 4>;          // 显式实例化定义：只在这里生成
```

❌ 头文件只留声明、定义放 `.cpp`：

```cpp-fragment
// twice.hpp
template <class T> T twice(T x);

// twice.cpp
template <class T> T twice(T x) { return x + x; }
```

编译能过，链接时报：

```text
ld.lld: error: undefined symbol: int twice<int>(int)
```

原因：`main.cpp` 看到声明就生成了对 `twice<int>` 的调用，但 `twice.cpp` 里没有任何代码**使用**这个模板，编译器不会主动实例化它，目标文件里自然没有符号。修法是把定义放进头文件，或在 `twice.cpp` 里加 `template int twice<int>(int);` 显式实例化。

最后两条工程经验：一是模板会为每组实参生成代码，`std::vector<int>` 和 `std::vector<double>` 各有一份完整实现，过度使用会造成代码膨胀；二是模板编译慢，能用 `if constexpr` 在编译期剪枝就不要让所有分支都实例化。调试模板问题时先写一个 20 行以内的最小复现，比在几千行项目里读报错快得多。

## 14. STL 容器与迭代器

STL 把"数据结构、迭代器、算法"拆成三层：容器管内存，迭代器提供统一的遍历接口，算法只认迭代器区间 `[first, last)`。这种拆分让 `sort` 只写一遍就能用于 `vector`、`deque`、`array`，而容器不需要为每个算法实现成员版本。选容器只看三件事：**怎么访问**（随机还是顺序）、**在哪插入删除**、**要不要有序**。

### 14.1 三层结构与左闭右开区间

```text
容器            迭代器                 算法
vector    ──►   begin()/end()   ──►   sort(first, last)
list      ──►   双向迭代器       ──►   find / for_each
map       ──►   按 key 有序      ──►   lower_bound
```

算法从不接收容器，只接收两个迭代器。区间是**左闭右开** `[first, last)`：`first` 指向第一个元素，`last` 指向最后一个元素的**下一个位置**，永远不能解引用。这个约定带来四个好处：

1. 空区间可以表示成 `[p, p)`，不需要特判；
2. 区间长度就是 `last - first`（随机访问迭代器）或 `std::distance(first, last)`；
3. 相邻区间可以无缝拼接：`[a,b)` 与 `[b,c)` 合起来是 `[a,c)`；
4. 循环条件统一写成 `it != last`，不用处理"最后一个元素"的边界。

容器是**值语义**：`vector<int> b = a;` 拷贝全部元素，修改 `b` 不影响 `a`；容器里存的是对象的副本，不是引用。想要多态，就存 `std::unique_ptr<Base>`（见第 16 节的对象切片）；想要引用语义，用 `std::reference_wrapper` 或指针，但要自己保证生命周期。

复杂度是标准给的**承诺**，不是实现细节：`vector` 随机访问 O(1)、尾部插入摊还 O(1)；`map` 查找 O(log n)；`unordered_map` 平均 O(1)、最坏 O(n)。考试问"哪个操作是 O(1)"，答的是承诺，不是某台机器上的实测。

### 14.2 序列容器：vector / deque / list / array

| 容器 | 内存布局 | 随机访问 | 头部插入 | 尾部插入 | 中间插入/删除 | 迭代器类别 |
| --- | --- | --- | --- | --- | --- | --- |
| `vector` | 一整块连续内存 | O(1) | O(n) | 摊还 O(1) | O(n) | 随机访问 + 连续 |
| `deque` | 分段连续（块数组 + 中控表） | O(1) | O(1) | O(1) | O(n) | 随机访问 |
| `list` | 双向链表，节点分散在堆上 | 不支持 | O(1) | O(1) | O(1)（已定位） | 双向 |
| `forward_list` | 单向链表 | 不支持 | O(1) | 无 `push_back` | O(1)（已定位） | 前向 |
| `array` | 栈上连续定长数组 | O(1) | 不支持 | 不支持 | 不支持 | 随机访问 + 连续 |

`vector` 的连续布局是它最大的优势：缓存命中率高、能取 `data()` 传给 C 接口、能用 `memcpy`。代价是扩容要整体搬家，以及在中间插入要挪动后面所有元素。`deque` 用"分段连续"折中：两端插入都不需要搬整个容器，代价是多一层间接寻址，而且**没有** `reserve()` 和 `data()`。`list` 的每个节点额外存两个指针（64 位下 16 字节），换来 O(1) 接链；但节点分散，遍历时缓存不友好，实际往往比 `vector` 慢，哪怕复杂度看起来更优。

`vector` 的内存布局可以用三个指针描述（libstdc++/libc++ 都是这个模型）：

```text
begin ──► [1][2][3][4][5][ ][ ][ ]
          ▲                 ▲     ▲
          begin            end   cap
size() = end - begin = 5
capacity() = cap - begin >= 8
```

`list` 没有 `operator[]`，也不能用 `std::sort`（它要求随机访问迭代器），要用成员函数 `l.sort()`；`forward_list` 连 `size()` 都没有，插入用 `insert_after`。这些差异不是"实现没做"，而是数据结构本身不支持。

```cpp
#include <deque>
#include <iostream>
#include <iterator>
#include <list>

int main() {
    std::list<int> l{3, 1, 2};
    l.sort();
    l.push_front(0);
    l.push_back(4);
    l.insert(std::next(l.begin(), 2), 99);
    std::cout << std::distance(l.begin(), l.end()) << ' ';
    for (int x : l) std::cout << x << ',';
    std::cout << '\n';

    auto it = l.begin();
    std::advance(it, 3);
    std::cout << *it << '\n';

    std::deque<int> d{1, 2, 3};
    d.push_front(0);
    d.push_back(4);
    std::cout << d.front() << ' ' << d.back() << ' ' << d[2] << ' ' << d.size() << '\n';
}
```

输出：
```text
6 0,1,99,2,3,4,
2
0 4 2 5
```

`l.sort()` 是链表专属的归并排序，O(n log n) 且不需要随机访问；`std::next(l.begin(), 2)` 在链表上是 O(n) 的，因为它只能一步步 `++`。`std::distance` 对链表同样是 O(n)，本例只有 6 个元素所以无所谓，但对十万节点的链表循环调用 `distance` 就会变成 O(n²)。`deque` 的 `d[2]` 是 O(1)，但内部要经过"中控表 → 块 → 块内偏移"两次跳转，常数比 `vector` 大。

### 14.3 vector 的容量管理

`vector` 的逻辑大小和物理容量是分开的：

| 操作 | 对 size | 对 capacity | 说明 |
| --- | --- | --- | --- |
| `reserve(n)` | 不变 | 至少 n | 只分配存储，不构造元素 |
| `resize(n)` | 变成 n | 至少 n | 新增元素值初始化（`int` 为 0） |
| `clear()` | 变 0 | 不变 | 不释放内存 |
| `shrink_to_fit()` | 不变 | 请求缩到 size | 标准不保证生效 |
| `push_back` 超容量 | +1 | 扩大（常见 1.5–2 倍） | 所有指针/引用/迭代器失效 |

扩容倍率是**实现定义**的：libstdc++ 约 2 倍，MSVC 约 1.5 倍。不要打印或依赖具体数值，只能依赖标准保证的两件事：`capacity() >= size()`，以及"size 不超过 capacity 时不会重新分配"。摊还 O(1) 的证明：假设每次扩到 k 倍，n 次插入的总搬运量是 `n + n/k + n/k² + ... < n·k/(k-1)`，是 O(n)，均摊到每次就是 O(1)。

`reserve` 只改容量，**不构造元素**，所以 `v.reserve(100); v[0] = 1;` 是越界写（UB）——此时 `size()` 仍是 0。`resize` 才会真正构造元素，新增的 `int` 被值初始化为 0。`push_back` 接收已构造的对象（拷贝或移动），`emplace_back` 把参数直接转发给构造函数，在原地构造，少一次临时对象。

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    v.reserve(4);
    std::cout << (v.capacity() >= 4) << ' ' << v.size() << ' ' << v.empty() << '\n';

    v.push_back(10);
    int* p = v.data();
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    std::cout << (v.data() == p) << ' ' << v.size() << ' ' << (v.capacity() >= 4) << '\n';

    v.resize(6);
    std::cout << v.size() << ' ' << v[4] << ' ' << v[5] << '\n';

    v.resize(3);
    std::cout << v.size() << ' ' << v.back() << '\n';

    v.shrink_to_fit();
    std::cout << (v.capacity() >= v.size()) << '\n';
}
```

输出：
```text
1 0 1
1 4 1
6 0 0
3 30
1
```

前三行是容量与大小的对照：`reserve(4)` 后 capacity 至少 4、size 仍为 0；四次 `push_back` 没有超过容量，所以 `data()` 不变（第二行第一个 1）；`resize(6)` 把 size 抬到 6，新增的两个元素值初始化为 0；`resize(3)` 截断到 3，`back()` 是 30。`shrink_to_fit()` 之后只保证 `capacity() >= size()`，不保证等于 3。

❌ `reserve` 之后用下标访问"已分配但未构造"的元素：

```cpp-fragment
std::vector<int> v;
v.reserve(100);
v[0] = 42;        // size() 仍是 0，越界写，UB
int x = v[0];     // 同样 UB
```

`reserve` 分配的是原始存储，不是元素；对象生命周期还没开始。正确做法是 `resize(100)` 或 `push_back`。

### 14.4 关联容器：map / set / unordered_*

`map` 和 `set` 通常用红黑树实现，保证按 key 有序、查找/插入/删除 O(log n)；`unordered_map` 和 `unordered_set` 用哈希表，平均 O(1)、最坏 O(n)。选择只看两个问题：要不要按 key 顺序遍历/范围查询？key 能不能哈希？

| 项目 | `map` / `set` | `unordered_map` / `unordered_set` |
| --- | --- | --- |
| 底层结构 | 平衡二叉搜索树（红黑树） | 哈希表（桶 + 链表/开放寻址） |
| 迭代顺序 | 按 key 升序 | 未指定，不可依赖 |
| 查找/插入/删除 | O(log n) | 平均 O(1)，最坏 O(n) |
| 对 key 的要求 | 可用 `<` 比较 | 可哈希 + 可判等 |
| 迭代器失效 | 只有被删元素 | rehash 时全部失效（引用不失效） |
| 范围查询 | `lower_bound` / `upper_bound` O(log n) | 不支持 |

`map` 的 `operator[]` 是最容易误用的接口：key 不存在时它会**插入**一个值初始化的 mapped_type，并返回引用。所以"查询"应该用 `find`、`count` 或 C++20 的 `contains`；确定要插入或覆盖时，`try_emplace`（不存在才构造）和 `insert_or_assign`（存在则赋值）比 `operator[]` 少一次默认构造，也更表达意图。`at()` 在 key 不存在时抛 `out_of_range`，且要求异常可用。

`set` 可以看成"只有 key 的 map"：自动去重、有序遍历；元素是 **const** 的，不能 `*it = 5`，要改只能 erase 再 insert。`multimap` / `multiset` 允许重复 key，`equal_range` 返回所有等值元素。

`unordered_map` 的迭代顺序由哈希函数和桶数量决定，标准不作保证；同一个程序在不同标准库、不同插入顺序下输出都可能不同。所以不要打印它的遍历结果，也不要假设"先插入的先遍历"。`rehash` 发生在元素数超过 `bucket_count() * max_load_factor()` 时，此时所有迭代器失效，但指向元素的**引用和指针仍然有效**（节点没搬家）。自定义类型做 key 时要提供 `std::hash` 特化和 `operator==`；`std::pair` 没有现成的 `hash`，需要自己写。

```cpp
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

int main() {
    std::vector<std::string> words{"pear", "apple", "pear", "fig", "apple", "pear"};

    std::map<std::string, int> ordered;
    for (const auto& w : words) ++ordered[w];
    for (const auto& [k, v] : ordered) std::cout << k << '=' << v << ';';
    std::cout << '\n';

    std::unordered_map<std::string, int> hashed;
    for (const auto& w : words) hashed[w] += 1;
    std::cout << hashed.size() << ' ' << hashed.count("fig") << ' ' << hashed.at("pear") << '\n';

    auto it = ordered.find("fig");
    if (it != ordered.end()) std::cout << it->first << ' ' << it->second << '\n';
    std::cout << ordered.contains("kiwi") << '\n';

    auto [ins, ok] = ordered.try_emplace("fig", 100);
    std::cout << ins->second << ' ' << ok << '\n';

    ordered.insert_or_assign("fig", 7);
    std::cout << ordered["fig"] << ' ' << ordered.size() << '\n';
}
```

输出：
```text
apple=2;fig=1;pear=3;
3 1 3
fig 1
0
1 0
7 3
```

`map` 的遍历按 key 升序，所以输出 `apple;fig;pear`。`try_emplace("fig", 100)` 发现 key 已存在，**不覆盖**，返回指向旧元素的迭代器和 `false`，所以输出 `1 0`；随后 `insert_or_assign("fig", 7)` 才把值改成 7，size 仍是 3。`hashed` 只打印与顺序无关的 `size/count/at`，避免依赖哈希表的遍历顺序。

一个隐蔽的性能坑：对 `set` 用全局 `std::lower_bound` 是 O(n)，因为它只会用 `std::advance` 一步步走；要用成员函数 `s.lower_bound(x)`（O(log n)）。`map` 同理。

### 14.5 容器适配器：stack / queue / priority_queue

| 适配器 | 默认底层容器 | 主要操作 | 语义 |
| --- | --- | --- | --- |
| `stack` | `deque` | `push` / `pop` / `top` | 后进先出 |
| `queue` | `deque` | `push` / `pop` / `front` / `back` | 先进先出 |
| `priority_queue` | `vector` | `push` / `pop` / `top` | 堆，默认大根堆 |

适配器**不提供迭代器**，不能遍历、不能套 STL 算法；要看全部元素只能逐个 `pop`（会清空容器）。`priority_queue` 默认用 `std::less` 比较，堆顶是最大值；要小根堆就传 `std::greater<int>`。它的 `pop()` 返回 `void`，取值必须先 `top()`。适配器也没有 `clear()`，清空只能循环 `pop()` 或整体赋空。

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
    std::cout << balanced("([]{})") << balanced("(]") << balanced("((") << '\n';

    std::queue<int> q;
    for (int i = 1; i <= 3; ++i) q.push(i * 10);
    while (!q.empty()) {
        std::cout << q.front() << ',';
        q.pop();
    }
    std::cout << '\n';

    std::priority_queue<int> pq;
    for (int x : {5, 1, 9, 3}) pq.push(x);
    std::cout << pq.top() << ' ' << pq.size() << '\n';
    pq.pop();
    std::cout << pq.top() << '\n';

    std::priority_queue<int, std::vector<int>, std::greater<int>> minq;
    for (int x : {5, 1, 9, 3}) minq.push(x);
    std::cout << minq.top() << '\n';
}
```

输出：
```text
100
10,20,30,
9 4
5
1
```

括号匹配是栈的原型题：遇左括号入栈，遇右括号与栈顶比对，最后栈必须为空。`queue` 的 `front()` 是队头，`pop()` 删队头。`priority_queue` 的 `top()` O(1)、`push/pop` O(log n)，底层就是 `std::push_heap` / `std::pop_heap`；小根堆用 `greater`。

### 14.6 迭代器类别与遍历

迭代器是"广义指针"，按能力分成六级。算法只要求它能完成工作所需的最小类别，这就是同一个 `find` 能用于 `vector` 和 `list` 的原因。

| 类别 | 能力 | 典型来源 | 可用算法举例 |
| --- | --- | --- | --- |
| 输入 input | 单向、单遍、只读 | `istream_iterator` | `copy`、`accumulate` |
| 输出 output | 单向、单遍、只写 | `ostream_iterator`、`back_inserter` | `copy`、`transform` |
| 前向 forward | 可多遍、只能 `++` | `forward_list`、`unordered_*` | `find`、`for_each` |
| 双向 bidirectional | 还能 `--` | `list`、`map`、`set` | `reverse`、`prev` |
| 随机访问 random access | `+n`、`-`、下标、比大小 | `vector`、`deque`、`array`、`string` | `sort`、`binary_search` |
| 连续 contiguous（C++20） | 内存物理连续，可 `data()` | `vector`、`array`、`string` | 传给 C 接口 |

`std::distance` 对随机访问迭代器是 O(1)（直接相减），对其他类别是 O(n)（一步步走）；`std::advance` 同理。`std::next` / `std::prev` 返回**新**迭代器，不修改传入的那个；`std::advance` 修改传入的迭代器且返回 `void`——这是三者最容易混的地方。C++20 的 `std::contiguous_iterator` 概念比随机访问更强：它保证 `&*(it + n) == data() + n`，所以只有 `vector`、`array`、`string` 满足。

遍历有三种写法：下标（只有随机访问容器能用）、迭代器、range-for。range-for 展开后就是 `begin()` / `end()` 那一套：

```cpp-fragment
for (auto&& x : range) { ... }
// 展开为：
auto&& __r = range;
auto __b = begin(__r), __e = end(__r);
for (; __b != __e; ++__b) { auto&& x = *__b; ... }
```

只读遍历用 `cbegin()` / `cend()` 拿 `const_iterator`，能防止误改；范围 for 里写 `const auto&` 效果相同。

```cpp
#include <array>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

template <class It>
void describe() {
    using C = typename std::iterator_traits<It>::iterator_category;
    std::cout << std::is_same_v<C, std::random_access_iterator_tag> << ' '
              << std::is_same_v<C, std::bidirectional_iterator_tag> << ' '
              << std::is_same_v<C, std::forward_iterator_tag> << '|';
}

int main() {
    describe<std::vector<int>::iterator>();
    describe<std::list<int>::iterator>();
    describe<std::forward_list<int>::iterator>();
    std::cout << '\n';

    static_assert(std::contiguous_iterator<std::vector<int>::iterator>);
    static_assert(std::bidirectional_iterator<std::list<int>::iterator>);
    static_assert(std::forward_iterator<std::forward_list<int>::iterator>);

    std::vector<int> v{1, 2, 3, 4, 5};
    std::list<int> l{1, 2, 3, 4, 5};
    std::cout << std::distance(v.begin(), v.end()) << ' '
              << std::distance(l.begin(), l.end()) << '\n';
    std::cout << *std::next(v.begin(), 3) << ' ' << *std::prev(v.end()) << '\n';

    std::array<int, 3> a{7, 8, 9};
    std::string s = "abc";
    std::cout << *a.begin() << ' ' << s.front() << '\n';
}
```

输出：
```text
1 0 0|0 1 0|0 0 1|
5 5
4 5
7 a
```

`describe` 用 `iterator_traits` 取出迭代器类别标签，三行分别是随机访问、双向、前向。`distance` 对两者都返回 5，但 `vector` 是相减，`list` 是走五步；数据量小时看不出差别。`std::next(v.begin(), 3)` 直接跳到下标 3 的 4，`std::prev(v.end())` 是最后一个元素 5。`array` 和 `string` 的迭代器也是连续迭代器。

### 14.7 迭代器失效

迭代器失效（iterator invalidation）是 STL 里最容易造成 UB 的地方：容器修改后，旧迭代器可能指向已释放的内存，继续解引用或 `++` 都是未定义行为。规则如下：

| 容器 | 什么操作让迭代器失效 | 引用/指针是否也失效 |
| --- | --- | --- |
| `vector` | 扩容（`push_back`/`insert`/`resize` 超容量）→ 全部；`erase` → 被删位置及之后 | 扩容时全部失效；`erase` 时被删及之后失效 |
| `deque` | 头/尾插入 → 迭代器全部失效；中间插入删除 → 全部 | 头尾插入时引用仍有效；中间操作全部失效 |
| `list` | 只有指向被删元素的那个 | 只有被删元素 |
| `map` / `set` | 只有指向被删元素的那个 | 只有被删元素 |
| `unordered_*` | rehash → 全部迭代器；否则只有被删元素 | rehash 后引用仍有效；删除只影响被删元素 |

`deque` 的插入与删除并不对称：在头部或尾部 `erase` 只让被删元素的迭代器和引用失效，在中间 `erase` 才让全部失效；而任何位置插入后引用都还有效（头尾插入）或全部失效（中间插入）。表格里"头/尾插入"一行只约束插入，不要套到删除上。

`vector` 的 `erase` 会让"被删位置及之后"的迭代器失效，因为后面的元素整体前移了；`insert` 同理。所以循环删除不能写 `for (...; ++it) if (...) v.erase(it);`，正确写法是 `it = v.erase(it)`——`erase` 返回被删元素的下一个有效位置。批量删除更推荐 erase-remove 惯用法：`v.erase(std::remove_if(v.begin(), v.end(), pred), v.end())`，一次前移、一次截断。

```cpp
#include <algorithm>
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

    std::map<int, int> m{{1, 10}, {2, 20}, {3, 30}, {4, 40}};
    for (auto it = m.begin(); it != m.end();) {
        if (it->first % 2 == 0) it = m.erase(it);
        else ++it;
    }
    std::cout << m.size() << ' ' << m.begin()->first << ' ' << m.rbegin()->first << '\n';

    std::vector<int> w{1, 2, 3, 4, 5, 6};
    w.erase(std::remove_if(w.begin(), w.end(), [](int x) { return x % 2 == 0; }), w.end());
    for (int x : w) std::cout << x << ',';
    std::cout << '\n';
}
```

输出：
```text
1,3,5,
2 1 3
1,3,5,
```

`vector` 与 `map` 的循环删除都用了 `it = erase(it)`；`map` 的 erase 只让被删节点失效，所以另一种写法 `m.erase(it++)` 也成立（C++11 起），但 `it = erase(it)` 更统一。第三个容器用 erase-remove：`remove_if` 把要保留的元素前移，返回新逻辑结尾，`erase` 再截断。

❌ 三种典型错误：

```cpp-fragment
// 1. erase 之后继续用旧迭代器
for (auto it = v.begin(); it != v.end(); ++it)
    if (*it % 2 == 0) v.erase(it);   // erase 后 it 已失效，++it 是 UB

// 2. 循环外缓存 end()
auto e = v.end();
for (auto it = v.begin(); it != e; ++it)
    if (*it == 0) v.push_back(1);    // 扩容后 e 悬垂，it != e 是 UB

// 3. 对空 vector 用 size()-1 当上界
for (std::size_t i = 0; i < v.size() - 1; ++i)
    sum += v[i];                     // 空容器时 size()-1 回绕成 SIZE_MAX
```

第 3 条严格说不是迭代器失效，而是无符号回绕，但后果一样是越界。空容器要单独判空，或把循环写成 `i + 1 < v.size()`。

### 14.8 选型建议与常见坑

| 需求 | 选谁 | 理由 |
| --- | --- | --- |
| 默认的"一组元素" | `vector` | 连续内存、随机访问、摊还 O(1) 尾插 |
| 频繁头尾插入 | `deque` | 两端 O(1)，仍支持随机访问 |
| 频繁在中间插入删除，且已持有位置 | `list` | O(1) 接链，代价是缓存不友好 |
| 按 key 有序、要范围查询 | `map` / `set` | 红黑树，O(log n)，天然有序 |
| 只要平均 O(1) 查找，不关心顺序 | `unordered_map` / `unordered_set` | 哈希表 |
| 后进先出 / 先进先出 / 取最值 | `stack` / `queue` / `priority_queue` | 语义清晰，封装好 |
| 定长、要栈上分配 | `std::array` | 无堆分配，size 是编译期常量 |

选型的默认答案是 `vector`：除非有明确理由，否则不要用 `list`。`list` 的 O(1) 插入只在"已经拿到位置"时成立，找位置本身是 O(n)；而 `vector` 的连续内存让线性扫描快一个数量级。`deque` 适合双端队列和滑动窗口，但它的迭代器比 `vector` 重，能 `vector` 就 `vector`。

最后集中列几个坑：

- `vector<bool>` 是特化：`v[0]` 返回代理对象，`&v[0]` 不合法，`auto x = v[0]` 拿到的是代理而不是 `bool&`。需要真 bool 数组就用 `vector<char>` 或 `array<bool,N>`。
- `size()` 返回无符号的 `size_type`，与 `int` 比较会触发 `-Wsign-compare`；空容器上 `size()-1` 会回绕。
- `v[i]` 不检查越界；要检查用 `v.at(i)`（越界抛 `out_of_range`）。
- `map[key]` 会插入（14.4）；查询请用 `find` / `contains`。
- `reserve` 之后保存的指针/引用，在扩容后悬垂（14.3）。
- `list` 没有 `operator[]`，`std::sort(l.begin(), l.end())` 编译错误，用 `l.sort()`。
- `priority_queue` 的 `pop()` 返回 `void`，取值要先 `top()`。

## 15. 算法与常用技巧

`<algorithm>` 和 `<numeric>` 里的算法都是"迭代器区间上的函数模板"：接收 `[first, last)`，返回迭代器或值，从不接收容器。算法只能重排或覆盖已有元素，**不能改变容器大小**——所以"删除"要算法和容器的 `erase` 配合。命名有两个后缀：`_if` 表示用谓词代替值（`find_if`、`remove_if`），`_n` 表示用个数代替尾迭代器（`copy_n`、`fill_n`）。

### 15.1 算法清单与复杂度

| 算法 | 复杂度 | 迭代器要求 | 说明 |
| --- | --- | --- | --- |
| `sort` | O(n log n) | 随机访问 | 内省排序，不稳定 |
| `stable_sort` | O(n log n) | 随机访问 | 稳定；无额外内存时退化为 O(n log² n) |
| `partial_sort` | O(n log k) | 随机访问 | 只把前 k 个排好 |
| `nth_element` | 平均 O(n) | 随机访问 | 第 k 小就位，两侧不排序 |
| `find` / `find_if` | O(n) | 输入迭代器 | 线性查找 |
| `binary_search` | O(log n) 次比较 | 前向（需已排序） | 只返回 bool |
| `lower_bound` / `upper_bound` | O(log n) 次比较 | 前向（需已排序） | 返回位置 |
| `count` / `count_if` | O(n) | 输入迭代器 | 计数 |
| `unique` | O(n) | 前向 | 只去**相邻**重复 |
| `remove` / `remove_if` | O(n) | 前向 | 只前移，不删除 |
| `min_element` / `max_element` | O(n) | 输入迭代器 | 返回迭代器 |
| `accumulate` | O(n) | 输入迭代器 | 在 `<numeric>` |
| `partial_sum` | O(n) | 输入迭代器 | 前缀和，在 `<numeric>` |

复杂度是选算法的第一依据：n = 10⁵ 时，O(n log n) 约 1.7×10⁶ 次操作，O(n²) 是 10¹⁰，后者必然超时。考试给数据范围，其实就是在提示复杂度：n ≤ 20 可以指数级，n ≤ 5000 可以 O(n²)，n ≤ 10⁶ 必须 O(n log n) 或 O(n)。

还有两个容易忽略的约定。第一，算法返回的是**迭代器**，不是下标：`lower_bound` 返回位置后要 `it - v.begin()` 才得到下标，而这个减法只对随机访问迭代器成立，链表上要用 `std::distance`（O(n)）。第二，除排序类算法外，大多数算法不会让迭代器失效；但 `remove` / `unique` 只是把元素前移，返回的新结尾之后是"未指定值"，读它们是合法的但内容没有意义，正确做法永远是配 `erase` 截断。

### 15.2 排序

`std::sort` 用内省排序（introsort：快排 + 堆排 + 插入排序），平均和最坏都是 O(n log n)，因此不会像朴素快排那样在特定输入上退化成 O(n²)。它**不稳定**：相等元素的相对顺序可能改变。需要稳定就 `stable_sort`（归并排序，代价是额外空间）。只关心前 k 名用 `partial_sort`（O(n log k)，k 是 `middle - first`）；只关心第 k 小用 `nth_element`（平均 O(n)，比全排序快）。

比较器必须满足**严格弱序**（strict weak ordering）：`comp(a,a)` 为 false，且传递。写 `<=` 会破坏这一条，标准库不检查，行为未定义，实践中可能越界访问。升序写 `a < b`，降序写 `a > b`，或直接传 `std::greater<int>()`。多关键字排序把 tie-break 写进比较器，或先按次要关键字排、再 `stable_sort` 主关键字。

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
        return a.name < b.name;
    });
    std::cout << st.front().name << ' ' << st.back().name << '\n';

    std::vector<int> w{7, 2, 9, 4, 1, 8, 3};
    std::nth_element(w.begin(), w.begin() + 2, w.end());
    std::cout << w[2] << ' ' << *std::min_element(w.begin(), w.begin() + 3) << ' '
              << *std::max_element(w.begin(), w.begin() + 3) << '\n';
}
```

输出：
```text
1,1,2,4,5,9,
9,5,4,2,1,1,
amy:90,bob:90,cid:80,
amy cid
3 1 3
```

`stable_sort` 按分数降序时，amy 和 bob 同为 90，稳定排序保留原来的先后，所以是 `amy:90,bob:90`。随后带 `name` tie-break 的 `sort` 让 amy 排在 bob 前，`front()` 是 amy、`back()` 是 cid。`nth_element(begin, begin+2)` 之后，下标 2 上正好是第 3 小的 3；前三个元素是 `{1,2,3}` 的某种排列，所以它们的最小值是 1、最大值是 3。

❌ 比较器写成非严格弱序：

```cpp-fragment
std::sort(v.begin(), v.end(), [](int a, int b) { return a <= b; }); // a==b 时为 true，UB
std::sort(v.begin(), v.end(), [](int a, int b) { return a - b; });  // 差值可能溢出
```

第一条违反 `comp(a,a) == false`；第二条在 `a=-2e9, b=2e9` 时 `a-b` 溢出（有符号溢出是 UB）。比较器里只做比较，不做减法。

`partial_sort(first, middle, last)` 只保证 `[first, middle)` 是全局最小的 k 个且有序，后面部分顺序未指定；`nth_element` 更弱，只保证第 k 个位置正确、左边都不大于它、右边都不小于它，两侧都不排序。求"前 10 名"用 `partial_sort` 是 O(n log 10)，比全排序 O(n log n) 快得多；求中位数用 `nth_element`，平均 O(n)。这两个函数都会打乱原区间，之后不能再假设原顺序。

### 15.3 查找与二分

线性查找 `std::find` 是 O(n)，适用于任何输入迭代器、无需有序。二分查找的前提是**区间已经按比较器排好序**：`binary_search` 只回答"在不在"；要位置用 `lower_bound`（第一个 **≥** value 的位置）和 `upper_bound`（第一个 **>** value 的位置）。两者之差就是等于 value 的元素个数，`equal_range` 一次返回这一对。

二分还有两个细节。第一，比较器必须与排序时一致：升序用默认 `<`，降序要同时传 `std::greater<>`，混用会得到错误结果。第二，`lower_bound` 返回的迭代器可能等于 `end()`（所有元素都小于 value），解引用前必须判断。对 `set` / `map` 要用成员 `lower_bound`（O(log n)），全局版本只会线性前进。

二分的比较次数是 ⌈log₂n⌉：n = 10⁶ 时约 20 次，n = 10⁹ 时约 30 次。这也是"二分答案"能把 O(n) 的 `check` 变成 O(n log S) 的原因——log S 通常只有 30 左右。`binary_search` 只返回 bool，如果还需要"第一个 ≥ x 的位置"，必须用 `lower_bound`，不要先用 `binary_search` 判断存在再 `find`（那样又退化成 O(n)）。

```cpp
#include <algorithm>
#include <functional>
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

    std::vector<int> d{9, 7, 5, 3, 1};
    auto it = std::lower_bound(d.begin(), d.end(), 5, std::greater<int>());
    std::cout << (it - d.begin()) << ' ' << *it << '\n';

    std::cout << (std::find(v.begin(), v.end(), 4) == v.end()) << '\n';
}
```

输出：
```text
1 0
1 3 2
2
5
2 5
1
```

`v` 里有两个 3：`lower_bound` 指向第一个 3（下标 1），`upper_bound` 指向 5（下标 3），差为 2，正好是 3 的个数。`lower_bound(4)` 返回第一个 ≥ 4 的元素 5。降序序列用 `greater` 时，`lower_bound` 的语义变成"第一个不满足 `> 5` 的位置"，即 5 本身（下标 2）。`find(4)` 找不到，返回 `end()`，所以比较结果为 1。

❌ 对未排序区间二分，或混用比较器：

```cpp-fragment
std::vector<int> v{5, 1, 4, 2};
std::binary_search(v.begin(), v.end(), 4);  // 未排序，结果无意义

std::sort(v.begin(), v.end());                                  // 升序
std::lower_bound(v.begin(), v.end(), 4, std::greater<int>());   // 比较器不一致
```

### 15.4 去重、删除与变换

`std::remove` / `remove_if` **不删除元素**，只把要保留的元素依次前移，返回新的逻辑结尾；容器大小不变，尾部残留的是被覆盖后的旧值。真正删除要 erase-remove 两连：`v.erase(std::remove(...), v.end())`。C++20 的 `std::erase_if(v, pred)` 把这两步合成一个函数，更不容易忘。

`std::unique` 只去除**相邻**重复，返回新的逻辑结尾；想对全体去重必须先排序。`unique` 同样不改变 size，所以也要配 `erase`。`std::transform` 对每个元素做映射，结果写到另一区间时配 `std::back_inserter`（它会调用 `push_back`，自动扩容）。

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> v{1, 1, 2, 2, 2, 3, 4, 4};
    auto last = std::unique(v.begin(), v.end());
    std::cout << v.size() << ' ' << (last - v.begin()) << '\n';
    v.erase(last, v.end());
    for (int x : v) std::cout << x << ',';
    std::cout << '\n';

    std::vector<int> w{1, 2, 3, 4, 5, 6};
    std::erase_if(w, [](int x) { return x % 2 == 0; });
    for (int x : w) std::cout << x << ',';
    std::cout << '\n';

    std::vector<int> u{5, 3, 5, 1, 3};
    std::sort(u.begin(), u.end());
    u.erase(std::unique(u.begin(), u.end()), u.end());
    std::cout << u.size() << ' ' << u.front() << ' ' << u.back() << '\n';

    std::vector<int> src{1, 2, 3};
    std::vector<int> dst;
    std::transform(src.begin(), src.end(), std::back_inserter(dst), [](int x) { return x * x; });
    for (int x : dst) std::cout << x << ',';
    std::cout << '\n';
}
```

输出：
```text
8 4
1,2,3,4,
1,3,5,
3 1 5
1,4,9,
```

第一行是本节的关键：`unique` 之前 size 是 8，返回的 `last` 在下标 4，**但 size 仍是 8**；只有 `erase(last, v.end())` 之后才真正变成 4 个元素。`u` 先排序再 `unique`，因为原序列 `5,3,5,1,3` 的重复不相邻，不排序只能去掉相邻的。

❌ 两个典型误用：

```cpp-fragment
std::vector<int> v{1, 2, 1, 2};
auto last = std::unique(v.begin(), v.end());  // 只去相邻重复，结果仍是 1,2,1,2

v.erase(std::remove(v.begin(), v.end(), 0), v.end());  // 正确
std::remove(v.begin(), v.end(), 0);                    // 忘了 erase，size 不变
```

### 15.5 前缀和、差分与 accumulate

`std::accumulate(first, last, init)` 的累加类型由 **init** 决定，不是由元素类型决定：init 是 `int`，累加就在 `int` 里做；init 是 `unsigned`，就在 `unsigned` 里做。无符号溢出会回绕（定义良好），有符号溢出则是 UB。两个 3×10⁹ 相加，`0u` 得到回绕的 1705032704，`0ULL` 才得到 6000000000。求和前先判断结果上界，超过 2³¹ 就写 `0LL` 或 `0ULL`，这是"init 决定类型"最典型的坑。

前缀和数组 `pref[i]` 表示前 i 个元素之和，`pref[0] = 0`，则区间 `[l, r)` 的和是 `pref[r] - pref[l]`，查询 O(1)，预处理 O(n)。差分是前缀和的逆运算：要给 `[l, r)` 整体加 x，只需 `diff[l] += x; diff[r] -= x;`，最后做一次前缀和还原，m 次区间修改 + 一次还原是 O(n + m)，比每次 O(n) 快。

```cpp
#include <cstddef>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> v{4, 7, 2, 9, 5};
    std::cout << std::accumulate(v.begin(), v.end(), 0) << ' '
              << std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>()) << '\n';

    std::vector<long long> pref(v.size() + 1, 0);
    for (std::size_t i = 0; i < v.size(); ++i) pref[i + 1] = pref[i] + v[i];
    auto sum = [&pref](std::size_t l, std::size_t r) { return pref[r] - pref[l]; };
    std::cout << sum(1, 3) << ' ' << sum(0, 5) << ' ' << sum(2, 2) << '\n';

    std::vector<long long> diff(6, 0);
    auto add = [&diff](std::size_t l, std::size_t r, long long x) {
        diff[l] += x;
        diff[r] -= x;
    };
    add(1, 4, 5);
    add(2, 5, 3);
    add(0, 2, 2);

    std::vector<long long> val(5, 0);
    std::partial_sum(diff.begin(), diff.end() - 1, val.begin());
    for (long long x : val) std::cout << x << ',';
    std::cout << '\n';

    std::vector<unsigned> big{3000000000u, 3000000000u};
    std::cout << std::accumulate(big.begin(), big.end(), 0u) << '\n';
    std::cout << std::accumulate(big.begin(), big.end(), 0ULL) << '\n';
}
```

输出：
```text
27 2520
9 27 0
2,7,8,8,3,
1705032704
6000000000
```

`sum(1,3)` 是 `v[1]+v[2]=9`，`sum(0,5)` 是全和 27，`sum(2,2)` 是空区间 0——这正是前缀和"左闭右开、pref[0]=0"的好处。差分数组经过 `partial_sum` 还原成每个位置的实际增量：位置 0 只有 +2，位置 1 是 +5，位置 2 是 +3-2，位置 3 没有修改，位置 4 是 -5（区间结束）。最后两行演示 accumulate 的 init 类型：`0u` 得到回绕的 1705032704，`0ULL` 得到正确的 6000000000。

### 15.6 二分答案与双指针

**二分答案**用于"求最小/最大的可行值"：答案有单调性（若 x 可行，则比 x 更宽松的值也可行），直接对答案区间二分，每次用 O(n) 的 `check` 判断。模板是 `while (lo < hi) { mid = lo + (hi-lo)/2; if (check(mid)) hi = mid; else lo = mid + 1; }`，最后 `lo` 就是最小可行值。`mid` 必须写成 `lo + (hi-lo)/2`，避免 `lo+hi` 溢出；`hi = mid` 与 `lo = mid + 1` 的搭配保证区间每轮缩小，不会死循环。

**双指针**利用单调性把 O(n²) 降到 O(n)：有序数组求两数之和，`i` 从头、`j` 从尾，和偏小就 `++i`，偏大就 `--j`；滑动窗口用 `[l, r)` 维护一个满足条件的区间，`r` 扩张、`l` 收缩，每个元素最多进出一次；快慢指针在原数组上原地去重/删除，`fast` 扫描、`slow` 写。三者共同的前提是"指针移动方向有依据"，不能随便套。

双指针为什么不会漏解？以有序数组两数之和为例：当前 `a[i] + a[j] < target` 时，`a[i]` 与任何 `≤ a[j]` 的数相加都更小，所以 `i` 可以安全右移，被排除的组合里不可能有答案；`> target` 时同理左移 `j`。滑动窗口的合法性依赖"窗口和随 `r` 增大、随 `l` 增大而减小"这一单调性；如果数组有负数，这个性质不成立，窗口就不能这样伸缩。写之前先确认单调性，再写循环。

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

这是"分割数组使最大子段和最小"：`check(limit)` 贪心地把元素尽量塞进当前段，段数 ≤ k 就可行。下界取最大元素（单元素段至少要装下它），上界取总和（只分一段）。k=1 时答案 32（不许分），k=5 时答案 10（每个元素一段，取最大），k=2 时 18，k=3 时 14。`check` 是 O(n)，二分 O(log S)，总 O(n log S)。

```cpp
#include <algorithm>
#include <cstddef>
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

int removeDuplicates(std::vector<int>& a) {
    if (a.empty()) return 0;
    std::size_t slow = 0;
    for (std::size_t fast = 1; fast < a.size(); ++fast) {
        if (a[fast] != a[slow]) a[++slow] = a[fast];
    }
    return static_cast<int>(slow + 1);
}

int main() {
    std::vector<int> a{1, 2, 3, 4, 5, 6};
    std::cout << twoSumPairs(a, 7) << '\n';

    std::vector<int> b{2, 3, 1, 2, 4, 3};
    std::cout << shortestWindow(b, 7) << ' ' << shortestWindow(b, 100) << ' '
              << shortestWindow(b, 1) << '\n';

    std::vector<int> c{1, 1, 2, 2, 2, 3};
    int n = removeDuplicates(c);
    std::cout << n << ' ';
    for (int i = 0; i < n; ++i) std::cout << c[i] << ',';
    std::cout << '\n';
}
```

输出：
```text
3
2 0 1
3 1,2,3,
```

`twoSumPairs` 在有序数组上数出和为 7 的三对：1+6、2+5、3+4。`shortestWindow(b, 7)` 的最短窗口是 `[4,3]`，长度 2；目标 100 无解返回 0；目标 1 时单个元素就够，长度 1。`removeDuplicates` 返回新长度 3，并把 `1,2,3` 写到数组前三个位置。

### 15.7 位运算技巧

| 表达式 | 作用 | 示例（x = 0b10110100） |
| --- | --- | --- |
| `x & -x` | 最低位的 1（lowbit） | 4（0b100） |
| `x & (x-1)` | 清掉最低位的 1 | 176（0b10110000） |
| `(x & (x-1)) == 0` | 判断 2 的幂（x>0） | false |
| `x >> k` / `x << k` | 除以/乘以 2ᵏ | `x>>2 = 45` |
| `x & 1` | 奇偶 | 0 |
| `std::popcount(x)` | 1 的个数（C++20） | 4 |
| `std::countr_zero(x)` | 末尾 0 的个数（C++20） | 2 |
| `x ^ x` / `x ^ 0` | 抵消 / 不变 | 0 / x |

异或的三条性质——`a^a=0`、`a^0=a`、异或满足交换律和结合律——让"只有一个数出现奇数次"可以直接全部异或。`std::popcount`、`std::countr_zero`、`std::bit_width` 在 C++20 的 `<bit>` 里，比自己写循环清晰，也比编译器内建函数可移植。

```cpp
#include <bit>
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
    std::cout << (x & 1) << ' ' << ((x & 1) == 0) << '\n';
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
0 1
```

`x & -x` 得到 4，因为最低位的 1 在 bit 2。`x & (x-1)` 把它清掉，剩 176。`popcount` 数出 4 个 1。`countr_zero` 是 2。数组 `4,1,2,1,2` 里只有 4 出现一次，异或全部抵消后剩 4。

❌ 优先级陷阱：`&` 的优先级低于 `==`，`^` 低于 `|`，`<<` 低于 `+`：

```cpp-fragment
if (x & 1 == 0) { }        // 解析为 x & (1 == 0)，恒为 0
int y = 1 << 2 + 1;        // 解析为 1 << 3，不是 (1<<2)+1
bool b = a ^ b | c;        // 解析为 a ^ (b | c)
```

位运算一律加括号，这是唯一可靠的写法。`-Wall` 会对前两条给出 `-Wparentheses` / `-Wshift-op-parentheses` 警告——警告本身就是提示。

### 15.8 综合示例：固定种子 LCG

需要随机数据时，标准库的 `std::rand()` 在不同平台上序列不同，`std::random_device` 又可能不确定，都会让"逐字比对输出"失效。教学代码用自写的线性同余发生器（linear congruential generator, LCG）：`s = s * a + c (mod 2³²)`，固定 `s` 就固定整个序列。取 `a = 1664525`、`c = 1013904223`（Numerical Recipes 的常数），用 `std::uint32_t` 保存状态——无符号溢出在 C++ 里是**定义良好**的模 2³² 回绕，正好充当取模。

```cpp
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

// Numerical Recipes 的 LCG 常数；unsigned 的模 2^32 回绕就是取模
std::uint32_t next(std::uint32_t& s) {
    s = s * 1664525u + 1013904223u;
    return s;
}

int main() {
    std::uint32_t seed = 2024u;
    std::vector<int> v;
    for (int i = 0; i < 10; ++i) v.push_back(static_cast<int>(next(seed) % 100u));

    for (std::size_t i = 0; i < v.size(); ++i)
        std::cout << (i == 0 ? "" : " ") << v[i];
    std::cout << '\n';

    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
    std::cout << v.size() << ':';
    for (int x : v) std::cout << x << ',';
    std::cout << '\n';

    std::vector<long long> pref(v.size() + 1, 0);
    for (std::size_t i = 0; i < v.size(); ++i) pref[i + 1] = pref[i] + v[i];
    std::cout << pref.back() << ' '
              << (pref.back() == std::accumulate(v.begin(), v.end(), 0LL)) << '\n';

    seed = 2024u;
    std::cout << next(seed) << ' ' << next(seed) << '\n';
}
```

输出：
```text
27 14 29 28 87 78 85 40 27 22
9:14,22,27,28,29,40,78,85,87,
410 1
87935527 3706503514
```

同样的 `seed = 2024` 在任何平台都得到同一串数，这是可复现验证的前提。10 个数排序去重后剩 9 个（27 重复），和是 410；`pref.back()` 与 `accumulate(..., 0LL)` 相等，返回 1，说明手写前缀和与标准算法一致。最后重新播种再取两个原始值，用来确认 LCG 的状态推进是确定的。

注意 `next(seed) % 100u` 的结果范围是 0–99，转成 `int` 安全；如果 LCG 直接返回 `uint32_t` 并转 `int`，超过 `INT_MAX` 时是实现定义行为，所以要么取模，要么保持无符号。

### 15.9 考试视角

| 题目特征 | 首选 | 复杂度 |
| --- | --- | --- |
| 求第 k 小 / 前 k 名 | `nth_element` / `partial_sort` | 平均 O(n) / O(n log k) |
| 静态数组多次区间求和 | 前缀和 | 预处理 O(n)，查询 O(1) |
| 多次区间加、最后统一读取 | 差分 | 修改 O(1)，还原 O(n) |
| 有序数组两数之和 | 双指针 | O(n) |
| 最短/最长满足条件的子数组 | 滑动窗口 | O(n) |
| "最小化最大值" / "最大化最小值" | 二分答案 | O(n log S) |
| 只出现一次的数 | 异或 | O(n) |
| 去重 | 排序 + `unique` + `erase` | O(n log n) |

写算法题的常见顺序是：先估复杂度，再选数据结构，最后才写代码。能排序就先排序（O(n log n) 换来的有序性通常值得），有序之后二分、双指针、去重都能用。不要为了"理论最优"把 `vector` 换成 `list`，常数和缓存往往比复杂度差异更大。

## 16. 常见陷阱清单与速查表

这一节是"考前最后十分钟"：把最容易错的类型、优先级、容器代价和头文件集中成表，再配四个能直接跑的小例子。表比文字适合速查，但每一条背后的原因在前三节都能找到对应段落。

### 16.1 类型大小与整数边界

| 类型 | 典型大小（字节） | 标准保证 | 取值范围（典型） |
| --- | --- | --- | --- |
| `bool` | 1 | 至少 1 | `false` / `true` |
| `char` | 1 | 恰好 1 | -128–127 或 0–255（实现定义） |
| `short` | 2 | 至少 2 | -32768–32767 |
| `int` | 4 | 至少 2 | -2147483648–2147483647 |
| `long` | 4（Windows）/ 8（Linux） | 至少 4 | 随平台 |
| `long long` | 8 | 至少 8 | -2⁶³–2⁶³-1 |
| `float` | 4 | 至少 4 | 约 ±3.4×10³⁸，6–7 位有效数字 |
| `double` | 8 | 至少 8 | 约 ±1.8×10³⁰⁸，15–17 位有效数字 |
| 指针 | 4（32 位）/ 8（64 位） | 不保证 | 与地址空间有关 |
| `std::size_t` | 8（64 位） | 足够存对象大小 | 无符号 |

"典型大小"不是标准保证：标准只规定相对顺序和最小值，`sizeof(long)` 在 Windows（LLP64）是 4、在 Linux（LP64）是 8。考试写"long 是 8 字节"会被扣分；可移植的写法是用 `<cstdint>` 的 `int32_t` / `int64_t`，或 `long long`。`sizeof(char) == 1` 是唯一被标准钉死的。

整数有三个高频坑：有符号溢出是 **UB**（不是回绕），无符号溢出才是模 2ⁿ 回绕；`/` 和 `%` 向零截断，`-7/2 == -3`、`-7%2 == -1`；`int` 与 `unsigned` 混合运算时 `int` 先转成 `unsigned`，`-1 < 1u` 竟然是 false。

混合运算的完整规则叫"寻常算术转换"：先做整型提升（`bool`、`char`、`short` 先转 `int`），再按"等级高者胜、同等级有 unsigned 则转 unsigned"统一类型。所以 `unsigned` 与 `int` 相遇时结果是无符号，循环里 `for (int i = n - 1; i >= 0; --i)` 一旦 `n` 是 `size_t`，`i >= 0` 还会因为 `i` 被转成无符号而恒真，直接死循环。写循环下标尽量统一用 `std::size_t` 或统一用 `int`，不要混。

```cpp
#include <climits>
#include <cstddef>
#include <iostream>
#include <limits>

int main() {
    std::cout << sizeof(char) << ' ' << sizeof(short) << ' ' << sizeof(int) << ' '
              << sizeof(long) << ' ' << sizeof(long long) << '\n';
    std::cout << sizeof(float) << ' ' << sizeof(double) << ' ' << sizeof(void*) << ' '
              << sizeof(std::size_t) << '\n';

    std::cout << INT_MAX << ' ' << UINT_MAX << '\n';
    std::cout << std::numeric_limits<int>::min() << '\n';

    unsigned u = 0;
    u -= 1;
    std::cout << u << '\n';

    int a = -7;
    int b = 2;
    std::cout << a / b << ' ' << a % b << ' ' << 1 / 2 << ' ' << 1.0 / 2 << '\n';

    int x = 100000;
    int y = 100000;
    std::cout << 1LL * x * y << '\n';

    int big = INT_MAX;
    std::cout << (big > INT_MAX - 1) << '\n';
}
```

输出：
```text
1 2 4 4 8
4 8 8 8
2147483647 4294967295
-2147483648
4294967295
-3 -1 0 0.5
10000000000
1
```

**注意**：本例第一行第 4 个数是 `sizeof(long)`，它依赖平台——Windows/MSVC/MinGW 上 `long` 是 4 字节（输出 `1 2 4 4 8`），Linux LP64 上是 8 字节（变成 `1 2 4 8 8`）；第二行第 3 个数是 `sizeof(void*)`，32 位环境下是 4。其余各行在所有主流平台一致。考试若要求可移植，不要对 `long` 和指针宽度做假设。

`u -= 1` 从 0 回绕到 `UINT_MAX`，这是定义良好的。`1LL * x * y` 先把第一个乘数变成 `long long`，整个乘法就在 64 位里做，得到 10¹⁰；如果写 `x * y`，10¹⁰ 超过 `int` 上限，是 UB。最后一行比较 `INT_MAX > INT_MAX - 1` 得 1，说明"用减法避免加法溢出"是常用技巧。

❌ 用 `int` 存乘积：

```cpp-fragment
int x = 100000, y = 100000;
int area = x * y;              // 10^10 超出 int，有符号溢出是 UB
long long ok = 1LL * x * y;    // 先把一个操作数变成 64 位
```

### 16.2 浮点数

| 陷阱 | 现象 | 正确做法 |
| --- | --- | --- |
| `0.1 + 0.2 == 0.3` | false（差约 5.5×10⁻¹⁷） | 用 `fabs(a-b) <= eps * scale` |
| 累加 0.1 十次 | 0.99999999999999989 | 同上，或改用整数/定点 |
| `NaN == NaN` | false | 用 `std::isnan` |
| `NaN < x` | false | 比较前先判 NaN |
| `-0.0 == 0.0` | true | 一般无需特殊处理 |
| 输出精度 | 默认 6 位有效数字 | `setprecision` / `fixed` |

二进制浮点无法精确表示 0.1、0.2、1/3 这类十进制小数，误差会在运算中累积。比较浮点数不能直接用 `==`，要判断"相对误差足够小"：`fabs(a-b) <= 1e-9 * max(1.0, max(fabs(a), fabs(b)))`，用 `scale` 是为了让大数也有一致的相对精度。浮点输出要固定格式，否则不同实现的有效数字位数可能不同；`std::setprecision` 在默认格式下是有效数字位数，加上 `std::fixed` 才是小数位数。

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
    std::cout << std::setprecision(6) << (0.0 == -0.0) << '\n';
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

`setprecision(17)` 是能唯一还原 `double` 的最少有效位数，所以能看到 `0.1` 累加十次的真实值。`nearlyEqual` 返回 1，说明用相对误差判断时它"等于 1"。`0.1+0.2` 的真实和是 0.300000000000000044…，与 0.3 不相等。NaN 与任何值比较（包括自己）都是 false，只有 `isnan` 能识别它。最后一行 `-0.0 == 0.0` 为 true。

### 16.3 对象切片与多态

| 写法 | 静态类型 | 实际调用 | 结果 |
| --- | --- | --- | --- |
| `void f(Base b)` 传 `Derived` | `Base` | `Base::name` | 切片，丢失派生部分 |
| `void f(const Base& b)` | `const Base&` | 虚函数动态分派 | `Derived::name` |
| `vector<Base>` 存 `Derived` | `Base` | `Base::name` | 切片 |
| `vector<unique_ptr<Base>>` | `Base*` | 动态分派 | `Derived::name` |

按值传递基类对象会发生**对象切片**（object slicing）：派生类特有的成员被"切掉"，只剩基类子对象，虚函数表也变成基类的，于是多态失效。容器按值存 `Base` 同样会切片，而且连"运行时类型"都没了。正确做法是传引用/指针，容器存 `unique_ptr<Base>` 或 `shared_ptr<Base>`；基类析构函数必须是 `virtual`，否则 `delete Base*` 不会调用派生类析构（UB）。

析构的规则值得单独记：只要一个类可能被继承、且可能通过基类指针删除，析构就必须是 `virtual`。写成 `virtual ~Base() = default;` 即可，不要手写空析构。派生类用 `override` 标注虚函数，编译器会检查签名是否真的覆盖了基类版本；漏写 `const`、参数类型不一致时，`override` 会直接报错，而不是悄悄变成一个新函数。

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

`byValue` 的参数是 `Base` 对象，`Derived` 被切片成 `Base`，虚表指向 `Base`，所以打印 Base；`byRef` 传的是引用，动态类型仍是 `Derived`，虚分派生效。`vector<Base>` 同理切片，`vector<unique_ptr<Base>>` 保存的是指针，`->name()` 走虚函数。最后两行顺带说明 `std::string` 是值语义：`b = a` 是深拷贝，改 `b` 不影响 `a`。

❌ 切片与析构的经典错误：

```cpp-fragment
void print(Base b) { b.name(); }          // 切片，永远 Base
std::vector<Base> v; v.push_back(Derived{}); // 切片
struct Base { ~Base() {} };                // 析构非虚
Base* p = new Derived;
delete p;                                  // UB：只调 Base 析构
```

### 16.4 优先级、结合性与自增

| 优先级（高→低） | 运算符 | 结合性 | 易错点 |
| --- | --- | --- | --- |
| 1 | `::` | 左 | — |
| 2 | `() [] . ->` `++ --`（后缀） | 左 | `*p++` 是 `*(p++)` |
| 3 | `! ~ ++ -- + - * & (type) sizeof`（前缀） | 右 | `*p++` 先取指针再自增 |
| 5 | `* / %` | 左 | — |
| 6 | `+ -` | 左 | — |
| 7 | `<< >>` | 左 | `1 << 2 + 1` 是 `1 << 3` |
| 9 | `< <= > >=` | 左 | — |
| 10 | `== !=` | 左 | `x & 1 == 0` 是 `x & (1==0)` |
| 11 | `&` | 左 | 位与低于等号 |
| 12 | `^` | 左 | — |
| 13 | `\|` | 左 | — |
| 14 | `&&` | 左 | `a && b \|\| c` 是 `(a&&b)\|\|c` |
| 15 | `\|\|` | 左 | — |
| 16 | `?:` | 右 | — |
| 17 | `=` `+=` `-=` 等 | 右 | `if (x = 0)` 恒假 |
| 18 | `,` | 左 | — |

不需要背完整张表，只要记住四组"反直觉"：**位运算低于比较**（`&`、`^`、`|` 都低于 `==`）；**移位低于加法**（`<<` 低于 `+`）；**赋值低于逻辑**（`x = a && b` 是 `x = (a && b)`）；**后缀 `++` 高于解引用**（`*p++` 是 `*(p++)`，先取当前值再移动指针）。拿不准就加括号，括号永远不改变语义，只会让意图明确。

```cpp
#include <iostream>

int main() {
    int a = 1;
    int b = 2;
    int c = 3;
    a = b = c;
    std::cout << a << ' ' << b << ' ' << c << '\n';

    int arr[] = {10, 20, 30};
    int* p = arr;
    std::cout << *p++ << ' ' << *p << '\n';

    int x = 4;
    std::cout << (x & 1) << ' ' << ((x & 1) == 0) << ' ' << (2 + 3 * 4) << '\n';
    std::cout << ((true && false) || true) << ' ' << ((1 + 2) << 3) << '\n';
}
```

输出：
```text
3 3 3
10 20
0 1 14
1 24
```

`a = b = c` 从右往左结合，先把 3 赋给 b，再把 b 的值赋给 a，三者都是 3。`*p++` 先解引用得到 10，然后 `p` 指向下一个元素，所以第二次输出 20。`x & 1` 是 0（4 是偶数），`(x & 1) == 0` 是 1；如果写成 `x & 1 == 0` 就会先算 `1 == 0` 得 0，再 `4 & 0` 得 0，结果"碰巧"也是 0，但语义完全错了——这种"碰巧对"最危险。

### 16.5 容器操作复杂度速查

| 操作 | `vector` | `deque` | `list` | `map`/`set` | `unordered_map` |
| --- | --- | --- | --- | --- | --- |
| 随机访问 `[]` | O(1) | O(1) | 不支持 | 不支持 | 不支持 |
| 查找 key | O(n) | O(n) | O(n) | O(log n) | 平均 O(1) |
| 头部插入 | O(n) | O(1) | O(1) | — | — |
| 尾部插入 | 摊还 O(1) | O(1) | O(1) | — | — |
| 中间插入（已定位） | O(n) | O(n) | O(1) | O(log n) | 平均 O(1) |
| 删除（已定位） | O(n) | O(n) | O(1) | O(log n) | 平均 O(1) |
| 有序遍历 | 需排序 | 需排序 | 需排序 | 天然有序 | 无序 |
| 迭代器类别 | 随机访问 | 随机访问 | 双向 | 双向 | 前向 |

`map` 的"中间插入 O(log n)"指先查找位置再插入的总代价；如果已经持有迭代器，接链本身是 O(1)，但红黑树还要再平衡。`unordered_map` 的"平均 O(1)"依赖哈希质量，最坏（全部冲突）是 O(n)。

复杂度和失效规则要一起记：`vector` 扩容让所有指针、引用、迭代器失效；`deque` 头尾插入让迭代器失效但引用仍有效；`list`/`map` 只让被删元素失效；`unordered_*` 在 rehash 时让迭代器失效、引用仍有效。考试问"哪个操作可能让迭代器失效"，答"可能扩容的插入"通常不会错。

❌ 用 `map[]` 查询、对空容器做 `size()-1`：

```cpp-fragment
std::map<std::string, int> m;
if (m["missing"] > 0) { }   // 插入 {"missing", 0}，size 从 0 变 1

std::vector<int> v;
for (std::size_t i = 0; i <= v.size() - 1; ++i) { }  // 上界回绕成 SIZE_MAX
```

### 16.6 头文件速查

| 头文件 | 主要内容 |
| --- | --- |
| `<vector>` `<deque>` `<list>` `<array>` `<forward_list>` | 序列容器 |
| `<map>` `<set>` | 有序关联容器（`multimap`/`multiset` 也在 `<map>`/`<set>` 里） |
| `<unordered_map>` `<unordered_set>` | 哈希容器 |
| `<stack>` `<queue>` | 适配器（`priority_queue` 在 `<queue>`） |
| `<algorithm>` | `sort`、`find`、`lower_bound`、`unique`、`transform` |
| `<numeric>` | `accumulate`、`partial_sum`、`iota`、`gcd` |
| `<iterator>` | `back_inserter`、`distance`、`advance`、`next` |
| `<bit>` | `popcount`、`countr_zero`、`bit_width`（C++20） |
| `<string>` | `std::string`、`stoi`、`to_string` |
| `<memory>` | `unique_ptr`、`shared_ptr`、`make_unique` |
| `<utility>` | `pair`、`move`、`swap`、`exchange` |
| `<limits>` | `numeric_limits` |
| `<cstdint>` | `int32_t`、`uint64_t` 等定宽整数 |
| `<iomanip>` | `setprecision`、`setw`、`fixed` |
| `<cmath>` | `fabs`、`sqrt`、`isnan` |

记不住时按"容器 / 算法 / 工具"三类归：容器各有一个头文件，算法集中在 `<algorithm>`/`<numeric>`，迭代器工具在 `<iterator>`。`<bits/stdc++.h>` 不是标准头，只有 GCC 有，可移植代码不要用。

### 16.7 高频陷阱总表

| # | 陷阱 | 症状 | 正确做法 |
| --- | --- | --- | --- |
| 1 | `if (x = 0)` | 条件恒假，编译器可能只给警告 | 写 `==`，或把常量放左边 |
| 2 | `x & 1 == 0` | 恒为 0，逻辑完全错 | 写 `(x & 1) == 0` |
| 3 | `1 << 2 + 1` | 得到 8 而不是 5 | 写 `(1 << 2) + 1` |
| 4 | `*p++` | 先解引用再自增，容易多走一格 | 明确写 `*(p++)` 或拆成两步 |
| 5 | `erase` 后 `++it` | 迭代器失效，UB/崩溃 | `it = v.erase(it)` |
| 6 | 缓存 `end()` 后 `push_back` | 扩容导致 `end()` 悬垂 | 每轮重新取 `end()` |
| 7 | `map[k]` 查询 | 悄悄插入默认值，size 变大 | `find` / `contains` / `at` |
| 8 | `v.size() - 1` 空容器 | 无符号回绕成 `SIZE_MAX` | 先判空，或写 `i + 1 < size` |
| 9 | `reserve` 后下标写 | size 仍为 0，越界 UB | `resize` 或 `push_back` |
| 10 | `unique` 不配 `erase` | size 不变，尾部残留旧值 | `v.erase(unique(...), v.end())` |
| 11 | 对 `list` 用 `std::sort` | 编译错误（需随机访问） | 用成员 `l.sort()` |
| 12 | `int` 乘法溢出 | 结果离谱，UB | 至少一个操作数转 `long long` |
| 13 | 浮点 `==` | 偶发不相等 | 相对误差比较 |
| 14 | 基类按值传参 | 切片，多态失效 | 传 `const Base&` 或指针 |
| 15 | 基类析构非虚 | `delete Base*` 不调派生析构 | `virtual ~Base() = default` |
| 16 | `vector<bool>` 取地址 | `&v[0]` 不合法 | 用 `vector<char>` 或 `array<bool,N>` |
| 17 | `long` 当 8 字节 | Windows 上只有 4 字节 | 用 `int64_t` / `long long` |
| 18 | 无符号减到负数 | 回绕成极大值 | 用有符号或先判大小 |
| 19 | `char` 的符号性 | 带符号平台上存 255 会变成 -1 | 存字节用 `unsigned char` |
| 20 | 数组传参退化 | 函数内 `sizeof(arr)` 是指针大小 | 传 `std::array`/`vector`，或另传长度 |
| 21 | 循环里用 `std::endl` | 每次换行都 flush，大量输出变慢 | 用 `'\n'` |

这张表建议对照前三节的例子看：第 5、6、9、10 条在 14.7、15.4 有完整代码，第 2、3、4 条在 15.7、16.4 有反例，第 12、13、14、15 条对应 16.1–16.3。

### 16.8 考试视角

- **看数据范围选复杂度**：n ≤ 20 指数级、n ≤ 5000 O(n²)、n ≤ 10⁶ O(n log n)。先估再写。
- **看要求选容器**：随机访问用 `vector`，有序范围查询用 `map`/`set`，平均 O(1) 查找用 `unordered_*`，双端操作才考虑 `deque`，`list` 只在"已定位的中间插入删除"时才划算。
- **删除三件套**：`remove`/`remove_if` + `erase`，或 C++20 的 `erase_if`；`unique` 只去相邻，先排序。
- **二分三检查**：区间有序、比较器一致、返回值判 `end()`。
- **整数三检查**：会不会溢出、有没有无符号混用、除法/取模的符号。
- **浮点两件事**：不要 `==`，输出固定精度。
- **多态两件事**：传引用/指针，基类析构加 `virtual`。
- **编译检查**：用 `-std=c++20 -Wall -Wextra` 并把警告当错误看；模板报错从第一行读起，先找"哪个实参不满足哪个约束"，再看候选函数列表，最后一行的 `required from here` 才是你代码的位置。

考试写代码时，宁可多写一行括号、多写一个 `long long`、多判一次空，也不要依赖"碰巧正确"。前 15 节的例子都可以直接编译运行，遇到不确定的语义，改一改再跑一遍，比背结论可靠。
