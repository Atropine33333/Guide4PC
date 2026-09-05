# 第7节：作业1 —— 25个基础练习

## 7.1 说明

这一节是练习。25个题目覆盖了前六节讲过的所有内容。你应该把每道题都做成一个**独立的函数**，包含输入、处理和输出全套逻辑。函数名统一为 `q1()` 到 `q25()`，并带有 `if __name__ == "__main__"` 调用入口。  
原谅我实在没法想出有意思的题目。

**答案结构**：每个答案都是一个完整的函数定义，可以直接复制运行。

---

## 题目区

### Q1：字符串反转
输入一个字符串，输出它的反转结果。

```python
# Your code here
```


### Q2：判断回文
输入一个字符串，判断它是否是回文（忽略空格和大小写）。

```python
# Your code here
```


### Q3：提取偶数
输入一组整数，提取其中所有的偶数。

```python
# Your code here
```


### Q4：列表去重
输入一组整数，去除重复元素，保持原有顺序。

```python
# Your code here
```


### Q5：字符频率统计
输入一个字符串，统计每个字符出现的次数。

```python
# Your code here
```


### Q6：阶乘
输入一个非负整数 n，计算 n!。

```python
# Your code here
```


### Q7：判断素数
输入一个整数，判断它是否为素数。

```python
# Your code here
```


### Q8：斐波那契数列
输入 n，输出前 n 个斐波那契数。

```python
# Your code here
```


### Q9：计算平均值
输入一组数字，计算它们的平均值。

```python
# Your code here
```


### Q10：找最大最小值
输入一组数字，找出其中的最大值和最小值。

```python
# Your code here
```


### Q11：合并两个字典
给定两个字典，合并它们。键冲突时第二个字典优先。

```python
# Your code here
```


### Q12：排序
输入一组整数，分别输出升序和降序结果。

```python
# Your code here
```


### Q13：删除指定元素
输入一组整数和一个目标值，删除列表中所有等于目标值的元素。

```python
# Your code here
```


### Q14：切片操作
对列表 `[0,1,2,3,4,5,6,7,8,9]` 分别取出：前3个、后3个、每2个取1个、反转、索引2到7。

```python
# Your code here
```


### Q15：提取数字
输入一个字符串，提取其中的所有数字字符。

```python
# Your code here
```


### Q16：输入验证
反复提示用户输入整数，直到输入合法为止。

```python
# Your code here
```


### Q17：集合运算
输入两组整数，输出它们的交集、并集、差集、对称差集。

```python
# Your code here
```


### Q18：查找元素位置
输入一组字符串和一个目标值，输出目标值在列表中所有出现的位置。

```python
# Your code here
```


### Q19：平方数
输入一组整数，用列表推导式生成它们的平方。

```python
# Your code here
```


### Q20：键值交换
输入一个字典，交换键和值（假设值唯一且不可变）。

```python
# Your code here
```


### Q21：扁平化列表
将嵌套列表 `[[1,2,3],[4,5],[6,7,8,9],[10]]` 展平为一层。

```python
# Your code here
```


### Q22：大小写转换
输入一个字符串，分别输出全大写、全小写、标题格式、首字母大写、大小写互换。

```python
# Your code here
```


### Q23：两列表转字典
输入两个长度相同的列表（键列表和值列表），将它们转换为字典。

```python
# Your code here
```


### Q24：深浅拷贝演示
创建一个嵌套列表，分别用赋值、浅拷贝、深拷贝复制，修改副本的内层元素，观察原列表的变化。

```python
# Your code here
```


### Q25：简单计算器
实现一个命令行计算器，支持 `+`、`-`、`*`、`/`，输入 `quit` 退出。

```python
# Your code here
```


---

## 答案区

### A1：字符串反转

```python
#!/usr/bin/env python3
"""Q1: Reverse a string."""

def q1() -> None:
    text = input("Enter a string: ")
    print(f"Reversed: {text[::-1]}")

if __name__ == "__main__":
    q1()
```


### A2：判断回文

```python
#!/usr/bin/env python3
"""Q2: Check if a string is a palindrome."""

def q2() -> None:
    text = input("Enter a string: ")
    cleaned = text.replace(" ", "").lower()
    is_pal = cleaned == cleaned[::-1]
    print(f"'{text}' is{' ' if is_pal else ' not '}a palindrome.")

if __name__ == "__main__":
    q2()
```


### A3：提取偶数

```python
#!/usr/bin/env python3
"""Q3: Extract even numbers from a list."""

def q3() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [int(x) for x in raw.split()]
    evens = [n for n in numbers if n % 2 == 0]
    print(f"Original: {numbers}")
    print(f"Even numbers: {evens}")

if __name__ == "__main__":
    q3()
```


### A4：列表去重

```python
#!/usr/bin/env python3
"""Q4: Remove duplicates from a list while preserving order."""

def q4() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [int(x) for x in raw.split()]
    seen = set()
    result = []
    for n in numbers:
        if n not in seen:
            seen.add(n)
            result.append(n)
    print(f"Original: {numbers}")
    print(f"Deduplicated: {result}")

if __name__ == "__main__":
    q4()
```


### A5：字符频率统计

```python
#!/usr/bin/env python3
"""Q5: Count character frequencies in a string."""

def q5() -> None:
    text = input("Enter a string: ")
    freq = {}
    for ch in text:
        freq[ch] = freq.get(ch, 0) + 1
    print("Character frequency:")
    for ch, count in freq.items():
        print(f"  '{ch}': {count}")

if __name__ == "__main__":
    q5()
```


### A6：阶乘

```python
#!/usr/bin/env python3
"""Q6: Calculate factorial."""

def q6() -> None:
    try:
        n = int(input("Enter a non-negative integer: "))
        if n < 0:
            print("Factorial is not defined for negative numbers.")
            return
        result = 1
        for i in range(2, n + 1):
            result *= i
        print(f"{n}! = {result}")
    except ValueError:
        print("Invalid input.")

if __name__ == "__main__":
    q6()
```


### A7：判断素数

```python
#!/usr/bin/env python3
"""Q7: Check if a number is prime."""

def q7() -> None:
    try:
        n = int(input("Enter an integer: "))
        if n < 2:
            print(f"{n} is not prime.")
            return
        is_prime = True
        for i in range(2, int(n ** 0.5) + 1):
            if n % i == 0:
                is_prime = False
                break
        print(f"{n} is{' ' if is_prime else ' not '}prime.")
    except ValueError:
        print("Invalid input.")

if __name__ == "__main__":
    q7()
```


### A8：斐波那契数列

```python
#!/usr/bin/env python3
"""Q8: Generate first N Fibonacci numbers."""

def q8() -> None:
    try:
        n = int(input("How many Fibonacci numbers? "))
        if n <= 0:
            print("Please enter a positive integer.")
            return
        fib = [0, 1]
        for i in range(2, n):
            fib.append(fib[i-1] + fib[i-2])
        print(f"First {n} Fibonacci numbers: {fib[:n]}")
    except ValueError:
        print("Invalid input.")

if __name__ == "__main__":
    q8()
```


### A9：计算平均值

```python
#!/usr/bin/env python3
"""Q9: Calculate average of numbers."""

def q9() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [float(x) for x in raw.split()]
    if not numbers:
        print("No numbers provided.")
        return
    print(f"Average: {sum(numbers) / len(numbers):.2f}")

if __name__ == "__main__":
    q9()
```


### A10：找最大最小值

```python
#!/usr/bin/env python3
"""Q10: Find maximum and minimum values."""

def q10() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [float(x) for x in raw.split()]
    if not numbers:
        print("No numbers provided.")
        return
    print(f"Max: {max(numbers)}")
    print(f"Min: {min(numbers)}")

if __name__ == "__main__":
    q10()
```


### A11：合并两个字典

```python
#!/usr/bin/env python3
"""Q11: Merge two dictionaries."""

def q11() -> None:
    dict_a = {"a": 1, "b": 2, "c": 3}
    dict_b = {"b": 10, "d": 20, "e": 30}
    print(f"Dict A: {dict_a}")
    print(f"Dict B: {dict_b}")
    print(f"Merged: {dict_a | dict_b}")

if __name__ == "__main__":
    q11()
```


### A12：排序

```python
#!/usr/bin/env python3
"""Q12: Sort numbers ascending and descending."""

def q12() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [int(x) for x in raw.split()]
    if not numbers:
        print("No numbers provided.")
        return
    print(f"Original: {numbers}")
    print(f"Ascending: {sorted(numbers)}")
    print(f"Descending: {sorted(numbers, reverse=True)}")

if __name__ == "__main__":
    q12()
```


### A13：删除指定元素

```python
#!/usr/bin/env python3
"""Q13: Remove all occurrences of a target element."""

def q13() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [int(x) for x in raw.split()]
    try:
        target = int(input("Enter the number to remove: "))
    except ValueError:
        print("Invalid input.")
        return
    filtered = [n for n in numbers if n != target]
    removed = len(numbers) - len(filtered)
    print(f"Original: {numbers}")
    print(f"After removal: {filtered}")
    print(f"Removed {removed} occurrence(s)")

if __name__ == "__main__":
    q13()
```


### A14：切片操作

```python
#!/usr/bin/env python3
"""Q14: Demonstrate various slicing operations."""

def q14() -> None:
    data = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    print(f"Original: {data}")
    print(f"First 3: {data[:3]}")
    print(f"Last 3: {data[-3:]}")
    print(f"Every 2nd: {data[::2]}")
    print(f"Reversed: {data[::-1]}")
    print(f"Index 2 to 7: {data[2:8]}")

if __name__ == "__main__":
    q14()
```


### A15：提取数字

```python
#!/usr/bin/env python3
"""Q15: Extract digit characters from a string."""

def q15() -> None:
    text = input("Enter a string: ")
    digits = ''.join(ch for ch in text if ch.isdigit())
    print(f"Digits: {digits}")

if __name__ == "__main__":
    q15()
```


### A16：输入验证

```python
#!/usr/bin/env python3
"""Q16: Repeatedly prompt until a valid integer is entered."""

def q16() -> None:
    while True:
        try:
            value = int(input("Enter an integer: "))
            print(f"OK: {value}")
            break
        except ValueError:
            print("Invalid. Try again.")

if __name__ == "__main__":
    q16()
```


### A17：集合运算

```python
#!/usr/bin/env python3
"""Q17: Set intersection, union, difference, symmetric difference."""

def q17() -> None:
    raw_a = input("Set A (numbers): ")
    raw_b = input("Set B (numbers): ")
    set_a = set(int(x) for x in raw_a.split())
    set_b = set(int(x) for x in raw_b.split())
    print(f"A: {set_a}")
    print(f"B: {set_b}")
    print(f"A ∩ B: {set_a & set_b}")
    print(f"A ∪ B: {set_a | set_b}")
    print(f"A - B: {set_a - set_b}")
    print(f"A △ B: {set_a ^ set_b}")

if __name__ == "__main__":
    q17()
```


### A18：查找元素位置

```python
#!/usr/bin/env python3
"""Q18: Find all positions of a target element."""

def q18() -> None:
    items = input("Enter items separated by spaces: ").split()
    target = input("Enter target: ")
    positions = [i for i, item in enumerate(items) if item == target]
    if positions:
        print(f"Found '{target}' at: {positions}")
    else:
        print(f"'{target}' not found.")

if __name__ == "__main__":
    q18()
```


### A19：平方数

```python
#!/usr/bin/env python3
"""Q19: Generate squares using list comprehension."""

def q19() -> None:
    raw = input("Enter numbers separated by spaces: ")
    numbers = [int(x) for x in raw.split()]
    print(f"Squares: {[n ** 2 for n in numbers]}")

if __name__ == "__main__":
    q19()
```


### A20：键值交换

```python
#!/usr/bin/env python3
"""Q20: Swap keys and values in a dictionary."""

def q20() -> None:
    raw = input("Enter key:value pairs (comma separated): ")
    pairs = [p.strip() for p in raw.split(",") if p.strip()]
    d = {}
    for pair in pairs:
        if ":" in pair:
            k, v = pair.split(":", 1)
            d[k.strip()] = v.strip()
    print(f"Original: {d}")
    print(f"Swapped: { {v: k for k, v in d.items()} }")

if __name__ == "__main__":
    q20()
```


### A21：扁平化列表

```python
#!/usr/bin/env python3
"""Q21: Flatten a nested list one level."""

def q21() -> None:
    nested = [[1, 2, 3], [4, 5], [6, 7, 8, 9], [10]]
    flat = [item for sublist in nested for item in sublist]
    print(f"Nested: {nested}")
    print(f"Flattened: {flat}")

if __name__ == "__main__":
    q21()
```


### A22：大小写转换

```python
#!/usr/bin/env python3
"""Q22: Demonstrate string case conversions."""

def q22() -> None:
    text = input("Enter a string: ")
    print(f"Original: {text}")
    print(f"Uppercase: {text.upper()}")
    print(f"Lowercase: {text.lower()}")
    print(f"Title: {text.title()}")
    print(f"Capitalize: {text.capitalize()}")
    print(f"Swapcase: {text.swapcase()}")

if __name__ == "__main__":
    q22()
```


### A23：两列表转字典

```python
#!/usr/bin/env python3
"""Q23: Convert two lists to a dictionary."""

def q23() -> None:
    keys = input("Keys: ").split()
    values = input("Values: ").split()
    if len(keys) != len(values):
        print("Error: lists must have same length.")
        return
    print(dict(zip(keys, values)))

if __name__ == "__main__":
    q23()
```


### A24：深浅拷贝演示

```python
#!/usr/bin/env python3
"""Q24: Demonstrate shallow vs deep copy."""

import copy

def q24() -> None:
    original = [[1, 2], [3, 4], [5, 6]]
    print(f"Original: {original}")

    # Assignment
    assigned = original
    assigned[0].append(99)
    print(f"After assignment: {original}  <- changed!")

    # Shallow copy
    original2 = [[1, 2], [3, 4], [5, 6]]
    shallow = original2[:]
    shallow[0].append(88)
    print(f"After shallow copy: {original2}  <- changed!")

    # Deep copy
    original3 = [[1, 2], [3, 4], [5, 6]]
    deep = copy.deepcopy(original3)
    deep[0].append(77)
    print(f"After deep copy: {original3}  <- unchanged!")

    print("\nConclusion:")
    print("  Assignment = same object")
    print("  Shallow copy = new outer list, shared inner lists")
    print("  Deep copy = completely independent")

if __name__ == "__main__":
    q24()
```


### A25：简单计算器

```python
#!/usr/bin/env python3
"""Q25: Simple calculator with +, -, *, /."""

def q25() -> None:
    print("Calculator: +, -, *, /")
    while True:
        expr = input("Enter expression (e.g. 3 + 5) or 'quit': ").strip()
        if expr.lower() == "quit":
            break
        parts = expr.split()
        if len(parts) != 3:
            print("Format: number operator number")
            continue
        try:
            a, op, b = float(parts[0]), parts[1], float(parts[2])
        except ValueError:
            print("Invalid number")
            continue
        if op == "+":
            print(f"{a} + {b} = {a + b}")
        elif op == "-":
            print(f"{a} - {b} = {a - b}")
        elif op == "*":
            print(f"{a} * {b} = {a * b}")
        elif op == "/":
            if b == 0:
                print("Cannot divide by zero")
            else:
                print(f"{a} / {b} = {a / b}")
        else:
            print(f"Unknown operator: {op}")

if __name__ == "__main__":
    q25()
```


## 7.7 你学到了什么

完成了这25个练习，你应该已经：

1. 熟练了字符串的切片、方法、遍历
2. 掌握了列表的增删改查、推导式、排序、切片
3. 理解了字典的键值操作、遍历、合并
4. 会使用集合的去重、成员检查、集合运算
5. 理解了深浅拷贝的区别
6. 能独立定义和调用函数
7. 养成了处理异常输入的习惯（`try-except`）
8. 能在函数内部完成输入、处理、输出全套逻辑