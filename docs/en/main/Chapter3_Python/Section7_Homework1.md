# Section 7: Homework 1 – 25 Basic Exercises

## 7.1 Instructions

This section is for practice. The 25 questions cover everything from the previous six sections. For each problem, you should write a **standalone function** that includes the full flow of input, processing, and output. Name the functions `q1()` through `q25()` and include an `if __name__ == "__main__"` entry point.  
Forgive me – I couldn't come up with anything too interesting.

**Answer format**: each answer is a complete function definition that you can copy and run directly.

---

## Problem Set

### Q1: Reverse a String
Input a string and output its reverse.

```python
# Your code here
```


### Q2: Palindrome Check
Input a string and check whether it is a palindrome (ignoring spaces and case).

```python
# Your code here
```


### Q3: Extract Even Numbers
Input a list of integers and extract all even numbers.

```python
# Your code here
```


### Q4: Remove Duplicates
Input a list of integers and remove duplicates while preserving the original order.

```python
# Your code here
```


### Q5: Character Frequency
Input a string and count the frequency of each character.

```python
# Your code here
```


### Q6: Factorial
Input a non‑negative integer n and compute n!.

```python
# Your code here
```


### Q7: Prime Check
Input an integer and check whether it is prime.

```python
# Your code here
```


### Q8: Fibonacci Sequence
Input n and output the first n Fibonacci numbers.

```python
# Your code here
```


### Q9: Average
Input a list of numbers and compute their average.

```python
# Your code here
```


### Q10: Min and Max
Input a list of numbers and find the maximum and minimum values.

```python
# Your code here
```


### Q11: Merge Two Dictionaries
Given two dictionaries, merge them. In case of key conflicts, the second dictionary takes priority.

```python
# Your code here
```


### Q12: Sort
Input a list of integers and output them in ascending and descending order.

```python
# Your code here
```


### Q13: Remove Target Element
Input a list of integers and a target value, then remove all occurrences of the target from the list.

```python
# Your code here
```


### Q14: Slicing
From the list `[0,1,2,3,4,5,6,7,8,9]`, extract: the first 3, the last 3, every second element, the reversed list, and indices 2 through 7.

```python
# Your code here
```


### Q15: Extract Digits
Input a string and extract all digit characters from it.

```python
# Your code here
```


### Q16: Input Validation
Repeatedly prompt the user for an integer until a valid one is entered.

```python
# Your code here
```


### Q17: Set Operations
Input two sets of integers and output their intersection, union, difference, and symmetric difference.

```python
# Your code here
```


### Q18: Find All Positions
Input a list of strings and a target value, then output all positions where the target appears.

```python
# Your code here
```


### Q19: Squares
Input a list of integers and use a list comprehension to generate their squares.

```python
# Your code here
```


### Q20: Swap Keys and Values
Input a dictionary and swap its keys and values (assume values are unique and immutable).

```python
# Your code here
```


### Q21: Flatten a List
Flatten the nested list `[[1,2,3],[4,5],[6,7,8,9],[10]]` into a single level.

```python
# Your code here
```


### Q22: Case Conversions
Input a string and output it in uppercase, lowercase, title case, capitalised, and swapped case.

```python
# Your code here
```


### Q23: Two Lists to Dictionary
Input two lists of equal length (keys and values) and combine them into a dictionary.

```python
# Your code here
```


### Q24: Shallow vs Deep Copy Demo
Create a nested list, then copy it using assignment, shallow copy, and deep copy. Modify the inner elements of the copies and observe the changes to the original.

```python
# Your code here
```


### Q25: Simple Calculator
Implement a command‑line calculator that supports `+`, `-`, `*`, `/`. Type `quit` to exit.

```python
# Your code here
```


---

## Answer Key

### A1: Reverse a String

```python
#!/usr/bin/env python3
"""Q1: Reverse a string."""

def q1() -> None:
    text = input("Enter a string: ")
    print(f"Reversed: {text[::-1]}")

if __name__ == "__main__":
    q1()
```


### A2: Palindrome Check

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


### A3: Extract Even Numbers

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


### A4: Remove Duplicates

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


### A5: Character Frequency

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


### A6: Factorial

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


### A7: Prime Check

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


### A8: Fibonacci Sequence

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


### A9: Average

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


### A10: Min and Max

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


### A11: Merge Two Dictionaries

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


### A12: Sort

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


### A13: Remove Target Element

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


### A14: Slicing

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


### A15: Extract Digits

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


### A16: Input Validation

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


### A17: Set Operations

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


### A18: Find All Positions

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


### A19: Squares

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


### A20: Swap Keys and Values

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


### A21: Flatten a List

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


### A22: Case Conversions

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


### A23: Two Lists to Dictionary

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


### A24: Shallow vs Deep Copy Demo

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


### A25: Simple Calculator

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


## 7.7 What You've Learned

By completing these 25 exercises, you should have:

1. Become proficient with string slicing, methods, and iteration
2. Mastered list operations: adding, removing, modifying, comprehensions, sorting, and slicing
3. Understood dictionary operations: key‑value access, iteration, merging
4. Learned to use sets for deduplication, membership checks, and set operations
5. Understood the difference between shallow and deep copy
6. Become able to define and call functions independently
7. Developed the habit of handling invalid input (using `try-except`)
8. Been able to complete the full flow of input, processing, and output inside a function