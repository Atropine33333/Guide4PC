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
