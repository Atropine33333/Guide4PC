# Section 8: Homework 2

> **Note**: This homework is completely independent of the PDE numerical computing context in this tutorial. It focuses on **modern C++ (C++17/20/23) language features, data structures and algorithm implementation, exception safety guarantees, and engineering architecture design patterns**.
>
> This is *not* a typical exam‑cramming sheet – it's quite challenging. I recommend spending some serious time studying before tackling it, or using this assignment as a roadmap for what to learn.
>
> **Structure**: Each question comes with a "Hint" section that provides directional guidance (not the answer). A full reference answer set is provided at the end. I suggest you think through and code up your own solutions first.
>
> It's clear that I'm still far below K‑sensei's level – my initial draft answers were thoroughly criticised by him. "The basic idea is correct, but severely lacking."

---

## I. Language Features (6 questions)

---

**1. `const` and reference collapsing**

Given the following template function:
```cpp
template<typename T>
void forward(T&& arg) {
    // function body
}
```
(a) When `forward` is called, what kind of reference is `T&&`? Explain the reference‑collapsing rules.
(b) If you pass an lvalue `int a = 5;`, what will `T` be deduced as? If you pass an rvalue `forward(10);`, what will `T` be?

> **Hint**:
> - `T&&` in a template deduction context has a special name – it's neither an ordinary rvalue reference nor an lvalue reference.
> - Reference collapsing occurs when template argument deduction interacts with explicit type specification. You can derive the rules with a 2×2 table: when `&` meets `&&`, `&` wins.
> - Try calling `forward` with an lvalue variable and a literal constant, then ask yourself: what does `T` deduce to? If `T` is `int&`, what does `T&&` collapse to?

---

**2. Move semantics and the nature of `std::move`**

(a) What does `std::move` actually do? How does it cast an lvalue to an rvalue reference?
(b) Suppose you have a class `Buffer` containing `std::vector<int> data`. Implement its move constructor and move assignment operator (handling self‑assignment), and explain why the source object should be left in a "valid but unspecified" state after being moved from.

> **Hint**:
> - `std::move` doesn't move anything – it's just a compile‑time cast.
> - Move operations are about "stealing" resources, not copying. After the steal, the source should no longer own the resource, otherwise double‑free occurs.
> - For self‑assignment, checking `this != &other` is standard. In C++14 and later, you can use `std::exchange` to make the code cleaner.
> - Think: what happens if you don't null out the source pointer after moving?

---

**3. Lambda captures and lifetimes**

Explain the potential risk in the code below, and propose two fixes:
```cpp
std::function<int()> makeCounter() {
    int count = 0;
    return [&]() { return ++count; };
}
int main() {
    auto f = makeCounter();
    std::cout << f();
}
```

> **Hint**:
> - What happens to the local variable `count` after `makeCounter` returns? Does the lambda capture by reference or by copy?
> - "Dangling reference" is the key concept here. If the lambda is called after the function returns, it accesses memory that has already been destroyed.
> - Fix 1: capture a copy of `count` – but note the role of the `mutable` keyword.
> - Fix 2: use `std::shared_ptr` to put the counter on the heap, so the lambda shares its lifetime.
> - In C++23, how does `std::move_only_function` differ from `std::function` in this scenario?

---

**4. SFINAE and Concepts**

You need to write a template function `toString` that converts any type to `std::string`, but only for **arithmetic types** (integers, floating‑point). For other types, another overload should be enabled. Provide implementations using C++17's `std::enable_if` and C++20's `concepts`.

> **Hint**:
- SFINAE stands for "Substitution Failure Is Not An Error". The key is to make the overload fail during template argument substitution if the condition is not satisfied, removing it from the overload set.
- When using `std::enable_if_t`, putting it in the return type position is safer than as a default template argument – it avoids overload ambiguities.
- C++20 Concepts provide cleaner syntax. Define an `Arithmetic` concept, then constrain the function template with `requires` or the shorthand syntax.
- Make sure the two overloads are mutually exclusive to avoid ambiguity.

---

**5. Variadic templates and fold expressions**

Implement a function `sum_all` that accepts any number of numeric arguments and returns their sum. Requirements:
- Use C++17 fold expressions.
- Accept at least one argument (reject empty parameter packs).
- The return type should be the common type of all arguments (e.g., mixing `int` and `double` yields `double`).

> **Hint**:
> - Fold expressions come in four forms: unary left fold, unary right fold, binary left fold, binary right fold.
> - If the parameter pack could be empty, a unary fold would fail to compile. You can enforce at least one argument by putting a separate template parameter for the first argument.
> - `std::common_type_t` can compute the common type of the pack, but note that it requires all types to be implicitly convertible to a common type.
> - A binary fold with an initial value, e.g., `(args + ... + 0)`, also works, but here we want at least one argument, so a unary fold is sufficient.

---

**6. User‑defined literals**

C++11 allows user‑defined literal suffixes. Implement a literal suffix `_km` that converts `10_km` to a `double` value in metres (1 km = 1000 m). Write the full operator overload and use `consteval` (C++20) to enforce compile‑time evaluation.

> **Hint**:
> - User‑defined literal suffixes must start with an underscore `_` (suffixes without an underscore are reserved for the standard library).
> - The signature of the literal operator depends on the type of the literal: integer, floating‑point, character, or string literals have different overload forms.
> - `constexpr` allows compile‑time evaluation; `consteval` (C++20) forces compile‑time evaluation – if it can't be evaluated at compile time, compilation fails.
> - Use `constexpr auto meters = 10.0_km;` to do the conversion at compile time.

---

## II. Data Structures and Algorithms (6 questions)

---

**7. Circular Buffer**

Design a class `CircularBuffer<T>` with a fixed capacity (specified at construction) that supports:
- `push_back(const T&)` – if full, overwrite the oldest element.
- `pop_front()` – remove the oldest element; throw on empty.
- `size()` and `capacity()`.
- `operator[](size_t idx)` – random access (by storage order); throw on out‑of‑bounds.

Use `std::vector` as the underlying storage, handle index wrapping correctly, and distinguish between "empty" and "full" states.

> **Hint**:
> - Use `head` and `tail` indices pointing to the front and back. When `head == tail`, the queue could be empty or full – you need an extra variable to distinguish.
> - Three common ways: maintain a `count` variable, sacrifice one slot, or use a `bool` flag. `count` is the most intuitive.
> - `tail = (tail + 1) % capacity` is the key for wrapping. `%` on `size_t` is safe.
> - When pushing into a full buffer, overwrite the oldest – in that case `head` must also move forward.

---

**8. Manual `shared_ptr` implementation**

Implement a simplified `SharedPtr<T>` that includes:
- Constructor (from raw pointer), destructor.
- Copy constructor, copy assignment, move constructor, move assignment.
- `get()`, `use_count()`, `reset()`.
- `operator*` and `operator->`.
- Explicit `operator bool`.

Use a separate reference‑count control block, and ignore thread safety for now.

> **Hint**:
> - The control block should contain a `size_t` counter, allocated separately from the managed object on the heap.
> - Copy construction increments the reference count; destruction decrements it. When the count reaches zero, delete both the object and the control block.
> - Move construction "steals" the pointer and control block, leaving the source empty. After moving, the source's `use_count()` should be 0.
> - `reset()` releases the current resource and then takes ownership of a new one (if any).
> - `operator*` returns a reference; `operator->` returns a pointer. `operator bool` should be `explicit` to prevent accidental conversions.

---

**9. LRU Cache**

Design an `LRUCache<Key, Value>` class with fixed capacity, providing `get(key)` and `put(key, value)` with O(1) time complexity. Use `std::unordered_map` and `std::list`. Ensure both `get` and `put` update the access order (move the most recently used item to the front).

> **Hint**:
> - Core data structure combination: `std::list` stores key‑value pairs (in access order), `std::unordered_map` maps keys to list iterators.
> - In `get`, if the key exists, `splice` the corresponding node to the head of the list, then return the value.
> - In `put`, if the key exists, update the value and move the node to the head; if the key does not exist and the cache is full, remove the tail node (least recently used) from the list and the map, then insert the new node at the head.
> - `std::list::splice` is O(1) and does not copy or move elements.
> - Capacity 0 should be rejected (throw in the constructor).

---

**10. Thread‑safe blocking queue**

Design a template class `BlockingQueue<T>` that supports:
- `push(T value)` – enqueue; if full, block until space is available.
- `pop(T& out)` – dequeue; if empty, block until an item is available.
- `try_pop()` – returns `std::optional<T>` (non‑blocking).
- `stop()` – wake up all waiting threads and reject new operations.

Use `std::mutex`, `std::condition_variable`, and `std::atomic<bool>`.

> **Hint**:
> - Two condition variables are needed: `not_empty` (wakes consumers) and `not_full` (wakes producers).
> - Use `std::unique_lock` with condition variables; remember that the wait predicate should check the stop flag.
> - `stop()` sets `stopped = true` and then `notify_all()` to wake all waiting threads.
> - `try_pop` uses `std::lock_guard` to lock, checks non‑empty, then pops; otherwise returns `std::nullopt`.
> - Consider: why does `std::atomic<bool>` still need to cooperate with a mutex? What happens if you check the stop flag outside the mutex?

---

**11. Simple `std::function` type erasure**

Write a class `Function<Signature>` that can store any callable object (function pointer, lambda, function object), and overload `operator()` to invoke it. **Do not use `std::function`** – implement type erasure yourself (via templates + abstract base class + `std::unique_ptr`).

Example:
```cpp
Function<int(double, double)> f = [](double a, double b) { return (int)(a + b); };
int result = f(1.5, 2.5);  // result = 4
```

> **Hint**:
> - The core of type erasure: use a non‑template base class to define the interface (virtual `invoke`), and a template derived class to store the concrete callable.
> - A base‑class pointer (`std::unique_ptr<CallableBase>`) erases the concrete type; `operator()` calls `invoke` via the virtual function.
> - Copy construction requires a `clone()` virtual function to copy the derived object, because `unique_ptr` cannot be copied.
> - Move construction can be `= default` because `unique_ptr` supports move.
> - If the callable is empty, `operator()` should throw `std::bad_function_call`.

---

**12. Binary tree inorder iterator**

Given a binary tree node:
```cpp
struct Node { int value; Node* left; Node* right; };
```
Write a class `InorderIterator` that takes a root pointer and implements `operator++`, `operator*`, `operator!=`. Then wrap it in an `InorderRange` class that supports range‑based `for` loops:
```cpp
for (int val : InorderRange(root)) { std::cout << val << " "; }
```
Use a stack to simulate recursion, with O(1) amortised time per increment.

> **Hint**:
> - Inorder traversal: left subtree → node → right subtree. Use a stack to simulate recursion: go left as far as possible, pushing nodes.
> - The iterator's `current` points to the current node. `operator++` handles the right subtree: push all left nodes of the right subtree, then pop the next node from the stack.
> - `begin()` should construct an iterator and advance to the first node (the leftmost). `end()` should return an iterator with `current == nullptr`.
> - The iterator type should satisfy `std::input_iterator` – define `iterator_category` and other aliases.
> - `operator*` should return `const int&` to enforce read‑only access.

---

## III. Exception Safety and Resource Management (4 questions)

---

**13. RAII file lock**

Assume the system provides two C functions: `lock_file(const char* path)` and `unlock_file(const char* path)` (return 0 on success, -1 on failure). Design an RAII class `FileLock` that locks in the constructor, unlocks in the destructor, and throws `std::runtime_error` if locking fails. Also disable copy and enable move semantics.

> **Hint**:
> - RAII core: acquire the resource in the constructor, release it in the destructor.
> - Throwing on lock failure ensures that no "unlocked" `FileLock` object can be constructed.
> - Disable copy because a file lock cannot be copied (two objects holding the same lock would lead to double release).
> - Move semantics allow transferring ownership: after moving, the source's `locked` flag is set to `false`, so its destructor doesn't release the lock.
> - Move assignment must release the current lock before taking over the source's lock.

---

**14. Exception‑safe copy assignment**

For a `String` class (holding a dynamically allocated `char*` C‑style string), write an **exception‑safe** copy assignment operator using the **copy‑and‑swap** idiom, providing the strong exception safety guarantee. Explain why this guarantees that the original object's state remains unchanged if the assignment fails.

> **Hint**:
> - "Strong exception safety guarantee" means that if an operation throws, the program state remains unchanged.
> - Copy‑and‑swap works by first creating a temporary copy via copy construction (which may throw), then swapping the temporary with `*this` using a non‑throwing `swap`.
> - The key point: if the copy construction fails, `this` is completely unaffected (because we haven't started modifying `this` yet).
> - `swap` only exchanges pointers and lengths, no memory allocation – so it can be marked `noexcept`.
> - Move assignment can be implemented directly (no copy‑and‑swap needed) because moves are usually `noexcept`.

---

**15. Proper use of `noexcept`**

(a) In what situations should you mark a function `noexcept`? When should you not?
(b) Move constructors should generally be `noexcept` – why? What performance impact does it have on `std::vector` reallocation if a move constructor is not `noexcept`?

> **Hint**:
> - `noexcept` is not just an optimisation hint; it affects the exception‑safety policy of containers.
> - Destructors, move operations, and `swap` should usually be `noexcept`.
> - If a function may throw (e.g., memory allocation, file I/O), marking it `noexcept` will cause the program to call `std::terminate` when an exception does occur.
> - During `std::vector` reallocation, if the element's move constructor is `noexcept`, the vector will use move; otherwise it uses copy – this is the key source of performance differences.

---

**16. Object Pool**

Design a simple `ObjectPool<T>` that pre‑allocates a contiguous block of memory (e.g., using `std::vector` of raw bytes) and provides `allocate(Args&&...)` and `deallocate(T*)` to reuse objects. Requirements:
- Avoid frequent `new`/`delete`.
- Support arbitrary type `T` (use placement new and `std::construct_at`, C++20).
- Implement automatic expansion (double the size when full).
- (Optional bonus) Use `std::shared_mutex` for thread safety.

> **Hint**:
> - The pool allocates a large chunk of memory upfront and manages free blocks with a free list.
> - `allocate` takes a block from the free list, constructs an object using placement new – C++20's `std::construct_at` is recommended.
> - `deallocate` calls the destructor (`std::destroy_at`, C++17) and returns the block to the free list.
> - On expansion, allocate a larger `vector` and add the new blocks to the free list. Existing objects remain unaffected (they stay in the old memory).
> - For thread safety, use `std::shared_mutex`: `allocate`/`deallocate` use exclusive locks; read‑only operations like `size()` use shared locks.

---

## IV. Architecture and Design Patterns (4 questions)

---

**17. Strategy pattern (swappable sorting algorithms)**

Design a `SortEngine` class that holds a sorting strategy interface and can switch sorting algorithms at runtime (e.g., bubble sort, quick sort, insertion sort). Requirements:
- Define an `ISorter` abstract class with a pure virtual `sort(std::vector<int>&)`.
- Implement at least two concrete strategy classes.
- `SortEngine` provides `setStrategy(std::unique_ptr<ISorter>)` and `execute(std::vector<int>&)`.

> **Hint**:
> - The Strategy pattern encapsulates algorithms in separate classes, making them interchangeable.
> - Define an abstract interface `ISorter`; concrete strategies (`QuickSort`, `BubbleSort`) implement it.
> - `SortEngine` holds a `std::unique_ptr<ISorter>` and switches via `setStrategy`.
> - `execute` calls the current strategy's `sort` method.
> - Consider: if a strategy has no state, could you use function pointers or `std::function` instead?

---

**18. Observer pattern (event system)**

Design a simple event system that supports:
- `EventDispatcher` singleton providing `register(eventType, callback)` and `dispatch(eventType, data)`.
- Multiple listeners can register for the same event type; callbacks receive `const std::string& data`.
- Use `std::unordered_map` mapping event types (`std::string`) to callback lists (`std::vector<std::function<void(const std::string&)>>`).
- **Key challenge**: handle the case where callbacks may add or remove listeners during dispatch (hint: copy the callback list before dispatching to avoid iterator invalidation).

> **Hint**:
> - A singleton can be implemented with the Meyers singleton (thread‑safe since C++11).
> - `register` needs thread safety (if multi‑threaded). Use `std::mutex` to protect `listeners`.
> - In `dispatch`, copy the callback list and iterate over the copy. This way, even if a callback modifies `listeners`, the ongoing traversal is unaffected.
> - If you iterate directly over the list and a callback registers a new listener, the `vector` may reallocate, invalidating iterators.
> - Consider: how would you support removing listeners? Use `std::weak_ptr`‑wrapped callbacks or return a token for unregistration.

---

**19. Dependency injection + factory pattern**

You are developing a game that needs to create different kinds of enemies (`Goblin`, `Orc`, `Dragon`). All enemies implement the `IEnemy` interface (with `attack()`).
- Design an `EnemyFactory` that, given a string parameter (e.g., `"goblin"`), returns the corresponding `std::unique_ptr<IEnemy>`. Use `std::unordered_map` internally to register creators.
- Design a `Game` class that receives an `EnemyFactory` reference via **constructor injection** and uses the factory to spawn enemies at runtime.
- Explain how this design benefits unit testing.

> **Hint**:
> - Factory pattern separates object creation from usage. `EnemyFactory` maintains a map from type name to creation function (lambda).
> - Dependency injection (constructor injection) passes the `Game`'s dependency (`EnemyFactory`) from the outside, rather than creating it inside `Game`.
> - Benefit: `Game` does not directly depend on concrete enemy classes (`Goblin`, `Orc`), only on the factory interface.
> - In unit tests, you can inject a `MockFactory` that returns fake enemies or records call counts, verifying `Game`'s logic.

---

**20. Compile‑time polymorphism (CRTP) vs. runtime polymorphism (virtual functions)**

In C++, you can pass strategies via template parameters (compile‑time polymorphism) or via base‑class pointers (runtime polymorphism).
(a) Describe the scenarios and performance differences for each approach.
(b) If you need to implement a container whose sorting strategy is determined at compile time and never changes, which approach would you choose? If the sorting strategy needs to be selected at runtime via a user configuration file, which approach would you choose? Provide code skeletons for both designs.

> **Hint**:
> - Compile‑time polymorphism (templates/CRTP) determines the type at compile time – no virtual call overhead, can be inlined, but cannot change strategy at runtime.
> - Runtime polymorphism (virtual functions) has vtable indirection overhead and may prevent inlining, but allows dynamic switching.
> - Compile‑time fixed strategy: pass the sorting strategy as a template parameter – the container holds an object of that type and calls `sort()` statically.
> - Runtime‑determined strategy: use a base‑class pointer to the concrete strategy object and call `sort()` virtually. The strategy object can be created from a configuration string.
> - Decision: if the strategy is fixed and performance‑critical (e.g., in a hot loop), choose compile‑time polymorphism; if it needs user configuration or dynamic plugin loading, choose runtime polymorphism.

---

---

# Reference Answers

> **Note**: The reference answers provide complete ideas and core code skeletons. For some questions, compilable core skeletons are given. I recommend attempting your own solutions first before referring to these.

---

## I. Language Features Deep Dive

### Question 1

**(a)** `T&&` in template argument deduction is a **forwarding reference** (also called a universal reference). It is not an ordinary rvalue reference – it adapts its type depending on whether the argument is an lvalue or an rvalue.

Reference collapsing rules:
- `T& &` → `T&`
- `T& &&` → `T&`
- `T&& &` → `T&`
- `T&& &&` → `T&&`

When an lvalue is passed, `T` is deduced as an lvalue reference type, and `T&&` collapses to `T&`; when an rvalue is passed, `T` is deduced as a non‑reference type, and `T&&` remains `T&&`.

**(b)** Passing an lvalue `int a = 5;` deduces `T` as `int&`, and `T&&` collapses to `int&` (lvalue reference). Passing an rvalue `10` deduces `T` as `int`, so `T&&` is `int&&` (rvalue reference).

---

### Question 2

**(a)** `std::move` is essentially a compile‑time cast – it converts an lvalue to an rvalue reference. Its core implementation is roughly `static_cast<T&&>(arg)`. It does not generate any runtime code and does not perform any moving itself. The actual move happens when the move constructor or move assignment operator is called.

**(b)**
```cpp
class Buffer {
    std::vector<int> data;
public:
    Buffer() = default;
    
    Buffer(Buffer&& other) noexcept 
        : data(std::exchange(other.data, {})) {}
    
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            data = std::exchange(other.data, {});
        }
        return *this;
    }
};
```

After a move, the source object should be left in a "valid but unspecified" state. For `std::vector`, this means an empty vector. This ensures the source doesn't double‑free resources on destruction, while still being safely destructible or reassignable. `std::exchange` (C++14) sets `other.data` to empty and returns the old value in one step.

---

### Question 3

**Risk analysis**: `makeCounter` returns a lambda that captures the local variable `count` by reference. When the function returns, `count` is destroyed, and the lambda holds a dangling reference. Calling `f()` accesses invalid memory, causing undefined behaviour.

**Fix 1** (value capture):
```cpp
std::function<int()> makeCounter() {
    int count = 0;
    return [count]() mutable { return ++count; };
}
```
`[count]` captures a copy of `count`; `mutable` allows modification inside the lambda.

**Fix 2** (dynamic allocation with `shared_ptr`):
```cpp
std::function<int()> makeCounter() {
    auto count = std::make_shared<int>(0);
    return [count]() { return ++(*count); };
}
```
`count` lives on the heap; the lambda holds a `shared_ptr` that keeps it alive.

**Note (C++23)**: If copy semantics are not needed, you can use `std::move_only_function` or simply return the lambda directly (exact type, no type erasure).

---

### Question 4

**C++17 version (return‑type SFINAE)**:
```cpp
template<typename T>
std::enable_if_t<std::is_arithmetic_v<T>, std::string>
toString(T val) { 
    return std::to_string(val); 
}

template<typename T>
std::enable_if_t<!std::is_arithmetic_v<T>, std::string>
toString(const T& val) { 
    return std::string(val);  // assuming T is convertible to std::string
}
```

**C++20 Concepts version**:
```cpp
template<typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template<Arithmetic T>
std::string toString(T val) { 
    return std::to_string(val); 
}

template<typename T>
    requires (!Arithmetic<T>)
std::string toString(const T& val) { 
    return std::string(val);
}
```

**Why not use `enable_if` in a default template parameter**: The `template<typename T, typename = std::enable_if_t<...>>` approach can cause ambiguity during overload resolution if both overloads are viable. Putting it in the return type is safer.

---

### Question 5

```cpp
#include <type_traits>

template<typename T, typename... Rest>
std::common_type_t<T, Rest...> sum_all(T first, Rest... rest) {
    return (first + ... + rest);  // binary left fold, at least one argument
}
```

**Explanation**:
- `(first + ... + rest)` is a binary left fold, expanding to `((first + rest1) + rest2) + ...`
- `std::common_type_t<T, Rest...>` computes the common type of all arguments – for example, mixing `int` and `double` yields `double`.
- The template parameter list includes at least one type `T`, forcing at least one argument.

---

### Question 6

```cpp
consteval long double operator"" _km(long double km) {
    return km * 1000.0L;
}

// Usage (compile‑time evaluation)
constexpr auto meters = 10.0_km;  // 10000.0L
```

**Explanation**:
- The literal suffix must start with `_` (suffixes without underscores are reserved for the standard library).
- `consteval` (C++20) forces compile‑time evaluation – compilation fails if it cannot be evaluated at compile time.
- `constexpr` (C++11) allows compile‑time or runtime evaluation.

---

## II. Data Structures and Algorithms

### Question 7

```cpp
#include <vector>
#include <stdexcept>

template<typename T>
class CircularBuffer {
    std::vector<T> buf;
    size_t head = 0;   // position of oldest element
    size_t tail = 0;   // next write position
    size_t count = 0;  // current number of elements (distinguishes empty/full)
    
public:
    explicit CircularBuffer(size_t cap) : buf(cap) {}
    
    void push_back(const T& value) {
        if (buf.empty()) throw std::runtime_error("Buffer has zero capacity");
        
        buf[tail] = value;
        tail = (tail + 1) % buf.size();
        
        if (count == buf.size()) {
            head = tail;   // overwrite oldest, head follows
        } else {
            ++count;
        }
    }
    
    void pop_front() {
        if (count == 0) throw std::runtime_error("Buffer is empty");
        head = (head + 1) % buf.size();
        --count;
    }
    
    [[nodiscard]] size_t size() const noexcept { return count; }
    [[nodiscard]] size_t capacity() const noexcept { return buf.size(); }
    
    T& operator[](size_t idx) {
        if (idx >= count) throw std::out_of_range("Index out of range");
        return buf[(head + idx) % buf.size()];
    }
    
    const T& operator[](size_t idx) const {
        if (idx >= count) throw std::out_of_range("Index out of range");
        return buf[(head + idx) % buf.size()];
    }
};
```

---

### Question 8

```cpp
#include <utility>

template<typename T>
class SharedPtr {
    T* ptr = nullptr;
    size_t* ref_count = nullptr;
    
    void release() noexcept {
        if (ref_count) {
            if (--(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
        }
    }
    
public:
    explicit SharedPtr(T* p = nullptr) 
        : ptr(p), ref_count(p ? new size_t(1) : nullptr) {}
    
    ~SharedPtr() { release(); }
    
    SharedPtr(const SharedPtr& other) 
        : ptr(other.ptr), ref_count(other.ref_count) {
        if (ref_count) ++(*ref_count);
    }
    
    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ref_count) ++(*ref_count);
        }
        return *this;
    }
    
    SharedPtr(SharedPtr&& other) noexcept 
        : ptr(std::exchange(other.ptr, nullptr))
        , ref_count(std::exchange(other.ref_count, nullptr)) {}
    
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr = std::exchange(other.ptr, nullptr);
            ref_count = std::exchange(other.ref_count, nullptr);
        }
        return *this;
    }
    
    T* get() const noexcept { return ptr; }
    size_t use_count() const noexcept { return ref_count ? *ref_count : 0; }
    
    void reset(T* p = nullptr) {
        release();
        ptr = p;
        ref_count = p ? new size_t(1) : nullptr;
    }
    
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    explicit operator bool() const noexcept { return ptr != nullptr; }
};
```

---

### Question 9

```cpp
#include <list>
#include <unordered_map>
#include <stdexcept>

template<typename Key, typename Value>
class LRUCache {
    using ListIter = typename std::list<std::pair<Key, Value>>::iterator;
    
    size_t cap;
    std::list<std::pair<Key, Value>> items;      // front = newest, back = oldest
    std::unordered_map<Key, ListIter> map;
    
public:
    explicit LRUCache(size_t capacity) : cap(capacity) {
        if (cap == 0) throw std::invalid_argument("Capacity must be > 0");
    }
    
    Value* get(const Key& key) {
        auto it = map.find(key);
        if (it == map.end()) return nullptr;
        
        // move to front
        items.splice(items.begin(), items, it->second);
        return &it->second->second;
    }
    
    void put(const Key& key, const Value& value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }
        
        if (items.size() >= cap) {
            map.erase(items.back().first);
            items.pop_back();
        }
        
        items.emplace_front(key, value);
        map[key] = items.begin();
    }
};
```

---

### Question 10

```cpp
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <optional>

template<typename T>
class BlockingQueue {
    std::queue<T> queue;
    mutable std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
    size_t max_size;
    std::atomic<bool> stopped{false};
    
public:
    explicit BlockingQueue(size_t capacity) : max_size(capacity) {}
    
    void push(T value) {
        std::unique_lock lock(mtx);
        not_full.wait(lock, [this] { 
            return queue.size() < max_size || stopped.load(); 
        });
        if (stopped.load()) throw std::runtime_error("Queue is stopped");
        queue.push(std::move(value));
        not_empty.notify_one();
    }
    
    void pop(T& out) {
        std::unique_lock lock(mtx);
        not_empty.wait(lock, [this] { 
            return !queue.empty() || stopped.load(); 
        });
        if (queue.empty()) throw std::runtime_error("Queue is stopped and empty");
        out = std::move(queue.front());
        queue.pop();
        not_full.notify_one();
    }
    
    std::optional<T> try_pop() {
        std::unique_lock lock(mtx);
        if (queue.empty()) return std::nullopt;
        T val = std::move(queue.front());
        queue.pop();
        not_full.notify_one();
        return val;
    }
    
    bool try_push(T value) {
        std::unique_lock lock(mtx);
        if (queue.size() >= max_size || stopped.load()) return false;
        queue.push(std::move(value));
        not_empty.notify_one();
        return true;
    }
    
    void stop() {
        stopped.store(true);
        not_empty.notify_all();
        not_full.notify_all();
    }
    
    [[nodiscard]] bool empty() const {
        std::lock_guard lock(mtx);
        return queue.empty();
    }
    
    [[nodiscard]] size_t size() const {
        std::lock_guard lock(mtx);
        return queue.size();
    }
};
```

---

### Question 11

```cpp
#include <memory>
#include <utility>
#include <functional>

template<typename>
class Function;

template<typename R, typename... Args>
class Function<R(Args...)> {
    struct CallableBase {
        virtual ~CallableBase() = default;
        virtual R invoke(Args... args) const = 0;
        virtual std::unique_ptr<CallableBase> clone() const = 0;
    };

    template<typename F>
    struct Callable : CallableBase {
        mutable F func;  

        explicit Callable(F f) : func(std::move(f)) {}

        R invoke(Args... args) const override {
            return invoke_impl(std::move(args)...);
        }

        template<typename... CallArgs>
        R invoke_impl(CallArgs&&... args) const {
            return func(std::forward<CallArgs>(args)...);
        }

        std::unique_ptr<CallableBase> clone() const override {
            return std::make_unique<Callable>(func);
        }
    };

    std::unique_ptr<CallableBase> callable;

public:
    Function() = default;

    template<typename F>
    Function(F f) : callable(std::make_unique<Callable<F>>(std::move(f))) {}

    Function(const Function& other)
        : callable(other.callable ? other.callable->clone() : nullptr) {}

    Function& operator=(const Function& other) {
        if (this != &other) {
            callable = other.callable ? other.callable->clone() : nullptr;
        }
        return *this;
    }

    Function(Function&&) noexcept = default;
    Function& operator=(Function&&) noexcept = default;

    R operator()(Args... args) const {
        if (!callable) throw std::bad_function_call();
        return callable->invoke(std::move(args)...);
    }

    explicit operator bool() const noexcept { return callable != nullptr; }
};
```

Usage examples:

```cpp
// 1. Function pointer
Function<int(double, double)> f1 = [](double a, double b) { return int(a + b); };
int r1 = f1(1.5, 2.5);  // 4

// 2. Lambda with mutable state
int offset = 10;
Function<int(int)> f2 = [offset](int x) mutable { return x + offset; };
int r2 = f2(5);  // 15

// 3. Lvalue reference parameter (fixed version correctly forwards)
Function<void(std::string&)> f3 = [](std::string& s) { s += "!"; };
std::string msg = "hello";
f3(msg);  // msg == "hello!"
```

---

### Question 12

```cpp
#include <stack>
#include <iterator>

struct Node {
    int value;
    Node* left = nullptr;
    Node* right = nullptr;
};

class InorderIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;
    
private:
    std::stack<Node*> stk;
    Node* current = nullptr;
    
    void pushLeft(Node* node) {
        while (node) {
            stk.push(node);
            node = node->left;
        }
    }
    
public:
    explicit InorderIterator(Node* root = nullptr) {
        pushLeft(root);
        if (!stk.empty()) {
            current = stk.top();
            stk.pop();
        }
    }
    
    InorderIterator& operator++() {
        pushLeft(current->right);
        if (!stk.empty()) {
            current = stk.top();
            stk.pop();
        } else {
            current = nullptr;
        }
        return *this;
    }
    
    InorderIterator operator++(int) {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }
    
    reference operator*() const { return current->value; }
    pointer operator->() const { return &current->value; }
    
    bool operator!=(const InorderIterator& other) const {
        return current != other.current;
    }
    bool operator==(const InorderIterator& other) const {
        return current == other.current;
    }
};

class InorderRange {
    Node* root;
public:
    explicit InorderRange(Node* r) : root(r) {}
    InorderIterator begin() const { return InorderIterator(root); }
    InorderIterator end() const { return InorderIterator(); }
};
```

---

## III. Exception Safety and Resource Management

### Question 13

```cpp
#include <string>
#include <stdexcept>
#include <utility>

extern "C" {
    int lock_file(const char* path);
    int unlock_file(const char* path);
}

class FileLock {
    std::string path;
    bool locked = false;
    
public:
    explicit FileLock(const char* p) : path(p) {
        if (lock_file(path.c_str()) != 0) {
            throw std::runtime_error("Failed to lock file: " + path);
        }
        locked = true;
    }
    
    ~FileLock() {
        if (locked) {
            unlock_file(path.c_str());
        }
    }
    
    FileLock(const FileLock&) = delete;
    FileLock& operator=(const FileLock&) = delete;
    
    FileLock(FileLock&& other) noexcept
        : path(std::move(other.path))
        , locked(std::exchange(other.locked, false)) {}
    
    FileLock& operator=(FileLock&& other) noexcept {
        if (this != &other) {
            if (locked) {
                unlock_file(path.c_str());
            }
            path = std::move(other.path);
            locked = std::exchange(other.locked, false);
        }
        return *this;
    }
    
    [[nodiscard]] bool is_locked() const noexcept { return locked; }
};
```

---

### Question 14

```cpp
#include <cstring>
#include <algorithm>

class String {
    char* data = nullptr;
    size_t len = 0;
    
public:
    String(const char* str = "") {
        len = std::strlen(str);
        data = new char[len + 1];
        std::memcpy(data, str, len + 1);
    }
    
    String(const String& other) : String(other.data) {}
    
    String(String&& other) noexcept 
        : data(std::exchange(other.data, nullptr))
        , len(std::exchange(other.len, 0)) {}
    
    ~String() { delete[] data; }
    
    String& operator=(const String& other) {
        String temp(other);   // may throw, but `this` remains unchanged
        swap(temp);           // noexcept
        return *this;
    }
    
    String& operator=(String&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = std::exchange(other.data, nullptr);
            len = std::exchange(other.len, 0);
        }
        return *this;
    }
    
    void swap(String& other) noexcept {
        using std::swap;
        swap(data, other.data);
        swap(len, other.len);
    }
    
    friend void swap(String& a, String& b) noexcept { a.swap(b); }
};
```

**Why copy‑and‑swap provides the strong exception safety guarantee**:
1. `String temp(other)` constructs a temporary copy on the stack. If it throws (e.g., allocation failure), `this` is completely unaffected.
2. `swap(temp)` only exchanges pointers and lengths, no memory allocation – marked `noexcept`, so it cannot fail.
3. Therefore, the operation either succeeds completely (after the swap) or fails completely (exception thrown in step 1, state unchanged).

---

### Question 15

**(a)** Mark `noexcept` in these situations:
- Destructors (usually `noexcept` by default).
- Move constructors and move assignment operators.
- `swap` functions.
- Functions that cannot and will not throw.

Do **not** mark `noexcept` for:
- Functions that may throw (e.g., memory allocation, file I/O, network operations).
- Functions where the implementation might throw in the future.

**(b)** Marking move constructors `noexcept` is crucial for `std::vector` reallocation. When a `vector` needs to reallocate, it will try to move elements to the new memory. If the move constructor is `noexcept`, `vector` will use move, which is efficient and safe. If it is not `noexcept`, `vector` falls back to copying to guarantee strong exception safety – which can be significantly slower for large element counts.

---

### Question 16

```cpp
#include <vector>
#include <list>
#include <type_traits>
#include <memory>
#include <new>
#include <shared_mutex>
#include <stdexcept>
#include <cstddef>
#include <cstdint>

template<typename T>
class ObjectPool {
    static_assert(!std::is_array_v<T>, "Array types not supported");
    static_assert(std::is_destructible_v<T>, "T must be destructible");

    struct Block {
        alignas(alignof(T)) unsigned char storage[sizeof(T)];
        bool used = false;
    };

    // Key design: list nodes never move – expansion never invalidates existing Block*
    std::list<std::vector<Block>> chunks;
    std::vector<Block*> free_list;   // free‑block pointer stack
    size_t next_chunk_size;
    mutable std::shared_mutex mtx;

    void grow() {
        // emplace_back into list does not invalidate existing Block*
        chunks.emplace_back(next_chunk_size);
        for (auto& block : chunks.back()) {
            free_list.push_back(&block);
        }
        next_chunk_size *= 2;
    }

public:
    explicit ObjectPool(size_t initial_size = 64) : next_chunk_size(initial_size) {
        if (initial_size == 0) {
            throw std::invalid_argument("Initial pool size must be > 0");
        }
        grow();
    }

    // Resource management is complex – disable copy/move
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator=(ObjectPool&&) = delete;

    template<typename... Args>
    T* allocate(Args&&... args) {
        std::unique_lock lock(mtx);

        if (free_list.empty()) {
            grow();
        }

        Block* block = free_list.back();
        free_list.pop_back();

        // RAII guard: if construction throws, automatically return the block to free_list
        struct Guard {
            Block* b;
            std::vector<Block*>* fl;
            bool committed = false;
            ~Guard() { if (!committed) fl->push_back(b); }
        } guard{block, &free_list};

        T* obj = reinterpret_cast<T*>(&block->storage);
        std::construct_at(obj, std::forward<Args>(args)...);

        block->used = true;
        guard.committed = true;
        return obj;
    }

    void deallocate(T* ptr) {
        if (!ptr) return;

        // Recover Block* from T* via offsetof (Block is standard‑layout, safe)
        Block* block = reinterpret_cast<Block*>(
            reinterpret_cast<unsigned char*>(ptr) - offsetof(Block, storage)
        );

        std::unique_lock lock(mtx);

        if (!block->used) {
            throw std::runtime_error("Double free or invalid pointer");
        }

        std::destroy_at(ptr);
        block->used = false;
        free_list.push_back(block);
    }

    // Read‑only operations use shared locks, allowing concurrent queries
    [[nodiscard]] size_t capacity() const {
        std::shared_lock lock(mtx);
        size_t total = 0;
        for (const auto& chunk : chunks) {
            total += chunk.size();
        }
        return total;
    }

    [[nodiscard]] size_t available() const {
        std::shared_lock lock(mtx);
        return free_list.size();
    }

    [[nodiscard]] size_t used() const {
        std::shared_lock lock(mtx);
        size_t total = 0;
        for (const auto& chunk : chunks) {
            total += chunk.size();
        }
        return total - free_list.size();
    }
};
```

- Uses `std::list<std::vector<Block>> chunks` – list nodes never move, so `Block*` remain valid after expansion.
- RAII Guard automatically returns the block on construction exception.
- `std::shared_mutex`: exclusive locks for write operations (allocate/deallocate), shared locks for read operations (capacity/size).
- `block->used` flag with runtime checks.

---

## IV. Architecture and Design Patterns

### Question 17

```cpp
#include <vector>
#include <memory>
#include <algorithm>

struct ISorter {
    virtual ~ISorter() = default;
    virtual void sort(std::vector<int>& data) = 0;
};

struct QuickSort : ISorter {
    void sort(std::vector<int>& data) override {
        std::sort(data.begin(), data.end());
    }
};

struct BubbleSort : ISorter {
    void sort(std::vector<int>& data) override {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                }
            }
        }
    }
};

class SortEngine {
    std::unique_ptr<ISorter> strategy;
    
public:
    void setStrategy(std::unique_ptr<ISorter> s) {
        strategy = std::move(s);
    }
    
    void execute(std::vector<int>& data) {
        if (strategy) strategy->sort(data);
    }
};
```

---

### Question 18

```cpp
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <mutex>

class EventDispatcher {
public:
    static EventDispatcher& instance() {
        static EventDispatcher inst;
        return inst;
    }
    
    using Callback = std::function<void(const std::string&)>;
    
    void registerListener(const std::string& event, Callback cb) {
        std::lock_guard lock(mtx);
        listeners[event].push_back(std::move(cb));
    }
    
    void dispatch(const std::string& event, const std::string& data) {
        std::vector<Callback> callbacks;
        {
            std::lock_guard lock(mtx);
            auto it = listeners.find(event);
            if (it == listeners.end()) return;
            callbacks = it->second;  // copy – key point to avoid iterator invalidation
        }
        for (auto& cb : callbacks) {
            cb(data);
        }
    }
    
private:
    EventDispatcher() = default;
    ~EventDispatcher() = default;
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
    
    std::unordered_map<std::string, std::vector<Callback>> listeners;
    std::mutex mtx;
};
```

**Why copy the callback list during dispatch**: If we iterated directly over `listeners[event]` and a callback registered or unregistered a listener for the same event, the `vector` could reallocate, invalidating iterators. Copying the list and iterating over the copy avoids this problem entirely.

---

### Question 19

```cpp
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

struct IEnemy {
    virtual ~IEnemy() = default;
    virtual void attack() = 0;
};

struct Goblin : IEnemy { void attack() override { /* ... */ } };
struct Orc : IEnemy { void attack() override { /* ... */ } };
struct Dragon : IEnemy { void attack() override { /* ... */ } };

class EnemyFactory {
    std::unordered_map<std::string, std::function<std::unique_ptr<IEnemy>()>> creators;
    
public:
    EnemyFactory() {
        registerType("goblin", []{ return std::make_unique<Goblin>(); });
        registerType("orc", []{ return std::make_unique<Orc>(); });
        registerType("dragon", []{ return std::make_unique<Dragon>(); });
    }
    
    void registerType(const std::string& name, 
                      std::function<std::unique_ptr<IEnemy>()> creator) {
        creators[name] = std::move(creator);
    }
    
    std::unique_ptr<IEnemy> create(const std::string& name) const {
        auto it = creators.find(name);
        return it != creators.end() ? it->second() : nullptr;
    }
};

class Game {
    const EnemyFactory& factory;
    
public:
    explicit Game(const EnemyFactory& f) : factory(f) {}
    
    void spawnEnemy(const std::string& type) {
        auto enemy = factory.create(type);
        if (enemy) enemy->attack();
    }
};
```

**Benefits for unit testing**: `Game` does not directly depend on concrete enemy classes – it relies on the factory interface. In unit tests, you can inject a `MockFactory` that returns `MockEnemy` objects (derived from `IEnemy`) to verify that `spawnEnemy` calls `attack()` correctly and the expected number of times, without having to instantiate real `Goblin`/`Orc` objects.

---

### Question 20

**(a) Scenarios and performance differences:**

| Dimension | Compile‑time polymorphism (templates/CRTP) | Runtime polymorphism (virtual functions) |
|-----------|--------------------------------------------|-------------------------------------------|
| Binding time | Compile time | Runtime |
| Call overhead | Zero overhead, can be inlined | vtable indirection, hard to inline |
| Flexibility | Strategy fixed at compile time | Strategy can be switched dynamically |
| Code size | May bloat (one instantiation per type) | Stable |
| Use cases | Performance‑critical, strategy never changes | Dynamic configuration, plugin architectures |

**(b) Compile‑time fixed strategy (template)**:
```cpp
template<typename SortPolicy>
class ContainerCompileTime {
    std::vector<int> data;
public:
    void sort() {
        SortPolicy::sort(data);  // static call, zero overhead
    }
};

struct QuickSortPolicy {
    static void sort(std::vector<int>& data) {
        std::sort(data.begin(), data.end());
    }
};

// Usage
ContainerCompileTime<QuickSortPolicy> ct;
ct.sort();  // compile‑time determined, no virtual overhead
```

**Runtime‑determined strategy (virtual functions)**:
```cpp
struct ISortStrategy {
    virtual ~ISortStrategy() = default;
    virtual void sort(std::vector<int>& data) = 0;
};

class ContainerRuntime {
    std::vector<int> data;
    std::unique_ptr<ISortStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<ISortStrategy> s) {
        strategy = std::move(s);
    }
    void sort() {
        if (strategy) strategy->sort(data);
    }
};

// Usage
ContainerRuntime rt;
// user can choose strategy from a config file
rt.setStrategy(std::make_unique<QuickSortImpl>());
rt.sort();
```

**Decision guidance**: If the strategy is performance‑sensitive and known at compile time, choose templates. If the strategy needs to be user‑configurable or dynamically loaded, choose virtual functions.