读入一个整数，把它写成英文单词。单词全部小写，单词之间用一个空格分隔。

例如 `42` 写成 `forty two`，`100` 写成 `one hundred`，`1001` 写成 `one thousand one`。不要使用 `and`，也不要使用连字符。

### 输入

一行一个整数 `n`，满足 `0 <= n <= 999999`。

### 输出

一行，`n` 的英文写法。

### 示例

输入：

```text
123456
```

输出：

```text
one hundred twenty three thousand four hundred fifty six
```

### 约束

- `0` 输出 `zero`。
- `10` 到 `19` 使用 `ten`、`eleven`、`twelve`、`thirteen`、`fourteen`、`fifteen`、`sixteen`、`seventeen`、`eighteen`、`nineteen`。
- 整十使用 `twenty`、`thirty`、`forty`、`fifty`、`sixty`、`seventy`、`eighty`、`ninety`。
- 百位和千位之间、百位和个位之间都只有一个空格。
- 字面量 `100000` 是 `one hundred thousand`。
- 只使用标准库。
