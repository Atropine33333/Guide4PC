# 报错文案写作指南

这份文档只解决一件事：把 clang 那些冷冰冰的英文诊断，写成“人话”，让人愿意读、读完知道下一步做什么。

你写文案的地方只有一个：`diagnostics/messages.json`。每条规则的 `human` 和 `hint` 留空时，页面只显示原始英文；填了就会显示在原始诊断上方。

---

## 一、一条规则长什么样

```json
{
  "id": "missing-semicolon",
  "match": "expected ';' at end of declaration",
  "category": "syntax",
  "example": "main.cpp:1:22: error: expected ';' at end of declaration",
  "human": "",
  "hint": ""
}
```

| 字段 | 用途 |
| --- | --- |
| `id` | 稳定标识，只用于维护，不显示 |
| `match` | 正则，匹配诊断正文（不含 `main.cpp:1:22:` 和 `error:` 前缀） |
| `category` | 语法 / 名字与作用域 / 类型 / 链接 / 运行时限制，仅用于归类 |
| `example` | 真实 clang 原文，方便以后回归时对照 |
| `human` | 你写的“人话”，建议 20~60 字 |
| `hint` | 一个可以立刻执行的动作，建议 ≤ 25 字 |

`match` 只匹配 `error:` / `warning:` 之后、去掉文件名与行列号的那段文字。所以不要写 `main.cpp`、不要写行号。

---

## 二、`human` 的三段式

> **现象**（哪里不对）→ **原因**（为什么）→ **出路**（怎么办）

示例：

> 这行末尾少了一个分号。C++ 用分号结束一条语句，就像句子末尾要画个句号。

- 现象：少了分号
- 原因：C++ 的语法要求
- 出路：补一个分号（hint 里给具体位置）

不是每条都要凑满三段。短的可以只写现象 + 出路：

> 编译器不认识 `x`。要么拼错了，要么它在使用之前还没声明。

## 三、`hint` 只做一件事

`hint` 是给“已经知道哪里错、但不知道改哪”的人看的。写一个动作，不解释原理：

- 好：`看看 return 前面，是不是漏了分号。`
- 好：`在文件开头加上 #include <iostream>。`
- 差：`分号是 C++ 中用来结束语句的符号，你需要在合适的位置补上它。`（这是 human 该说的话）

## 四、语气规则

1. **第二人称**：用“你”，不用“用户”“考生”。
2. **先安抚，再指路**：新手看到红色报错会紧张，第一句不要追责。
3. **不嘲讽代码，可以调侃编译器**：可以写“编译器在这里卡住了”，不要写“这么简单的错也犯”。
4. **少用术语**；非用不可时，第一次出现加一句解释，例如“链接器（把多个 .o 拼成可执行文件的那一步）”。
5. **中文标点**，代码标识符与英文术语保持原样（`std::vector`、`const`）。
6. **长度克制**：human 超过 80 字就开始像教科书；真需要展开，挪到题目说明里。
7. **同一类错误保持同一称呼**：今天叫“声明”，明天别叫“定义”。

## 五、常见诊断的写法示例

下面这些可以直接改成你喜欢的风格，重点是结构。

| clang 原文（节选） | 建议 human | 建议 hint |
| --- | --- | --- |
| `expected ';' at end of declaration` | 这行末尾少了一个分号。C++ 用分号收尾一条语句，就像句子末尾要画句号。 | 看看上一行行尾，或者 `return` 前面。 |
| `use of undeclared identifier 'x'` | 编译器不认识 `x`。可能是拼错了，也可能是它在这行之前还没被声明。 | 检查拼写，并确认 `x` 已经定义或包含在参数里。 |
| `no member named 'cout' in namespace 'std'` | `std` 里没有 `cout` —— 它住在 `<iostream>` 里，而且完整名字是 `std::cout`。 | 在文件开头加 `#include <iostream>`。 |
| `expected expression` | 这里少了一个“可以算的东西”。通常是运算符后面空着，或者多写了一个符号。 | 检查 `=` 或 `return` 后面是不是漏了值。 |
| `expected '}' at end of input` | 花括号没有配对：某个 `{` 一直在等它的 `}`，直到文件结束都没等到。 | 数一数 `{` 和 `}`，或折叠代码找没闭合的那一块。 |
| `redefinition of 'x'` | `x` 被定义了两遍。同一作用域里，一个名字只能有一份定义。 | 给第二个 `x` 换个名字，或者删掉重复的定义。 |
| `no matching member function for call to 'append'` | `append` 的参数对不上：它想要字符或字符串，你可能塞了个数字进去。 | 确认括号里传的是 `char`、`const char*` 或 `std::string`。 |
| `invalid operands to binary expression ('std::string' … and 'int')` | 字符串和整数不能直接相加。C++ 不会偷偷把 `1` 变成 `"1"`。 | 先用 `std::to_string(n)` 转成字符串再拼。 |
| `cannot initialize a variable of type 'int' with an lvalue of type 'const char[3]'` | `"abc"` 是字符串，装不进 `int`。你可能想写的是字符 `'a'`，或者一个整数。 | 把双引号换成单引号，或者把变量类型改成 `std::string`。 |
| `wasm-ld: error: main.o: undefined symbol: f()` | 链接器找不到 `f` 的身体：代码里只见过它的声明，没有见过实现。 | 检查这个函数是不是真的写了实现，名字和参数是否一致。 |
| `cannot use 'throw' with exceptions disabled` | 这个在线编译器没有打开 C++ 异常，`throw` / `try` 暂时跑不了。 | 先用返回错误码或 `std::optional` 代替；异常题请在本地运行。 |
| `use of undeclared identifier 'endl'` | `endl` 需要 `std::` 前缀，而且得先包含 `<iostream>`。 | 写成 `std::endl`，并检查头文件。 |

## 六、写 `match` 的几个经验

1. **挑最独特的一小段**：`expected ';' at end of declaration` 比 `expected` 稳。
2. **不要把行列号、文件名写进去**：它们每次都会变。
3. **引号里的名字用通配**：`undeclared identifier '(\w+)'`，但别急着捕获——显示上用不到。
4. **先具体、后一般**：数组里靠前的规则先匹配。`no member named 'cout'` 要排在泛化的 `no member named` 前面。
5. **小心换行与空白**：诊断可能折行，用 `\s+` 代替空格。
6. **一条规则只负责一种错误**：同一个 `no matching function` 可能对应十种原因，拆开写比一条大规则好维护。

## 七、工作流

1. 在测试页写一段会触发错误的代码，运行，点开原始诊断。
2. 复制 `error:` 后面的那句英文，粘到对应规则的 `example` 里。
3. 从这句话里挑一段独特文字，写成 `match`（用正则校验工具试一下）。
4. 按三段式写 `human`，再写一个动作型 `hint`。
5. `node js/build.js` 后在浏览器里重跑，确认文案出现、且没有匹配到别的错误。

## 八、分类与排序

`category` 只是给你自己维护用的，不显示在页面上。建议按“新手遇到频率”排序规则：

1. 语法（分号、括号、关键字拼写）
2. 名字与作用域（未声明、重定义、成员不存在）
3. 类型（初始化、参数不匹配、隐式转换）
4. 链接（undefined symbol、重复定义）
5. 运行时限制（异常禁用、超时、内存）

---

写完 10 条以后，你会发现它们自然形成一种口吻。先别追求一次写满 27 条，挑最常被触发的 5 条开始，剩下的可以慢慢补。
