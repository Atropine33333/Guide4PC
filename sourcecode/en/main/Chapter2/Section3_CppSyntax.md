# Section 3: A Beginner's Guide to C++ Syntax (Based on This Project)

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Header file (`#include`) | Tells the compiler which pre‑existing library features you need – like telling the storekeeper which tools you want to borrow |
| Namespace (`std::`) | The "surname" of the standard library, used to distinguish functions that have the same name from different sources |
| Pre‑increment (`++i`) | Increments `i` first, then uses the incremented value as the result of the expression |
| Post‑increment (`i++`) | Uses the old value of `i` as the result of the expression, then increments `i` |
| Row‑major storage | A 2D array stored in memory row by row, contiguously |
| Cache hit rate | The probability that the CPU finds the data it needs in the high‑speed cache – higher means faster programs |
| Ternary operator (`? :`) | A shorthand if‑else that returns one of two values based on a condition, all in one line |
| Reference (`&`) | An alias for a variable – operating on the reference affects the original, avoiding copies |
| Pointer (`*`) | A variable that stores a memory address, allowing indirect access and modification of the original object |
| Dereference (`*ptr`) | Retrieves the object that the pointer points to |
| Doxygen | A tool that automatically generates API documentation from specially formatted comments |
| OpenMP | A set of compiler directives – with a few commented lines, you can distribute loops across multiple CPU cores |
| Compiler optimisation (`-O2`) | The compiler automatically improves the machine instructions of your code, making it run faster without changing the result |
| Pass by reference (`&`) | The function operates directly on the original object, rather than making a copy |
| `const` qualifier | Tells the compiler "this variable / parameter must not be modified" – helps catch bugs early |
| `#pragma` | A compiler directive that tells the compiler to enable certain features or optimisations |

---

## Prelude: A Quick C++ Syntax Primer

> **Note**: This section lists only the C++ syntax essentials you **must** know to understand this project. It is not a complete C++ tutorial. If you are a complete beginner, I suggest you type out the examples in this section first to get a feel for "what C++ code looks like".

### 1. The Simplest C++ Program (Hello World)

```cpp
#include <iostream>   // 1. Header file: bring in input/output functionality

int main() {          // 2. Main function: the program starts executing here
    std::cout << "Hello World!" << std::endl;  // 3. Print a line
    return 0;         // 4. Return 0 to tell the system that the program finished normally
}
```

**Breakdown:**
- `#include`: brings in someone else's toolbox (the standard library). `iostream` is the "input‑output stream" library.
- `int main()`: the program's front door – the operating system calls this first. `int` means this function must eventually return an integer.
- `std::cout`: standard output – think of it as "the screen". `<<` is the arrow that "pushes things onto the screen".
- `return 0`: tells the system "I'm done, everything is fine". A non‑zero value indicates an error.

### 2. Variables and Basic Data Types

Every piece of data used in a program must first be declared with a type. This project mainly uses the following three:

```cpp
int a = 10;           // integer (range roughly ±2 billion)
double b = 3.14159;   // double‑precision floating‑point number (up to ~15 decimal digits)
bool flag = true;     // boolean (only true or false)
```

**Note**: because we are computing temperatures as decimals, `double` appears most frequently in this project.

### 3. Arrays and Loops (Extremely Important)

In C++, a group of data of the same type can be stored in an array. This project uses `vector` (a dynamic array), but the access style is similar:

```cpp
#include <vector>
std::vector<double> scores = {95, 87, 76};  // three scores

// Traversal: using a for loop
for (int i = 0; i < 3; ++i) {
    double value = scores[i];   // access the i‑th element with [i] (starting from 0)
    std::cout << value << std::endl;
}
```

**Understanding `i = 0; i < 3; ++i`:**
1. `int i = 0` – set up a counter starting at 0.
2. `i < 3` – continue as long as `i` is less than 3 (because 0, 1, 2 are exactly three elements; stop when it reaches 3).
3. `++i` – after each loop iteration, add 1 to `i`. Here `++i` simply means "make `i` become `i+1`".

### 4. `if` Conditional

Programs often need to choose between two alternatives based on a condition. This project uses such logic to decide the direction of the wind speed:

```cpp
double velocity = 0.5;
if (velocity >= 0.0) {
    // velocity points right, execute plan A
    std::cout << "going right" << std::endl;
} else {
    // velocity points left, execute plan B
    std::cout << "going left" << std::endl;
}
```

### 5. Defining and Calling Functions (Methods)

Packaging repeated logic into a "processing unit" is what functions are for.

```cpp
// Definition: compute the sum of squares of two numbers
double squareSum(double x, double y) {
    double result = x * x + y * y;
    return result;   // send the result back
}

// Call (inside main):
double ans = squareSum(3.0, 4.0);  // ans becomes 25.0
```

### 6. Classes and Objects

A class is like a blueprint; an object is the actual "thing" built from that blueprint. In this project, `Grid2D` is a blueprint used to create the actual "2D grid" objects.

```cpp
class Dog {
public:                     // public means everyone can access what's inside
    std::string name;       // attribute: name
    int age;                // attribute: age

    void bark() {           // method: make a sound
        std::cout << name << " says Woof!" << std::endl;
    }
};

// How to use it (inside main):
Dog myDog;                  // create a concrete dog from the blueprint
myDog.name = "旺财";        // assign a value to the attribute
myDog.bark();               // make the dog bark
```

---

### 7. References (`&`) – No Copy, Work on the Original

This is the syntax that beginners most often overlook, but it is **heavily used** in this project. Let's start with a wrong example:

```cpp
void badAddOne(int num) {   // no &, this is a copy
    num = num + 1;
}

int main() {
    int a = 5;
    badAddOne(a);
    std::cout << a;   // prints 5, not 6! because we modified the copy
}
```

With `&`, the function receives a **reference** (an alias) to the original, so modifications affect the original:

```cpp
void goodAddOne(int& num) {   // with &, this is a reference to the original
    num = num + 1;
}

int main() {
    int a = 5;
    goodAddOne(a);            // pass a in
    std::cout << a;           // prints 6!
}
```

**Why references are used extensively in this project:**
A `Grid2D` object holds 800 `double` values (about 6.4 KB). If a function parameter does **not** use `&`, every call makes a **copy** of those 6.4 KB. In gradient computation, `runSimulation` is called 1600 times – needless copying would waste a huge amount of time. With `&`, the function operates directly on the original object – zero copy overhead.

```cpp
// Actual code from this project
void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source, ...)
//                      ↑ reference – the function will modify T
//                               ↑ const reference – read‑only, efficient and safe
```

**Const reference (`const T&`)**: `const` tells the compiler "this function guarantees it will not modify this object". If you accidentally write a modification, compilation fails. This gives us both data safety and the efficiency of references.

---

### 8. Pointers (`*`) – Variables That Store Addresses

Pointers are C/C++'s "hardcore" feature. A pointer is itself a variable, but it stores a **memory address** – that is, "where another variable lives".

```cpp
int a = 42;          // a normal variable
int* ptr = &a;       // ptr is a pointer storing the address of a (& is the address‑of operator)
std::cout << ptr;    // prints a number like 0x7ffd1234 (a's memory address)
std::cout << *ptr;   // prints 42 (* is the dereference operator – go to that address and get the value)

*ptr = 100;          // modify a through the pointer
std::cout << a;      // prints 100
```

**References vs. pointers – how this project chooses:**

| Feature | Reference (`&`) | Pointer (`*`) |
|---------|----------------|---------------|
| Can be null? | No, must bind to a valid object | Yes, can be `nullptr` |
| Can be rebound? | No, bound for life | Yes, can point to different objects |
| Access syntax | Use the variable name directly | Need `*` to dereference |
| Where used here | Function parameters (all grid arguments in `stepForward`) | Almost nowhere, except `solver_template` (see below) |

**The only pointer usage in this project:**

```cpp
class InverseOptimizer {
    HeatSolver* solver_template;   // pointer to a solver template
    // ...
};
```

Why a pointer here instead of a reference? Because `solver_template` needs to be passed in at construction time, but a suitable `HeatSolver` object may not exist yet. A pointer can be `nullptr` and assigned later (though in this project it is assigned at construction). More importantly, in OpenMP parallelisation, each thread needs to **copy‑construct** its own local solver from this template – pointers make it easy to pass the template's address around, whereas references are less flexible in such "save address first, copy later" scenarios.

> **For beginners to remember**: for function parameters that pass large objects, use `const T&` (read‑only) or `T&` (modifiable). Only use pointers when you need "rebindable" or "potentially null" semantics. The code in this project follows this rule.

---

### 9. Doxygen‑Style Comments (Why I Write This Way)

If you look carefully at the project's code, you'll notice a special comment format:

```cpp
/**
 * @brief Advance one time step (explicit Euler method)
 * @param T Temperature field (will be modified to the next time level)
 * @param Alpha Thermal diffusivity field (read‑only)
 * @param Source Heat source term (read‑only)
 * @param vx Advection velocity in x direction
 * @param vy Advection velocity in y direction
 */
void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source,
                 double vx, double vy);
```

**This is Doxygen‑style commenting.**

**Why I chose this style:**

1. **Automatic documentation generation**: Doxygen is a tool that scans these `/** ... */` comments and produces nicely formatted HTML or PDF manuals. In a team, new members don't need to read through all the code – they can go straight to the generated docs to see what each function does.

2. **Forces clear expression**: `@brief` forces you to state in one sentence what the function does. `@param` forces you to explain every parameter's purpose. This process itself is a form of "self‑review" – if you can't write a clear `@brief`, the function is probably too complex.

3. **IDE tooltips**: Modern IDEs (like VSCode, CLion) recognise Doxygen comments. When you hover over a function call, the IDE shows the `@brief` and `@param` content – you don't need to jump to the definition to understand usage.

4. **Why `@param` over plain text comments in this project**: Plain `// parameter T is the temperature field` has no structured information that Doxygen tools can parse. `@param T` is a recognised keyword that tools can reliably process. All public interfaces in this project use Doxygen format; internal private functions (like `runSimulation`) use plain comments since they aren't exposed externally.

**Common Doxygen keywords:**

| Keyword | Purpose | Example |
|---------|---------|---------|
| `@brief` | One‑line summary | `@brief Compute mean squared error` |
| `@param` | Explain a parameter | `@param a The first grid` |
| `@return` | Explain the return value | `@return The error value` |
| `@tparam` | Explain a template parameter | Not used in this project |
| `@note` | Additional notes | `@note This function is thread‑safe` |

I recommend beginners start with `@brief` and `@param` – once it becomes a habit, your code quality will visibly improve.

---

### 10. Compiler Directives (`#pragma` and `-O2`)

Lines starting with `#pragma` are **compiler directives** – they are not C++ statements, but instructions telling the compiler "please enable this feature".

```cpp
#pragma omp parallel   // tell the compiler: use OpenMP to create a parallel region
```

`#pragma` does not depend on any header file (though `omp.h` provides function declarations). It is recognised and processed **at compile time** by the compiler. Different compilers support different `#pragma`s; GCC and MSVC have some incompatible ones. But `#pragma omp` is part of the OpenMP standard and is supported by all major compilers (GCC, Clang, MSVC).

**The `-O2` optimisation flag:**

The command you use to compile this project is:
```bash
g++ -std=c++17 -O2 -fopenmp thermal.cpp -o thermal
```

`-O2` stands for Optimisation Level 2. With it enabled, the compiler performs many "equivalent transformations" to improve runtime speed:
- Turns `i++` and `++i` into the same assembly instructions (see Section 3.3)
- Unrolls simple loops (reduces loop‑jump overhead)
- Pre‑computes constants (e.g., `dx * dx` is computed only once at runtime)

`-O2` **does not change program logic** – it only affects performance. For debugging, you can use `-O0` (no optimisation) to make single‑stepping easier; for production, use `-O2` or `-O3`.

---

**The 10 points above** cover 95% of the C++ syntax skeleton used in this project. If you can type out the "Dog" and "squareSum" examples on your own machine and get them working, then when you look at the project code below, you'll feel a sense of déjà vu. Next, we'll formally introduce the architecture of `thermal.cpp`.

---

## 3.1 Header Files and Basic Structure

A C++ program starts with `#include` directives, telling the compiler which external libraries to bring in.

```cpp
#include <algorithm>   // std::clamp, std::max_element, std::min_element
#include <cmath>       // std::exp
#include <fstream>     // std::ofstream (file writing)
#include <iomanip>     // std::setprecision, std::setw (output formatting)
#include <iostream>    // std::cout (print to screen)
#include <string>      // std::string
#include <vector>      // std::vector (dynamic arrays)
#include <omp.h>       // OpenMP parallel support
```

**Why these specific headers:**
Each header provides only the most essential functionality; including them on demand shortens compilation time. `iostream` provides `cout`, `fstream` provides file writing, `vector` gives the dynamic array (used internally by `Grid2D`), and `algorithm` provides `std::clamp` (C++17's parameter‑clipping function) and `std::max_element` (to find the maximum value). `omp.h` is required for OpenMP – without it, `#pragma omp` directives would be ignored, but functions like `omp_get_max_threads` would fail to compile.

**What does the `std::` prefix mean?**
Everything in the C++ standard library lives inside the `std` namespace. `std::cout` means "`cout` inside the `std` namespace". Without `std::`, the compiler would report "identifier not found". You could write `using namespace std;` to drop the prefix, but this project does not – in large projects, `using namespace std` can introduce naming conflicts. Beginners can just follow the pattern and treat it as "the standard library's tag".

```cpp
int main() {
    g_logfile.open("inversion.log");
    // ... actual logic ...
    return 0;
}
```

Execution starts from `main`. `return 0` means normal termination; a non‑zero value signals an error, which the operating system receives.

---

## 3.2 Loop Order and Array Indexing (Why `i` Inside `j`)

The most common loop pattern in the program is nested loops for traversing a 2D grid:

```cpp
for (int j = 0; j < ny; ++j) {
    for (int i = 0; i < nx; ++i) {
        // process point (i, j)
        double val = data[j * nx + i];
    }
}
```

`i` corresponds to the x‑direction (column index), `j` to the y‑direction (row index). The **inner loop is `i`**, and the outer is `j`.

**Why put `i` on the inside:**
`Grid2D` uses row‑major storage – all columns of row 0 are contiguous, then row 1, and so on. When the inner loop traverses `i`, memory addresses increase sequentially: `data[0]`, `data[1]`, `data[2]` … The CPU's hardware prefetcher detects this sequential access pattern and loads subsequent data from main memory into cache in advance. In `data[j * nx + i]`, when `j` is fixed, `j * nx` is constant – the inner loop only computes `+ i`, which is very efficient.

If we reversed the order (`i` outer, `j` inner), memory access would be strided: after `(0,0)`, it jumps to `(1,0)`, skipping an entire row. The prefetcher cannot predict such jumps, and each access might have to read from main memory – which can be 10× or more slower than sequential access (this is called the "cache miss penalty"). For 800 points, the difference isn't obvious, but when scaling to 400,000 points, the gap can turn seconds into minutes.

**Syntax breakdown of `for (int j = 0; j < ny; ++j)`:**
1. `int j = 0` – initialisation, executed once before the loop starts.
2. `j < ny` – condition, checked before each iteration; exits when `false`.
3. `++j` – update, executed after each iteration body.

---

## 3.3 Pre‑increment (`++i`) vs. Post‑increment (`i++`)

The code uses `++i` or `++j` extensively as loop updates:

```cpp
for (int k = 1; k <= sim_steps; ++k)
```

**The difference between them:**

- `++i` (pre‑increment): first adds 1 to `i`, then the incremented value becomes the result of the expression.
- `i++` (post‑increment): first takes the current value of `i` as the result of the expression, then adds 1 to `i`.

```cpp
int a = 5;
int b = ++a;   // a becomes 6, b is assigned 6
int c = 5;
int d = c++;   // c becomes 6, but d is assigned 5 (the old value)
```

**Why this project only uses `++i`:**
In the update part of a `for` loop, we only care about the side effect of incrementing `i` – we don't care about the result of the expression. For built‑in types (`int`, `double`, etc.), `++i` and `i++` generate almost identical machine code. But in C++, if `i` is a complex object (like an iterator), `i++` must return a copy of the old value, which involves a copy construction, whereas `++i` only modifies itself. Although all `i`s in this program are `int`, using `++i` is a good habit that signals "I only want to increment, I don't need the old value".

**About `-O2` optimisation:**
With `-O2` enabled, for `int` types, `++i` and `i++` produce identical assembly. At the assembly level, the "save then increment" vs. "increment then return" distinction is optimised away – the compiler knows the old value won't be used in loop updates and generates just `ADD r, 1`.

**But exams still distinguish them!**
In domestic CS exams (like the Chinese Computer Rank Examination) and postgraduate entrance tests, it is common to ask about the different values of `i++` vs. `++i` in expressions. For example:
```cpp
int i = 3;
int a = i++;  // a = 3, i = 4
int b = ++i;  // b = 5, i = 5
```
Such questions test your understanding of "expression result" vs. "side effect". So even though in real engineering they are equivalent, in exams you must pay close attention to whether it is pre‑ or post‑increment.

---

## 3.4 Ternary Conditional Operator (`? :`)

The upwind differencing in the program uses the ternary operator to perform an `if-else` selection in one line:

```cpp
double dTx = (vx >= 0.0)
                 ? (T_old.at(i, j) - T_old.at(i - 1, j)) / dx
                 : (T_old.at(i + 1, j) - T_old.at(i, j)) / dx;
```

**Syntax:**
```
condition ? expression_if_true : expression_if_false
```

**Why write it this way:**
The upwind decision rule is "if velocity goes right, take the left neighbour; if velocity goes left, take the right neighbour" – it is a binary choice, and the ternary operator fits perfectly. It is more compact than an `if-else`, and in this code, `dTx` needs a value – the ternary returns one directly. With `if-else`, you would need to declare `double dTx;` in advance and assign it in each branch – more lines, and you risk forgetting to assign in one branch (though the compiler would warn, it interrupts your flow).

**Note:** The ternary is not a replacement for all `if-else`. When each branch has multiple statements, stick with `if-else`. This project uses it only in "select‑one‑of‑two‑return‑values" scenarios – exactly where it belongs.

---

## 3.5 OpenMP Parallelisation Directives

OpenMP is a standard for parallel programming via compiler directives (`#pragma`). This program uses OpenMP in three places.

**Use case 1: Parallel region and loop distribution**

```cpp
#pragma omp parallel
{
    // Each thread executes all of this code
    HeatSolver local_solver(...);
    // ...
    #pragma omp for collapse(2) schedule(dynamic)
    for (int j = 0; j < ny; ++j) {
        for (int i = 0; i < nx; ++i) {
            // compute gradient
        }
    }
}
```

- `#pragma omp parallel`: launches a team of threads (equal to the number of CPU cores), each executing the following block.
- `#pragma omp for`: distributes the iterations of the immediately following `for` loop across all threads in the current parallel region. Each thread handles a subset of the `(i, j)` combinations.
- `collapse(2)`: flattens the double loop into 800 independent iterations and then distributes them. Without `collapse(2)`, OpenMP would only parallelise the outer `j` loop (20 iterations) – on 8 cores, only 2.5‑way parallelism, leaving the inner 40 iterations serial. `collapse(2)` makes the parallelism finer‑grained and fully utilises the cores.
- `schedule(dynamic)`: dynamic scheduling – each thread, after finishing its current task, picks the next unprocessed iteration from a queue. If one iteration is interrupted by system scheduling, other threads can share the remaining work, achieving load balancing.

**Use case 2: `#pragma omp parallel for collapse(2)`**

In the RMSprop normalisation, gradient clipping, and parameter update loops, a more concise form is used:

```cpp
#pragma omp parallel for collapse(2)
for (int j = 0; j < ny; ++j) {
    for (int i = 0; i < nx; ++i) {
        double g = grad.at(i, j);
        grad_sq_avg.at(i, j) = rho * grad_sq_avg.at(i, j) + (1.0 - rho) * g * g;
        // ...
    }
}
```

This is equivalent to `#pragma omp parallel` with an inner `#pragma omp for` – a combined shorthand.

**Why these loops are safe to parallelise:**
In these update loops, each `(i, j)` reads and writes only its own `grad.at(i,j)` and `Alpha.at(i,j)`. There is no data dependency between different points. If two threads process `(0,0)` and `(0,1)`, they operate on adjacent but independent memory locations – they won't interfere. This type of workload is called "embarrassingly parallel", and OpenMP can achieve nearly linear speedup.

**Why the gradient‑computation loop cannot simply be `parallel for`:**
Inside the gradient loop, `runSimulation` is called, which internally modifies `T_pred`. If multiple threads shared a single `T_pred`, we would have a data race. Hence we must first use `#pragma omp parallel` to create thread‑local copies, and then inside use `#pragma omp for` to distribute tasks. This is a common "thread‑local data" pattern in OpenMP programming.

---

## 3.6 Pass by Reference (`&`) and `const`

The `&` in function parameters means **pass by reference** – the function operates directly on the original object, not on a copy.

```cpp
void stepForward(Grid2D& T, const Grid2D& Alpha, const Grid2D& Source, ...)
```

- `Grid2D& T`: `T` is a reference – any modification inside `stepForward` directly affects the object passed in by the caller. This is exactly what we need: the function updates the temperature field.
- `const Grid2D& Alpha`: `Alpha` is a const reference. `const` tells the compiler "this function will not modify Alpha" – if we accidentally write `Alpha.at(i,j) = xxx`, compilation fails. A const reference avoids copying (800 doubles ≈ 6.4 KB, but accumulated over many calls it adds up) while providing a read‑only safety guarantee.
- Without `&`, i.e., `Grid2D T`, the function would modify a copy, leaving the caller's original unchanged. Using a pointer `Grid2D* T` would require `(*T).at(i,j)` – clunky syntax. References are both concise and efficient.

**`static` in `static double computeMSE(...)`:**
`static` on a member function means the function belongs to the class itself, not to any instance of the class. You call it with `HeatSolver::computeMSE(a, b)` without creating a `HeatSolver` object. Since this function doesn't depend on any member variables (it is purely computational), it is appropriate to declare it static.

---

## 3.7 Output Formatting (`std::setprecision` / `std::setw`)

The program uses formatting controls when writing CSV files and logs:

```cpp
std::ostringstream oss;
oss << std::scientific << std::setprecision(6);
oss << "Iter " << std::setw(4) << iter
    << "  Loss: " << rep_loss
    << "  α ∈ [" << std::fixed << std::setprecision(6) << amin << ", " << amax << "]";
```

- `std::scientific`: use scientific notation (e.g., `1.495626e+01`).
- `std::fixed`: use fixed‑point notation (e.g., `0.500000`).
- `std::setprecision(6)`: sets the number of digits. In scientific mode it's 6 significant digits; in `fixed` mode it's 6 digits after the decimal point.
- `std::setw(4)`: sets the minimum field width to 4 characters, padding with spaces if needed – this aligns output for readability. `iter` goes from 1 to 200; with `setw(4)`, 1, 10, 100 are aligned in a column.

**Core CSV‑writing code:**
```cpp
f << std::fixed << std::setprecision(8);
for (int j = 0; j < g.ny; ++j) {
    for (int i = 0; i < g.nx; ++i) {
        f << g.at(i, j);
        f << (i < g.nx - 1 ? "," : "\n");
    }
}
```
Each row ends with `\n`, columns separated by commas. The CSV file can be read directly by Excel or Python's `pandas`.

---

## 3.8 Common Questions from Beginners

**What is the difference between `std::endl` and `\n`:**
`std::endl` outputs a newline **and flushes the buffer** – it forces the data to be written immediately. Frequent `endl` can hurt performance. This program uses `std::endl` in `log_write` because logs need to be written immediately (so that if the program crashes, the log is preserved). For CSV output, `\n` is used because we don't need to flush after every line – the operating system's batch writing is more efficient.

**Why does `alpha_true.csv` show only 0.1 and 0.8 but each row has 40 numbers?**
`writeCSV` writes rows for `j` from 0 to 19, each with 40 `i` values. The left half (`i*dx < 0.5`, i.e., `i < 20`) is 0.1, the right half is 0.8. So the first 20 columns are all 0.1, and the last 20 are all 0.8. In a text editor, this CSV appears as a matrix of numbers that directly corresponds to the spatial grid.

**What is the `#` in `#pragma`:**
Lines starting with `#` are **preprocessor directives** – they are processed before the compiler actually compiles the code. `#include` pastes the header file content into the source; `#pragma` tells the compiler "please enable this feature". Unlike normal C++ statements, preprocessor directives do **not** end with a semicolon.

---

## Syntax Self‑Check List for This Section

| Syntax Feature | Where It Appears in the Code | Mastered? |
| :--- | :--- | :--- |
| `#include` and header files | At the top of the file | □ |
| `std::` namespace | Everything with `std::` prefix | □ |
| `class` and `public:` | `Grid2D`, `HeatSolver` | □ |
| `std::vector<double>` | `Grid2D::data` | □ |
| Row‑major nested loops | `HeatSolver::stepForward` | □ |
| Pre‑increment `++i` | All `for` loops | □ |
| Ternary operator `? :` | `dTx`, `dTy` calculations | □ |
| OpenMP `#pragma omp` | Three places in `trainStep` | □ |
| Pass by reference `&` | `stepForward` parameters | □ |
| `const` read‑only protection | `const Grid2D& Alpha` | □ |
| `std::clamp` | After parameter updates | □ |
| Stream formatting `setprecision` | `log_write` and CSV output | □ |

---

## Exercises and Further Practice

Please refer to Section 4.