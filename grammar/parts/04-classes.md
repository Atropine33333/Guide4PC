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
