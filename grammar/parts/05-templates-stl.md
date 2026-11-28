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



