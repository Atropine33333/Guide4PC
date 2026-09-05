明白了。所有代码注释（包括 `#` 注释和 Docstring）都用英文写，正文解释保持中文。

以下是修正后的完整第2节，只有代码块内的注释和 Docstring 改为英文，正文讲解不变：

---

# 第2节：Python 基本语法与缩进规则

## 术语快查表

| 术语 | 简单解释 |
|------|----------|
| 注释 | 代码中不会被执行的文本，用来给人看，Python 用 `#` 开头 |
| Docstring | 文档字符串，用三个引号包裹，放在函数/类/模块开头，用来生成文档 |
| 缩进 | 代码行开头的空白，Python 用它来表示代码块层级 |
| 变量 | 用来存储数据的“名字”，比如 `x = 5` 中的 `x` |
| 字符串 | 文本数据，用引号包裹，如 `"hello"` |
| 布尔值 | 只有两个值：`True` 和 `False` |
| 类型 | 数据的种类：整数、浮点数、字符串、布尔值等 |
| 类型转换 | 把数据从一种类型变成另一种，如 `int("123")` |
| f-string | 格式化字符串，在字符串中嵌入变量的值 |

---

## 2.1 注释

注释是写给人类读的，不是给计算机执行的。Python 解释器会忽略注释。

**单行注释**用 `#` 开头：

```python
# This is a comment
print("Hello")  # This is also a comment
```

**多行注释**用三个引号（单引号或双引号都可以）：

```python
"""
This is a multi-line comment.
You can write several lines here.
Python will ignore all of it.
"""
```

注释的用处：
- 解释这段代码在做什么（尤其是逻辑复杂的地方）
- 写 TODO 提醒自己还有没做完的部分
- 临时注释掉某一行代码来调试

---

## 2.2 缩进规则（重点）

Python 用**缩进**来表示代码块的层级关系。这是 Python 和其他语言最明显的区别之一。

在 C、Java、JavaScript 这些语言里，用花括号 `{}` 来划分代码块：

```c
if (x > 0) {
    printf("x is positive");
}
```

在 Python 里，没有花括号，用**缩进**来标记层级：

```python
if x > 0:
    print("x is positive")
```

**规则只有一条：同一层级的代码，缩进必须一致。**

```python
if x > 0:
    print("positive")       # 4 spaces indent
    print("greater than 0") # 4 spaces indent, same level
print("always runs")        # 0 spaces, back to outer level
```

**缩进错误会直接让程序崩溃**：

```python
if x > 0:
    print("positive")
  print("inconsistent indent")  # IndentationError
```

**缩进量**：约定用 **4 个空格**，按一次 Tab 键（大部分编辑器会自动把 Tab 转成 4 个空格）。不要混用 Tab 和空格，否则会引发难以排查的缩进错误。

为什么 Python 这么设计？因为强制统一的缩进让代码更整洁，可读性更高。你没法写出“格式混乱但逻辑正确”的代码。

---

## 2.3 变量与赋值

变量就是一个名字，用来存放数据。用 `=` 赋值：

```python
x = 10
name = "MKJ"
price = 99.99
is_ok = True
```

变量可以重新赋值，类型也可以改变（Python 是动态类型语言）：

```python
x = 10      # now an integer
x = "hello" # now a string — legal, but not recommended
```

变量名规则：
- 以字母或下划线开头
- 后面可以跟字母、数字、下划线
- 区分大小写：`name` 和 `Name` 是两个变量
- 不能用 Python 的保留字（`if`、`for`、`while`、`def` 等）

---

## 2.4 数据类型

Python 中常见的数据类型：

### 2.4.1 整数（int）

```python
age = 20
count = -5
big_num = 1_000_000  # underscore as thousands separator
```

### 2.4.2 浮点数（float）

```python
pi = 3.14159
temperature = -2.5
score = 95.0
```

### 2.4.3 字符串（str）

用单引号或双引号包裹：

```python
name = "MKJ"
city = 'Beijing'
```

字符串内可以包含引号：

```python
msg = "She said 'hello'"       # double quotes outside, single inside
msg2 = 'He said "hi"'          # single outside, double inside
```

多行字符串用三个引号：

```python
long_text = """
Line 1
Line 2
Line 3
"""
```

### 2.4.4 布尔值（bool）

只有两个值：`True` 和 `False`。首字母大写。

```python
is_student = True
is_working = False
```

### 2.4.5 查看类型

用 `type()` 函数查看数据的类型：

```python
print(type(10))         # <class 'int'>
print(type(3.14))       # <class 'float'>
print(type("hello"))    # <class 'str'>
print(type(True))       # <class 'bool'>
```

---

## 2.5 类型转换

有时需要手动把一种类型转成另一种：

```python
int("123")          # string to int → 123
float("3.14")       # string to float → 3.14
str(456)            # int to string → "456"
str(3.14)           # float to string → "3.14"
```

注意：不能把明显不是数字的字符串转成数字：

```python
int("hello")        # ValueError
```

---

## 2.6 基本输入输出

### 2.6.1 输出：print()

`print()` 把内容输出到屏幕上：

```python
print("Hello, world!")
print(123)
print(3.14)
```

可以同时输出多个值，用逗号分隔（自动加空格）：

```python
print("Age", 20, "years")
# Output: Age 20 years
```

### 2.6.2 格式化字符串：f-string

f-string 是在字符串前加 `f`，用 `{变量名}` 嵌入变量的值：

```python
def greet_user(name: str, age: int) -> str:
    """Generate a greeting message.

    Args:
        name (str): User's name.
        age (int): User's age.

    Returns:
        str: Greeting message with name and age.
    """
    return f"Hello, {name}! You are {age} years old."
```

> **关于 `->` 返回值类型注解**
>
> `def greet_user(name: str, age: int) -> str:` 中：
> - `name: str` → 参数 `name` 的类型提示是 `str`
> - `age: int` → 参数 `age` 的类型提示是 `int`
> - `-> str` → 这个函数的**返回值**类型提示是 `str`
>
> 需要强调的是：**这只是一个提示，Python 解释器不会强制检查**。
>
> ```python
> def greet_user(name: str, age: int) -> str:
>     return 123   # 返回 int，但不会报错
> ```
>
> 这符合 Python 动态类型的特性——类型注解只是给人看的（IDE 会用它做提示，linter 如 mypy 可以单独检查，但解释器本身完全忽略它）。

调用：

```python
message = greet_user("MKJ", 20)
print(message)
# Output: Hello, MKJ! You are 20 years old.
```

可以在花括号里做简单运算：

```python
x = 10
y = 5
print(f"{x} + {y} = {x + y}")
# Output: 10 + 5 = 15
```

### 2.6.3 输入：input()

`input()` 让用户从键盘输入一行文本，程序会暂停等待输入：

```python
def get_user_name() -> str:
    """Get user's name from input.

    Returns:
        str: The name entered by the user.
    """
    name = input("Please enter your name: ")
    return name
```

调用：

```python
name = get_user_name()
print(f"Hello, {name}!")
```

`input()` 返回的类型是**字符串**。如果需要数字，要手动转换：

```python
def get_age() -> int:
    """Get user's age and convert to int.

    Returns:
        int: The age entered by the user.

    Raises:
        ValueError: If the input is not a valid integer.
    """
    age_str = input("Please enter your age: ")
    return int(age_str)
```

---

## 2.7 基本运算符

### 2.7.1 算术运算符

```python
def calculate(a: float, b: float) -> None:
    """Demonstrate basic arithmetic operations.

    Args:
        a (float): First operand.
        b (float): Second operand.
    """
    print(f"{a} + {b} = {a + b}")
    print(f"{a} - {b} = {a - b}")
    print(f"{a} * {b} = {a * b}")
    print(f"{a} / {b} = {a / b}")      # division (always float)
    print(f"{a} // {b} = {a // b}")    # floor division
    print(f"{a} % {b} = {a % b}")      # modulo
    print(f"{a} ** {b} = {a ** b}")    # exponentiation
```

### 2.7.2 比较运算符

比较的结果是布尔值（`True` 或 `False`）：

```python
print(10 > 3)    # True
print(10 < 3)    # False
print(10 == 10)  # True  equal
print(10 != 3)   # True  not equal
print(10 >= 10)  # True  greater or equal
print(10 <= 3)   # False less or equal
```

### 2.7.3 逻辑运算符

```python
x = True
y = False

print(x and y)   # False
print(x or y)    # True
print(not x)     # False
```

---

## 2.8 动手：写一个完整的程序

现在把前面学的东西串起来，写一个完整的 `.py` 文件。这里我们定义一个 `add` 函数，并用 Docstring 记录它的用途。

打开 VS Code，新建 `calc.py`，写入以下代码：

```python
#!/usr/bin/env python3
"""Simple calculator module.

This module demonstrates Python basics:
- Function definition with docstring
- Type conversion
- Input / output
- f-string formatting
"""

def add(a: float, b: float) -> float:
    """Compute the sum of two numbers.

    A simple addition function demonstrating standard docstring format.
    Both arguments and return value are type-annotated.

    Args:
        a (float): First addend.
        b (float): Second addend.

    Returns:
        float: Sum of a and b.

    Example:
        >>> add(3.5, 2.5)
        6.0
        >>> add(-1, 5)
        4.0
    """
    return a + b


def main() -> None:
    """Program entry point: read two numbers and print their sum."""
    print("Simple Addition Calculator")
    
    num1 = input("Enter first number: ")
    num2 = input("Enter second number: ")
    
    # Convert string input to float
    a = float(num1)
    b = float(num2)
    
    result = add(a, b)
    print(f"{a} + {b} = {result}")


if __name__ == "__main__":
    main()
```

**Docstring 的三个位置**：

1. **Module docstring**（文件最开头）：说明这个模块是做什么的。
2. **Function docstring**（`def` 语句下面第一行）：说明函数的用途、参数、返回值，甚至可以写示例。
3. **类 Docstring**（本示例暂无）。

保存后，在终端中执行：

```powershell
py calc.py
```

按提示输入两个数字，程序会输出它们的和。

---

## 2.9 何时用 Docstring，何时用普通注释

- **Docstring（`"""..."""`）** ：放在模块、函数、类开头，用来生成文档。**给使用这个代码的人看**。用 `help(add)` 可以看到 Docstring 的内容。
- **Inline comments（`#`）** ：放在代码行上方或右侧，解释某一段代码的写法或原理。**给读代码的人看**，帮助理解实现细节。

```python
def divide(a: float, b: float) -> float:
    """Compute the quotient of two numbers. 

    Args:
        a (float): Dividend.
        b (float): Divisor.

    Returns:
        float: Quotient.

    Raises:
        ZeroDivisionError: If b is 0.
    """
    # Check for zero divisor to avoid crash
    if b == 0:
        raise ZeroDivisionError("divisor cannot be zero")
    return a / b
```

---

## 2.10 动态类型：不需要显式声明变量类型

**这部分可以先跳过**  
**这部分可以先跳过**  
**这部分可以先跳过**

如果你之前学过 C++ 或 Kotlin，写变量时通常会带上类型：

```cpp
// C++
int x = 10;
string name = "MKJ";
```

```kotlin
// Kotlin
val x: Int = 10
var name: String = "MKJ"
```

Python 不需要这样做。Python 是**动态类型语言**，变量的类型是在**运行时由赋值决定**的：

```python
x = 10           # x 是 int
x = "hello"      # 现在 x 是 str —— 完全合法
```

你不需要写 `int x = 10`，也不需要写 `str name = "MKJ"`。Python 会根据你赋的值自动推断类型。

**类型注解（Type Hints）是可选的**。Python 3.5 之后支持了类型注解语法：

```python
x: int = 10
name: str = "MKJ"
```

但请记住：**这只是一个“注释”，不影响程序运行**。Python 解释器不会因为你写了 `: int` 就真的把 `x` 限制为整数类型：

```python
x: int = 10
x = "hello"      # 仍然合法，注解不会阻止
```

这和 C++ / Kotlin 有本质区别。在 C++ 里，`int x = 10; x = "hello";` 编译不通过。在 Kotlin 里，`var x: Int = 10; x = "hello"` 也编译不通过。但在 Python 里，类型注解只是给人看的——给 IDE、给 linter、给阅读代码的人——解释器完全忽略它。

### 2.10.1 需要注意的坑

**坑一：类型错误在运行时才会暴露**

因为 Python 不检查类型，很多在 C++/Kotlin 里能在编译期发现的问题，在 Python 里只有程序跑起来才会报错：

```python
def add(a, b):
    return a + b

result = add(10, "20")   # 运行时 TypeError: unsupported operand type(s) for +: 'int' and 'str'
```

**坑二：类型注解不能替代类型检查**

```python
def add(a: int, b: int) -> int:
    return a + b

# 下面这行不会报错，程序正常运行
add(10, "20")   # 返回 "1020"，因为字符串拼接也能用 +
```

如果你需要真正的类型检查，需要用 `mypy` 这样的工具单独运行检查，或者用 `pydantic` 做运行时校验。Python 本身不会帮你做这件事。

**坑三：变量可以随时改变类型，可能带来意想不到的行为**

```python
# 这里 value 可能是从某个函数返回的
value = get_data()  # 可能返回 int，也可能返回 None
result = value + 10  # 如果 value 是 None，这里报错
```

这在 C++/Kotlin 里通常不是问题——因为类型是明确的，编译器会帮你检查。在 Python 里，你需要自己留意变量可能是什么类型，尤其是从外部传入的数据。

### 2.10.2 要不要写类型注解

用 Python 的惯用建议是：

- **写代码时尽量加上类型注解**，让 IDE 能提供更好的自动补全和错误提示。
- **但不要依赖注解来做运行时检查**，它只是一个文档和静态分析工具。
- **多用 `mypy` 做静态类型检查**，如果你习惯强类型的开发方式，这能帮你提前发现很多问题。

如果你刚接触 Python，可以先不写类型注解，把注意力放在语法和逻辑上。等你熟悉了 Python 的动态特性，再慢慢养成写类型注解的习惯。


本节的示例代码中，Docstring 用来解释函数接口，普通注释用来解释实现细节。养成这个习惯，以后写工程代码会清晰很多。

---

## 2.11 本节你做了什么

1. 学会了用 `#` 写注释，用 `"""` 写 Docstring（代码内全部使用英文）
2. 理解了 Python 的缩进规则——**用缩进表示层级，同一层级的缩进必须一致**
3. 掌握了变量赋值和基本数据类型（int、float、str、bool）
4. 学会了用 `type()` 查看类型，以及类型转换
5. 学会了用 `print()` 输出，用 `input()` 接收输入
6. 学会了 f-string 格式化字符串
7. 学会了算术、比较、逻辑运算符
8. 写了一个包含模块级 Docstring 和函数级 Docstring 的完整程序
9. 区分了 Docstring（给使用者看）和普通注释（给维护者看）的不同用途
10. 学会了 Python 不需要显示表明变量类型的优势和导致的问题
