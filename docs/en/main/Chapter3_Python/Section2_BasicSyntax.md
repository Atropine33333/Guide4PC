# Section 2: Python Basic Syntax and Indentation Rules

## Quick Reference Table

| Term | Plain Explanation |
|------|-------------------|
| Comment | Text in code that is not executed – written for humans; Python uses `#` for comments |
| Docstring | Documentation string, wrapped in triple quotes, placed at the top of modules, functions, and classes – used to generate documentation |
| Indentation | Leading whitespace at the beginning of a line – Python uses it to indicate block nesting |
| Variable | A "name" that stores data – for example, `x` in `x = 5` |
| String | Text data, enclosed in quotes, such as `"hello"` |
| Boolean | A type with only two values: `True` and `False` |
| Type | The kind of data: integer, float, string, boolean, etc. |
| Type conversion | Changing data from one type to another – e.g., `int("123")` |
| f-string | A formatted string that embeds variable values directly inside a string literal |

---

## 2.1 Comments

Comments are written for human readers, not for the computer. The Python interpreter ignores them entirely.

**Single‑line comments** start with `#`:

```python
# This is a comment
print("Hello")  # This is also a comment
```

**Multi‑line comments** are written with three quotes (single or double):

```python
"""
This is a multi-line comment.
You can write several lines here.
Python will ignore all of it.
"""
```

Why use comments:
- Explain what the code is doing (especially in complex logic)
- Write TODO reminders for unfinished parts
- Temporarily comment out a line for debugging

---

## 2.2 Indentation Rules (Crucial)

Python uses **indentation** to represent the nesting level of code blocks. This is one of the most visible differences between Python and many other languages.

In languages like C, Java, or JavaScript, curly braces `{}` are used to delimit blocks:

```c
if (x > 0) {
    printf("x is positive");
}
```

In Python, there are no curly braces – **indentation** marks the hierarchy:

```python
if x > 0:
    print("x is positive")
```

**The only rule is: indentation must be consistent within the same block level.**

```python
if x > 0:
    print("positive")       # 4 spaces indent
    print("greater than 0") # 4 spaces indent, same level
print("always runs")        # 0 spaces, back to outer level
```

**Incorrect indentation will crash your program**:

```python
if x > 0:
    print("positive")
  print("inconsistent indent")  # IndentationError
```

**Amount of indentation**: the convention is **4 spaces**. Pressing the Tab key usually inserts 4 spaces in modern editors (they often convert tabs to spaces automatically). Never mix tabs and spaces – that leads to hard‑to‑debug indentation errors.

Why does Python design it this way? Because enforced uniform indentation makes code cleaner and more readable – you simply cannot write "messy but logically correct" code.

---

## 2.3 Variables and Assignment

A variable is simply a name that holds data. Use `=` for assignment:

```python
x = 10
name = "MKJ"
price = 99.99
is_ok = True
```

Variables can be reassigned, and their type can change (Python is dynamically typed):

```python
x = 10      # now an integer
x = "hello" # now a string — legal, but not recommended
```

Variable naming rules:
- Must start with a letter or underscore
- Subsequent characters can be letters, digits, or underscores
- Case‑sensitive: `name` and `Name` are different variables
- Cannot use Python's reserved words (`if`, `for`, `while`, `def`, etc.)

---

## 2.4 Data Types

Common data types in Python:

### 2.4.1 Integer (`int`)

```python
age = 20
count = -5
big_num = 1_000_000  # underscore as thousands separator
```

### 2.4.2 Float (`float`)

```python
pi = 3.14159
temperature = -2.5
score = 95.0
```

### 2.4.3 String (`str`)

Enclosed in single or double quotes:

```python
name = "MKJ"
city = 'Beijing'
```

Strings can contain quotes inside:

```python
msg = "She said 'hello'"       # double outside, single inside
msg2 = 'He said "hi"'          # single outside, double inside
```

Multi‑line strings use triple quotes:

```python
long_text = """
Line 1
Line 2
Line 3
"""
```

### 2.4.4 Boolean (`bool`)

Only two values: `True` and `False` (capitalised).

```python
is_student = True
is_working = False
```

### 2.4.5 Checking the Type

Use the `type()` function to inspect a value's type:

```python
print(type(10))         # <class 'int'>
print(type(3.14))       # <class 'float'>
print(type("hello"))    # <class 'str'>
print(type(True))       # <class 'bool'>
```

---

## 2.5 Type Conversion

Sometimes you need to manually convert one type to another:

```python
int("123")          # string to int → 123
float("3.14")       # string to float → 3.14
str(456)            # int to string → "456"
str(3.14)           # float to string → "3.14"
```

Note: converting a string that is clearly not a number will fail:

```python
int("hello")        # ValueError
```

---

## 2.6 Basic Input and Output

### 2.6.1 Output: `print()`

`print()` sends output to the screen:

```python
print("Hello, world!")
print(123)
print(3.14)
```

You can print multiple values separated by commas (a space is added automatically):

```python
print("Age", 20, "years")
# Output: Age 20 years
```

### 2.6.2 Formatted Strings: f‑strings

An f‑string is a string literal prefixed with `f`; you embed variables inside `{}`:

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

> **About the `->` return type annotation**
>
> In `def greet_user(name: str, age: int) -> str:`:
> - `name: str` → type hint that `name` should be a `str`
> - `age: int` → type hint that `age` should be an `int`
> - `-> str` → type hint that the function **returns** a `str`
>
> It is important to stress: **this is only a hint – Python does not enforce it.**
>
> ```python
> def greet_user(name: str, age: int) -> str:
>     return 123   # returns an int, but no error
> ```
>
> This is consistent with Python's dynamic nature – type hints are for humans (IDEs use them for suggestions, tools like mypy can check them separately, but the interpreter itself ignores them completely).

Usage:

```python
message = greet_user("MKJ", 20)
print(message)
# Output: Hello, MKJ! You are 20 years old.
```

You can even do simple operations inside the curly braces:

```python
x = 10
y = 5
print(f"{x} + {y} = {x + y}")
# Output: 10 + 5 = 15
```

### 2.6.3 Input: `input()`

`input()` reads a line of text from the keyboard – the program pauses and waits for input:

```python
def get_user_name() -> str:
    """Get user's name from input.

    Returns:
        str: The name entered by the user.
    """
    name = input("Please enter your name: ")
    return name
```

Usage:

```python
name = get_user_name()
print(f"Hello, {name}!")
```

`input()` always returns a **string**. If you need a number, convert it manually:

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

## 2.7 Basic Operators

### 2.7.1 Arithmetic Operators

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

### 2.7.2 Comparison Operators

Comparisons yield booleans (`True` or `False`):

```python
print(10 > 3)    # True
print(10 < 3)    # False
print(10 == 10)  # True
print(10 != 3)   # True
print(10 >= 10)  # True
print(10 <= 3)   # False
```

### 2.7.3 Logical Operators

```python
x = True
y = False

print(x and y)   # False
print(x or y)    # True
print(not x)     # False
```

---

## 2.8 Hands‑on: Write a Complete Program

Now let's put everything together and write a full `.py` file. We'll define an `add` function and document it with a Docstring.

Open VS Code, create a new file `calc.py`, and write the following:

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

**Three places for Docstrings**:

1. **Module docstring** (at the very top): explains what the module does.
2. **Function docstring** (the first line after `def`): explains the function's purpose, parameters, return value, and may include examples.
3. **Class docstring** (not shown here).

Save the file and run it in the terminal:

```powershell
py calc.py
```

Follow the prompts – the program will output the sum of the two numbers.

---

## 2.9 When to Use Docstrings vs. Inline Comments

- **Docstrings (`"""..."""`)**: placed at the top of modules, functions, and classes – used to generate documentation. **For users of your code**. You can view a Docstring with `help(add)`.
- **Inline comments (`#`)**: placed above or to the right of a line – explain the writing style or rationale of a particular piece of code. **For maintainers of your code**, to help understand implementation details.

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

## 2.10 Dynamic Typing: No Explicit Type Declarations

**This subsection can be skipped on first reading**  
**This subsection can be skipped on first reading**  
**This subsection can be skipped on first reading**

If you have previously learned C++ or Kotlin, you are used to writing types explicitly when declaring variables:

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

Python does not require this. Python is a **dynamically typed language** – the type of a variable is determined **at runtime by the value assigned to it**:

```python
x = 10           # x is int
x = "hello"      # now x is str — perfectly legal
```

You do not need to write `int x = 10` or `str name = "MKJ"`. Python automatically infers the type from the value you assign.

**Type hints are optional**. Python 3.5+ supports type hint syntax:

```python
x: int = 10
name: str = "MKJ"
```

But remember: **these are just "annotations" – they do not affect runtime behaviour**. Python will not restrict `x` to integers just because you wrote `: int`:

```python
x: int = 10
x = "hello"      # still legal – the annotation does not prevent it
```

This is fundamentally different from C++ or Kotlin. In C++, `int x = 10; x = "hello";` fails to compile. In Kotlin, `var x: Int = 10; x = "hello"` also fails. In Python, type hints are only for humans – for IDEs, linters, and readers – the interpreter completely ignores them.

### 2.10.1 Pitfalls to Watch Out For

**Pitfall 1: Type errors only appear at runtime**

Because Python does not check types, many errors that would be caught at compile time in C++/Kotlin only surface when the program actually runs:

```python
def add(a, b):
    return a + b

result = add(10, "20")   # Runtime TypeError: unsupported operand type(s) for +: 'int' and 'str'
```

**Pitfall 2: Type hints are not a substitute for runtime checking**

```python
def add(a: int, b: int) -> int:
    return a + b

# The line below does NOT cause an error – the program runs normally
add(10, "20")   # returns "1020" because string concatenation also uses +
```

If you need actual type checking, you must use an external tool like `mypy` for static analysis, or use runtime validation with libraries like `pydantic`. Python itself does not enforce types.

**Pitfall 3: Variables can change type at any time, leading to unexpected behaviour**

```python
# Here, `value` might come from some function
value = get_data()  # could return int or None
result = value + 10  # if value is None, this raises an error
```

In C++/Kotlin this is usually not an issue – the type is explicit and the compiler checks it. In Python, you need to keep track of what types your variables might hold, especially for data coming from external sources.

### 2.10.2 Should You Write Type Hints?

The pragmatic advice in the Python community:

- **Try to add type hints when writing code** – they give IDEs better autocompletion and error detection.
- **But do not rely on them for runtime safety** – they are documentation and static‑analysis aids.
- **Use `mypy` regularly for static type checking** if you come from a strongly‑typed background – it can catch many issues early.

If you are new to Python, feel free to skip type hints at first and focus on the syntax and logic. Once you are comfortable with Python's dynamic nature, gradually build the habit of adding type hints.

---

In the example code for this section, Docstrings explain the function interface, while inline comments explain implementation details. Building this habit early will make your future engineering code much clearer.

---

## 2.11 What You've Accomplished in This Section

1. Learned to write comments with `#` and Docstrings with `"""` (all code‑internal text is in English)
2. Understood Python's indentation rules – **indentation marks block hierarchy, and indentation must be consistent at the same level**
3. Mastered variable assignment and basic data types (int, float, str, bool)
4. Learned to check types with `type()` and perform type conversion
5. Learned to output with `print()` and read input with `input()`
6. Learned f‑string formatting
7. Learned arithmetic, comparison, and logical operators
8. Wrote a complete program with module‑level and function‑level Docstrings
9. Distinguished the different purposes of Docstrings (for users) and inline comments (for maintainers)
10. Learned the advantages and pitfalls of Python's dynamic typing system