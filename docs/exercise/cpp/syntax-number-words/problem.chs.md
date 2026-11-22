“哦，我亲爱的，”老配音演员把剧本按在胸口，“观众要的是声音。您在银幕上打一串阿拉伯数字，那叫字幕，不叫台词。”

他清清嗓子：“四十二，您得念 `forty two`；一百，念 `one hundred`；一千零一，念 `one thousand one`。中间不许有 `and`，也不许有连字符——老天爷，这规矩是我入行那年立的。”

“整十的、十几的，各有各的名字，一个也不能念错。零，就念 `zero`。单词全小写，词与词之间一个空格，别多也别少。”

**人话**：读入一个整数 `n`，把它的英文写法输在一行里：全部小写，单词之间用一个空格分隔，不用 `and`，不用连字符。

**输入**：一行一个整数 `n`，满足 `0 <= n <= 999999`。

**输出**：一行，`n` 的英文写法。

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
