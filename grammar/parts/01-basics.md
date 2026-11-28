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
