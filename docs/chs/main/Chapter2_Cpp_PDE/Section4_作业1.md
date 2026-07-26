# 4 作业1

> **说明**：本作业不依赖 `thermal.cpp` 中的具体工程代码，而是围绕 C++ 核心语法、常见编程错误、软件架构及性能优化设计。所有题目均为独立小问题，适合巩固基础。每题均附有参考答案（在末尾），建议先独立完成再对照。更推荐把题目和你的解答发给任意 AI ，让它给你评析和指导。

---

## 一、语法基础巩固（共 6 题）

**1. 引用与值传递**  
观察下面函数，它试图将参数 `a` 的值增加 10：
```cpp
void addTen(int a) {
    a += 10;
}
int main() {
    int x = 5;
    addTen(x);
    std::cout << x;   // 输出多少？
}
```
（a）上述代码输出什么？为什么？  
（b）如何修改函数，使其真正修改 `x` 的值？请写出两种不同方式（引用和指针）。

---

**2. 前置递增与后置递增**  
写出以下程序段的输出，并解释每行结果：
```cpp
int a = 3;
int b = ++a;
int c = a++;
int d = ++c + b++;
std::cout << "a=" << a << " b=" << b << " c=" << c << " d=" << d;
```

---

**3. `const` 的正确使用**  
给定以下类：
```cpp
class Data {
public:
    int value;
    void setValue(int v) { value = v; }
    int getValue() const { return value; }
};
```
（a）若有一个 `const Data` 对象，能否调用 `setValue`？为什么？  
（b）`getValue` 被声明为 `const`，其内部能否修改 `value`？为什么？  
（c）请写出一个接受 `const Data&` 参数的函数，并安全地读取 `value`。

---

**4. 三元运算符与 if‑else**  
将以下 `if-else` 改写为三元运算符，并说明三元运算符的适用场景（何时使用更清晰）：
```cpp
int maxVal;
if (x > y) maxVal = x;
else maxVal = y;
```

---

**5. 命名空间与头文件**  
（a）`std::cout` 中的 `std` 是什么？为什么需要写它？  
（b）写 `using namespace std;` 有什么潜在缺点？给出一个可能造成命名冲突的例子。  
（c）如何只引入 `cout` 和 `endl`，而不引入整个 `std` 命名空间？

---

**6. Doxygen 风格注释**  
为以下函数编写完整的 Doxygen 注释（包括 `@brief`、`@param`、`@return`）：
```cpp
double computeAverage(const std::vector<double>& values);
```
该函数返回 `values` 中所有元素的平均值，若向量为空则返回 0.0。

---

## 二、代码阅读与找 Bug（共 6 题）

**7. 未初始化变量**  
下面的程序打算计算数组中所有元素的和，但结果不正确，请找出原因并修复：
```cpp
int sumArray(int arr[], int size) {
    int sum;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }
    return sum;
}
```

---

**8. 悬垂引用/指针**  
指出下面代码中的严重缺陷，并解释为什么危险：
```cpp
int& getElement(std::vector<int>& vec, int index) {
    int temp = vec[index];
    return temp;   // 返回局部变量的引用
}
```

---

**9. 不必要的拷贝**  
以下函数接受一个大型 `std::vector<double>` 作为参数，但并未修改它，有什么性能问题？如何改进？
```cpp
double computeNorm(std::vector<double> v) {
    double sum = 0.0;
    for (double x : v) sum += x * x;
    return std::sqrt(sum);
}
```

---

**10. 数组越界**  
下列循环访问数组，可能发生什么错误？如何修正？
```cpp
int arr[5] = {1,2,3,4,5};
for (int i = 0; i <= 5; ++i) {
    std::cout << arr[i] << " ";
}
```

---

**11. 死循环**  
此循环本应打印 1 到 10，但运行后却无限循环，为什么？如何修正？
```cpp
int i = 1;
while (i <= 10) {
    std::cout << i << " ";
    // 缺少 i 的更新
}
```

---

**12. const 正确性违规**  
以下代码编译会报错，请指出错误所在并给出两种修复方案：
```cpp
class Printer {
public:
    void print(const std::string& msg) { std::cout << msg; }
};
void show(const Printer& p) {
    p.print("Hello");   // 编译错误
}
```

---

## 三、架构设计与改进（共 4 题）

**13. 类的职责划分**  
有一个 `FileManager` 类，它同时负责：读取文件内容、解析 JSON、将数据写入数据库、记录日志。请分析这种设计的缺点，并提出如何按职责拆分。

---

**14. 深拷贝与浅拷贝**  
给定一个 `String` 类，包含 `char*` 成员（动态分配的 C 风格字符串）：
```cpp
class String {
    char* data;
public:
    String(const char* s);
    ~String() { delete[] data; }
};
```
（a）如果不定义拷贝构造函数和拷贝赋值运算符，使用 `String a = b;` 会发生什么？  
（b）需要定义哪些特殊成员函数来正确管理资源？请简要说明。

---

(难)**15. RAII 与资源管理**  
写一个简单的 `FileHandle` 类，在构造时打开文件，在析构时自动关闭文件（使用 `fopen`/`fclose`），以防止忘记调用 `fclose` 导致资源泄漏。请写出该类的基本框架，**重点考虑拷贝/移动语义**，并说明如何避免资源重复释放。

---

**16. 可扩展的算法设计**  
你需要设计一个“排序器”，能够根据不同的策略（如冒泡排序、快速排序、插入排序）对 `std::vector<int>` 进行排序。请用类或函数指针的方式给出设计思路（不必写出完整排序实现，只需描述接口和如何切换策略）。

---

## 四、性能与优化（共 4 题）

**17. 缓存友好性**  
有一个二维数组 `int matrix[1024][1024]`，以下两段代码哪个执行更快？为什么？
```cpp
// 代码 A
for (int i = 0; i < 1024; ++i)
    for (int j = 0; j < 1024; ++j)
        sum += matrix[i][j];

// 代码 B
for (int j = 0; j < 1024; ++j)
    for (int i = 0; i < 1024; ++i)
        sum += matrix[i][j];
```
请从内存布局和缓存行角度解释。

---

**18. OpenMP 并行化安全**  
给定以下循环，计算数组 `A` 和 `B` 对应元素之和放入 `C`，是否可以直接用 `#pragma omp parallel for` 并行？为什么？如果不行，如何修改？
```cpp
std::vector<double> A(1000), B(1000), C(1000);
for (int i = 0; i < 1000; ++i) {
    C[i] = A[i] + B[i];
}
```

---

**19. 编译优化级别**  
（a）编译选项 `-O2` 和 `-O0` 有什么区别？对程序调试有何影响？  
（b）调试时是否应该使用 `-O2`？为什么？生产环境通常使用哪个级别？

---

(难)**20. 分支预测优化**  
以下循环统计数组中大于 0 的元素个数。如果数组元素正负随机分布，分支预测失误率较高。请提出一种优化方法（例如利用无分支写法），并说明其原理。
```cpp
int countPositive(const int* arr, int n) {
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] > 0) ++cnt;
    }
    return cnt;
}
```

---

# 参考答案

## 一、语法基础

**1.**  
（a）输出 `5`，因为参数是按值传递，函数修改的是局部副本，原变量不变。  
（b）引用方式：`void addTen(int& a)`；指针方式：`void addTen(int* a)`，调用时 `addTen(&x)`。

**2.**  
输出：`a=5 b=5 c=5 d=9`  
推导过程：  
- 初始 `a=3`；  
- `++a` → `a=4`，`b=4`；  
- `a++` → `c=4`，`a=5`；  
- `++c` → `c=5`；`b++` → 取 `b` 旧值 `4`，`b` 变为 `5`；`d=5+4=9`。  
最终 `a=5, b=5, c=5, d=9`。

**3.**  
（a）不能，因为 `setValue` 不是 `const` 成员，`const` 对象只能调用 `const` 成员。  
（b）不能，`const` 成员承诺不修改对象（除非成员声明为 `mutable`）。  
（c）示例：  
```cpp
void readData(const Data& d) {
    int val = d.getValue();  // 合法，因为 getValue 是 const
}
```

**4.**  
三元：`int maxVal = (x > y) ? x : y;`  
适用场景：简单的二分支赋值，使代码紧凑清晰；分支体复杂或有多个语句时应使用 `if-else`。

**5.**  
（a）`std` 是标准库命名空间，用于避免与用户代码或其他库同名符号冲突。  
（b）潜在缺点：可能引入名字冲突，例如自己定义了 `vector` 类，则与 `std::vector` 冲突。  
（c）使用 `using std::cout; using std::endl;`。

**6.**  
示例：
```cpp
/**
 * @brief 计算双精度浮点数向量的平均值。
 * @param values 输入的数值向量（只读）。
 * @return 所有元素的平均值；若向量为空，返回 0.0。
 */
double computeAverage(const std::vector<double>& values);
```

---

## 二、找 Bug

**7.**  
原因：`sum` 未初始化，初始值不确定。修复：`int sum = 0;`。

**8.**  
缺陷：返回局部变量 `temp` 的引用，函数返回后 `temp` 销毁，引用悬垂，访问导致未定义行为。应返回 `vec[index]`（本身是引用）或返回副本值。

**9.**  
按值传递会拷贝整个向量，开销大。改进：改为 `const std::vector<double>& v`。

**10.**  
循环条件 `i <= 5` 会访问 `arr[5]`（越界）。修正：`i < 5`。

**11.**  
循环体内未更新 `i`，导致条件永远成立。添加 `++i;` 或 `i++;`。

**12.**  
错误：`p` 是 `const Printer&`，但 `print` 不是 `const` 成员，不能通过 `const` 对象调用。  
修复方案：  
① 将 `print` 声明为 `void print(const std::string& msg) const`；  
② 将 `show` 参数改为非 `const` 引用 `Printer& p`。

---

## 三、架构设计

**13.**  
缺点：违反单一职责原则，修改一处可能影响其他，类庞大难测试难维护。  
拆分方案：拆分为 `FileReader`（文件读取）、`JsonParser`（JSON 解析）、`DatabaseWriter`（数据库写入）、`Logger`（日志记录），通过组合或依赖注入协作。

**14.**  
（a）浅拷贝：两个对象 `data` 指针指向同一块内存，析构时重复释放（double free）导致程序崩溃。  
（b）需要定义拷贝构造函数、拷贝赋值运算符、析构函数（已有），以及移动构造函数和移动赋值运算符（C++11）以提升性能。

**15.**  
RAII 的核心是资源获取即初始化，析构时释放资源。但必须正确处理拷贝和移动，否则会发生双重释放或悬空指针。  

**方案一：手动管理（C++11/14 风格）**  
禁用拷贝，支持移动：
```cpp
class FileHandle {
    FILE* fp;
public:
    FileHandle(const char* name, const char* mode) 
        : fp(fopen(name, mode)) {}
    ~FileHandle() { if (fp) fclose(fp); }

    // 禁止拷贝
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // 支持移动
    FileHandle(FileHandle&& other) noexcept 
        : fp(other.fp) { other.fp = nullptr; }
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (fp) fclose(fp);
            fp = other.fp;
            other.fp = nullptr;
        }
        return *this;
    }
};
```

**方案二：C++17 推荐做法（Rule of Zero）**  
使用 `std::unique_ptr` 配合自定义删除器，完全委托智能指针管理资源：
```cpp
#include <cstdio>
#include <memory>

class FileHandle {
    std::unique_ptr<FILE, decltype(&fclose)> fp_;
public:
    explicit FileHandle(const char* name, const char* mode)
        : fp_(fopen(name, mode), &fclose) {}

    // Rule of Zero：不需要手写析构、拷贝、移动
    // unique_ptr 自动禁用拷贝，支持移动
    bool is_open() const { return fp_ != nullptr; }
    FILE* get() const { return fp_.get(); }
    operator FILE*() const { return fp_.get(); }
};
```
这种方式更简洁、安全，完全避免资源管理错误，是现代 C++ 的首选。

**16.**  
设计思路：定义一个 `Sorter` 类，内部存储 `std::function<void(std::vector<int>&)>` 策略，通过构造函数或 `setStrategy` 传入具体算法（如 lambda 或函数指针）。也可使用策略模式（基类 `SortStrategy`，派生类实现），运行时多态切换。这样调用 `sort(vec)` 即可使用当前策略。

---

## 四、性能优化

**17.**  
代码 A 更快。C++ 二维数组按行优先存储，代码 A 内层遍历 `j`，访问地址连续，缓存命中率高；代码 B 跳跃访问各行的同一列，缓存利用效率低。

**18.**  
可以直接并行，因为每个 `i` 计算独立，无数据依赖，添加 `#pragma omp parallel for` 即可。

**19.**  
（a）`-O0` 不优化，保留原始代码结构，便于单步调试；`-O2` 进行大量优化（如指令重排、循环展开），运行更快但调试时变量可能被优化掉，单步行为与源码不一致。  
（b）调试时建议用 `-O0` 或 `-Og`；生产环境通常用 `-O2` 或 `-O3`。

**20.**  
无分支写法（编译器通常会生成 `cmov` 指令，避免分支预测失败）：
```cpp
cnt += (arr[i] > 0);   // bool 转 int，自动转为 0 或 1
```
原理：条件表达式不产生分支，而是通过位运算或条件传送指令实现，消除了分支预测失误的惩罚。对于 SIMD 向量化也可考虑使用 `_mm256_cmpgt_epi32` 等指令批量处理。

---

> **提示**：建议先独立作答，再对照参考答案。对于开放设计题，鼓励提出自己的创新方案。