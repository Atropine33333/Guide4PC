# Section 4: Homework 1

> **Note**: This homework does not depend on the specific project code in `thermal.cpp`. Instead, it focuses on core C++ syntax, common programming errors, software architecture, and performance optimisation. All questions are self‑contained mini‑problems suitable for reinforcing fundamentals. Each question comes with a reference answer (at the end). I recommend trying them on your own before checking the solutions. Even better, feed your answers to any AI for feedback and guidance.

---

## I. Syntax Fundamentals (6 questions)

**1. References vs. Pass‑by‑value**  
Look at the following function, which attempts to increase parameter `a` by 10:
```cpp
void addTen(int a) {
    a += 10;
}
int main() {
    int x = 5;
    addTen(x);
    std::cout << x;   // What is printed?
}
```
(a) What is printed? Why?  
(b) How should the function be changed to actually modify `x`? Provide two different approaches (reference and pointer).

---

**2. Pre‑increment vs. Post‑increment**  
Write the output of the following code segment and explain each line:
```cpp
int a = 3;
int b = ++a;
int c = a++;
int d = ++c + b++;
std::cout << "a=" << a << " b=" << b << " c=" << c << " d=" << d;
```

---

**3. Using `const` correctly**  
Given the following class:
```cpp
class Data {
public:
    int value;
    void setValue(int v) { value = v; }
    int getValue() const { return value; }
};
```
(a) If you have a `const Data` object, can you call `setValue`? Why?  
(b) `getValue` is declared `const`. Can it modify `value` inside? Why?  
(c) Write a function that takes a `const Data&` parameter and safely reads `value`.

---

**4. Ternary operator vs. if‑else**  
Rewrite the following `if-else` using the ternary operator, and explain when the ternary is clearer (and when it is not):
```cpp
int maxVal;
if (x > y) maxVal = x;
else maxVal = y;
```

---

**5. Namespaces and header files**  
(a) What is `std` in `std::cout`, and why do we need to write it?  
(b) What are the potential downsides of writing `using namespace std;`? Give an example of a possible name collision.  
(c) How can you bring only `cout` and `endl` into scope without pulling in the entire `std` namespace?

---

**6. Doxygen‑style comments**  
Write complete Doxygen comments (including `@brief`, `@param`, `@return`) for the following function:
```cpp
double computeAverage(const std::vector<double>& values);
```
The function returns the average of all elements in `values`; if the vector is empty, it returns 0.0.

---

## II. Code Reading and Bug Hunting (6 questions)

**7. Uninitialised variable**  
The following program intends to compute the sum of all elements in an array, but the result is wrong. Find the cause and fix it:
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

**8. Dangling reference / pointer**  
Point out the serious defect in the following code and explain why it is dangerous:
```cpp
int& getElement(std::vector<int>& vec, int index) {
    int temp = vec[index];
    return temp;   // returning a reference to a local variable
}
```

---

**9. Unnecessary copying**  
The following function takes a large `std::vector<double>` as a parameter but does not modify it. What performance issue does it have? How would you improve it?
```cpp
double computeNorm(std::vector<double> v) {
    double sum = 0.0;
    for (double x : v) sum += x * x;
    return std::sqrt(sum);
}
```

---

**10. Array out of bounds**  
The following loop accesses an array. What could go wrong, and how do you fix it?
```cpp
int arr[5] = {1,2,3,4,5};
for (int i = 0; i <= 5; ++i) {
    std::cout << arr[i] << " ";
}
```

---

**11. Infinite loop**  
This loop is intended to print 1 through 10, but it runs forever. Why, and how do you fix it?
```cpp
int i = 1;
while (i <= 10) {
    std::cout << i << " ";
    // missing update to i
}
```

---

**12. Const‑correctness violation**  
The following code fails to compile. Point out the error and give two ways to fix it:
```cpp
class Printer {
public:
    void print(const std::string& msg) { std::cout << msg; }
};
void show(const Printer& p) {
    p.print("Hello");   // compilation error
}
```

---

## III. Architecture and Design (4 questions)

**13. Class responsibility separation**  
A `FileManager` class is responsible for: reading file contents, parsing JSON, writing data to a database, and logging. Analyse the drawbacks of this design and suggest how to split it by responsibility.

---

**14. Deep copy vs. shallow copy**  
Given a `String` class that holds a `char*` member (dynamically allocated C‑style string):
```cpp
class String {
    char* data;
public:
    String(const char* s);
    ~String() { delete[] data; }
};
```
(a) If you do not define a copy constructor or copy assignment operator, what happens when you write `String a = b;`?  
(b) Which special member functions must be defined to manage resources correctly? Briefly explain.

---

**(Hard) 15. RAII and resource management**  
Write a simple `FileHandle` class that opens a file in its constructor and automatically closes it in its destructor (using `fopen`/`fclose`), preventing resource leaks if you forget to call `fclose`. Provide the basic skeleton, **with special attention to copy/move semantics**, and explain how you avoid double‑release of the same resource.

---

**16. Extensible algorithm design**  
You need to design a "sorter" that can sort a `std::vector<int>` using different strategies (e.g., bubble sort, quicksort, insertion sort). Give a design sketch using either classes or function pointers (you don't need to implement the full sorting algorithms, just describe the interface and how strategies can be switched).

---

## IV. Performance and Optimisation (4 questions)

**17. Cache‑friendliness**  
Given a 2D array `int matrix[1024][1024]`, which of the following two code snippets runs faster, and why?
```cpp
// Code A
for (int i = 0; i < 1024; ++i)
    for (int j = 0; j < 1024; ++j)
        sum += matrix[i][j];

// Code B
for (int j = 0; j < 1024; ++j)
    for (int i = 0; i < 1024; ++i)
        sum += matrix[i][j];
```
Explain from the perspectives of memory layout and cache lines.

---

**18. OpenMP parallelisation safety**  
Given the following loop that computes the element‑wise sum of arrays `A` and `B` into `C`, can you safely parallelise it with `#pragma omp parallel for`? Why or why not? If not, how should you modify it?
```cpp
std::vector<double> A(1000), B(1000), C(1000);
for (int i = 0; i < 1000; ++i) {
    C[i] = A[i] + B[i];
}
```

---

**19. Compiler optimisation levels**  
(a) What are the differences between optimisation flags `-O2` and `-O0`? How do they affect debugging?  
(b) Should you use `-O2` while debugging? Why? Which level is typically used in production?

---

**(Hard) 20. Branch prediction optimisation**  
The following loop counts the number of positive elements in an array. If the elements are randomly distributed between positive and negative, the branch misprediction rate is relatively high. Propose an optimisation (e.g., using a branch‑free approach) and explain the principle behind it.
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

# Reference Answers

## I. Syntax Fundamentals

**1.**  
(a) It prints `5`, because the parameter is passed by value – the function modifies a local copy, not the original variable.  
(b) Reference: `void addTen(int& a)`; pointer: `void addTen(int* a)` (call with `addTen(&x)`).

**2.**  
Output: `a=5 b=5 c=5 d=9`  
Step‑by‑step:  
- Initially `a=3`;  
- `++a` → `a=4`, `b=4`;  
- `a++` → `c=4`, then `a=5`;  
- `++c` → `c=5`; `b++` uses the old value of `b` (4), then `b=5`; `d=5+4=9`.  
Final: `a=5, b=5, c=5, d=9`.

**3.**  
(a) No, because `setValue` is not a `const` member; a `const` object can only call `const` members.  
(b) No, a `const` member promises not to modify the object (unless the member is `mutable`).  
(c) Example:
```cpp
void readData(const Data& d) {
    int val = d.getValue();   // OK because getValue is const
}
```

**4.**  
Ternary: `int maxVal = (x > y) ? x : y;`  
Use it for simple two‑branch assignments to keep code compact; use `if-else` when each branch contains multiple statements or side effects.

**5.**  
(a) `std` is the standard library namespace – it prevents name collisions with user‑defined or third‑party symbols.  
(b) Potential downside: name collisions. Example: if you define your own `vector` class, it conflicts with `std::vector`.  
(c) Write `using std::cout; using std::endl;`.

**6.**  
Example:
```cpp
/**
 * @brief Computes the average of a vector of double values.
 * @param values The input vector (read‑only).
 * @return The arithmetic mean of all elements; returns 0.0 if the vector is empty.
 */
double computeAverage(const std::vector<double>& values);
```

---

## II. Bug Hunting

**7.**  
Cause: `sum` is uninitialised – its initial value is indeterminate. Fix: `int sum = 0;`.

**8.**  
Defect: returning a reference to a local variable `temp`. After the function returns, `temp` is destroyed, leaving a dangling reference – accessing it causes undefined behaviour. The correct approach is to return `vec[index]` (which is itself a reference) or return a copy.

**9.**  
Passing by value copies the entire vector, which is expensive. Improvement: change the parameter to `const std::vector<double>& v`.

**10.**  
The condition `i <= 5` accesses `arr[5]`, which is out of bounds. Fix: change to `i < 5`.

**11.**  
The loop body never updates `i`, so the condition remains true forever. Add `++i;` or `i++;` inside the loop.

**12.**  
Error: `p` is a `const Printer&`, but `print` is not a `const` member function, so it cannot be called on a `const` object.  
Two fixes:  
① Make `print` const: `void print(const std::string& msg) const;`  
② Change `show` to take a non‑const reference: `void show(Printer& p)`.

---

## III. Architecture and Design

**13.**  
Drawbacks: violates the Single Responsibility Principle – changes in one area may affect others, the class becomes large, hard to test, and hard to maintain.  
Suggested split: `FileReader` (file I/O), `JsonParser` (JSON parsing), `DatabaseWriter` (database operations), `Logger` (logging). Let them collaborate via composition or dependency injection.

**14.**  
(a) Shallow copy: both objects' `data` pointers point to the same memory block. When they are destroyed, the same memory is freed twice (double free), causing a crash.  
(b) You need a copy constructor, copy assignment operator, destructor (already present), and move constructor / move assignment (C++11) for efficiency.

**15.**  
RAII means acquiring the resource in the constructor and releasing it in the destructor. However, you must handle copy and move carefully to avoid double‑free or dangling pointers.

**Option 1: Manual management (C++11/14 style)** – disable copy, support move:
```cpp
class FileHandle {
    FILE* fp;
public:
    FileHandle(const char* name, const char* mode) 
        : fp(fopen(name, mode)) {}
    ~FileHandle() { if (fp) fclose(fp); }

    // disable copy
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // support move
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

**Option 2: C++17 recommended (Rule of Zero)** – use `std::unique_ptr` with a custom deleter, delegating all resource management to the smart pointer:
```cpp
#include <cstdio>
#include <memory>

class FileHandle {
    std::unique_ptr<FILE, decltype(&fclose)> fp_;
public:
    explicit FileHandle(const char* name, const char* mode)
        : fp_(fopen(name, mode), &fclose) {}

    // Rule of Zero: no need to write destructor, copy, move – unique_ptr handles them
    bool is_open() const { return fp_ != nullptr; }
    FILE* get() const { return fp_.get(); }
    operator FILE*() const { return fp_.get(); }
};
```
This is cleaner, safer, and the preferred modern C++ approach.

**16.**  
Design sketch: define a `Sorter` class that holds a `std::function<void(std::vector<int>&)>` strategy. The strategy can be passed in via the constructor or a `setStrategy` method (e.g., using lambdas or function pointers). Alternatively, use the Strategy pattern with a base `SortStrategy` class and derived implementations; then at runtime, you can switch strategies. Calling `sort(vec)` applies the current strategy.

---

## IV. Performance Optimisation

**17.**  
Code A is faster. In C++, 2D arrays are stored in row‑major order. Code A's inner loop traverses `j`, accessing contiguous memory addresses, which yields high cache hit rates. Code B accesses elements column‑wise, jumping across rows, which is less cache‑efficient.

**18.**  
Yes, it can be safely parallelised because each iteration `i` is independent – there are no data dependencies between different `i`s. Simply add `#pragma omp parallel for` before the loop.

**19.**  
(a) `-O0` disables optimisation, preserving original code structure for easy single‑stepping debugging. `-O2` enables many optimisations (instruction reordering, loop unrolling, etc.) – the program runs faster, but debugging may be harder because variables can be optimised away and the execution order may differ from the source.  
(b) For debugging, use `-O0` or `-Og`; for production, `-O2` (or `-O3`) is typical.

**20.**  
A branch‑free approach:
```cpp
cnt += (arr[i] > 0);   // bool converts to int (0 or 1)
```
Modern compilers often generate a conditional move (`cmov`) instruction for this pattern, eliminating branch misprediction penalties. For SIMD, you could use vectorised comparisons (e.g., `_mm256_cmpgt_epi32`) to process multiple elements at once.

---

> **Tip**: Try to answer all questions on your own before looking at the reference answers. For open‑ended design questions, feel free to propose your own creative solutions.