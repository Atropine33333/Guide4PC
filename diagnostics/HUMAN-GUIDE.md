# 诊断文案写作规范（v2）

这份文档写给写文案的人，也给校验脚本看。目标：同一条编译器诊断，在中文 / English / 日本語 下都能被写成「人话」，并且可以机器检查。

---

## 1. 文案存在哪里

```
diagnostics/
  rules/cpp/
    syntax.json     语法
    naming.json     名字与作用域
    types.json      类型
    link.json       链接
    runtime.json    运行时限制
  schema.json       规则格式（编辑器校验用）
  HUMAN-GUIDE.md    本文件
  catalog.json      生成物：node tools/diag.mjs build 产出，不要手改
```

一条规则长这样：

```json
{
  "id": "missing-semicolon",
  "category": "语法",
  "priority": 50,
  "when": {
    "all": [
      { "field": "severity", "in": ["error", "fatal error"] },
      { "field": "message", "startsWith": "expected ';'" }
    ]
  },
  "capture": {
    "token": { "field": "message", "regex": "expected '([^']+)'" }
  },
  "human": {
    "chs": "杂鱼，这行末尾的分号又没写？",
    "en": "",
    "ja": ""
  },
  "hint": {
    "chs": "回头看看上一行末尾啦~ return 和赋值后面尤其要查哦。",
    "en": "",
    "ja": ""
  },
  "example": "expected ';' at end of declaration",
  "tests": [
    { "message": "expected ';' at end of declaration", "expect": true },
    { "message": "expected expression", "expect": false }
  ]
}
```

---

## 2. 三语字段

`human` 和 `hint` 都是对象，固定三个键：

| 键 | 语言 | 说明 |
| --- | --- | --- |
| `chs` | 简体中文 | 主文案 |
| `en` | English | 英文 |
| `ja` | 日本語 | 日文 |

规则：

1. **至少填一个**；三个都填是目标状态。
2. 没写的语言填 `""`，不要省略键，不要写 `null`。
3. 展示时的回退顺序：当前语言 → `en` → `chs`。
4. 三种语言可以语气不同，但**事实必须一致**：不能中文说「少分号」、日文说「少括号」。
5. 不要写 HTML、Markdown、颜色、emoji 过渡动画之类；渲染器只按纯文本显示。
6. 换行用 `\n`，不要用 `<br>`。

### 长度上限（check 会警告）

| 字段 | 中文 | English | 日本語 |
| --- | --- | --- | --- |
| `human` | ≤ 120 字 | ≤ 220 字符 | ≤ 180 字符 |
| `hint` | ≤ 70 字 | ≤ 130 字符 | ≤ 110 字符 |

超了不禁止，但会提示：卡片里放不下，会被截断。

---

## 3. 占位符

文案里可以插值，形式是 `{名字}`。可用变量：

| 变量 | 含义 |
| --- | --- |
| `{line}` / `{column}` | 行号 / 列号 |
| `{file}` | 文件名 |
| `{severity}` | error / warning |
| `{token}` 等 | `capture` 里定义的名字 |

规则：

1. **同一规则的所有语言，占位符集合必须完全一致**；check 会逐语言比对，少一个就报错。
2. 原文里真的要写大括号时用 `{{` 和 `}}`。
3. 占位符只能出现在 capture 真的可能产出的地方；没有捕获时渲染为空字符串。
4. 文案不要自己编变量名，未定义的 `{foo}` 会被 check 判错。

示例：

```json
"human": {
  "chs": "标识符 `{name}` 没有声明过。",
  "en": "`{name}` has not been declared.",
  "ja": "`{name}` は宣言されていません。"
}
```

（反引号只是这里为了可读，正文里不要加；渲染器不处理 Markdown。）

---

## 4. 语气与写法

### 共通

- 先说**发生了什么**，再说**为什么**，最后给**一个动作**。
- 用第二人称：你 / you / あなた。
- 具体到这一行、这个词；不要泛泛说「代码有语法错误」。
- 可以俏皮（中文「杂鱼 / 大哥哥」、日语「ざぁこ / おにいさん」），但不要羞辱到让人不想再写代码；玩笑落在「编译器」身上，不落在人身上。
- 不要重复编译器原句；`human` 是翻译，`hint` 是下一步。
- 一条诊断只讲一个问题；不要把「少分号」和「缩进」混在一起。

### 中文

- 口语，短句，允许语气词。
- 避免「请」「您」「错误：」这种公文体。

### English

- 自然口语，不要直译中文梗；可以俏皮，但别用中英混杂。
- 避免 "Error:" 开头，避免全大写。

### 日本語（メスガキ口吻）

- 角色：雌小鬼。嘴臭、居高临下、爱看用户出丑，但会用施舍的语气把正确做法讲出来。
- 称呼：自称「あたし」，叫用户「おにいさん」（偶尔「あんた」）。
- 骂人上限：「ざぁこ」「ざこ」「よわよわ」「へなちょこ」「つちくれ」「ばか」「あほ」；不要越过「ざぁこ♡」的强度。
- 语尾与口头禅：〜、…、はぁ？、ふーん、えー、ちょっとぉ、しょうがないなぁ、特別に、べつに、教えてあげない；每条最多一个 ♡ 或 ～，不要堆颜文字。
- 不要方言，不要（笑）（怒）这类括号动作，不要机翻语序，不要直译中文梗（不必每句都硬塞「雑魚」）。
- `hint` 必须留下可执行动作，骨架轮换：傲娇拒绝 / 假好心 / 反问 / 威胁 / 半截收住 / 条件交换 / 直接命令 / 温柔陷阱；同一骨架不超过 4 条。
- 双关（掛け詞）每条最多一处，不能影响理解：セミコロン＝「締め」、波括弧＝「開きっぱなし」、return＝「帰る」、throw＝「投げる」、catch＝「受け止める」。
- 技术词用常用写法：`セミコロン`、`丸括弧`、`波括弧`、`ヘッダ`、`名前空間`、`変数`、`型`、`参照`、`ポインタ`、`オーバーロード`、`コンストラクタ`、`リンカ`、`例外`、`未定義`。
- 例：
  - chs：「这行末尾少了个分号。」
  - en："This line is missing a semicolon at the end."
  - ja：「おにいさん、またセミコロン忘れてる〜。文の終わりはちゃんと締めてって言ったよね？ ざぁこ♡」

---

## 5. 匹配规则（`when`）

`when` 是一棵谓词树，节点只有三种：

```json
{ "all": [ ... ] }
{ "any": [ ... ] }
{ "not": { ... } }
```

叶子是 `{ "field": "...", "op": "...", "value": ... }`，`op` 可以省略，默认 `contains`：

| 字段 | 说明 |
| --- | --- |
| `severity` | error / warning / fatal error |
| `message` | 诊断正文（不含文件名和行列号） |
| `file` | 文件名 |
| `line` / `column` | 数字 |
| `sourceLine` | 出错行源码（如果运行时提供） |
| `lang` | cpp / py |

| 操作符 | 值 | 说明 |
| --- | --- | --- |
| `contains` | string | 子串 |
| `startsWith` / `endsWith` | string | 前缀 / 后缀 |
| `equals` | string | 全等 |
| `in` | array | 枚举 |
| `regex` | string | 兜底时才用；正则不能带 flags，内部按 `i` 处理 |
| `present` | — | 字段存在且非空 |

选择逻辑：

1. 所有规则都算出「是否命中」；命中多条时取 **priority 高**的，再取 **叶子谓词多（更具体）** 的，最后按文件里的顺序。
2. `priority` 默认 0；越具体的规则可以给 10–100。
3. 不要写「万能规则」；`{ "field": "message", "regex": ".*" }` 会把所有诊断吃掉。

### capture

`capture` 是名字到 `{field, regex}` 的映射；正则建议用**命名之外**的捕获组，取第 1 组：

```json
"capture": { "name": { "field": "message", "regex": "undeclared identifier '([^']+)'" } }
```

---

## 6. 测试（必填）

每条规则至少一条正例；建议再加一条反例：

```json
"tests": [
  { "message": "expected ';' at end of declaration", "expect": true },
  { "message": "expected expression", "expect": false }
]
```

`node tools/diag.mjs check` 会：

- 校验 schema 结构；
- 逐条跑 `tests`；
- 检查三种语言的占位符是否一致；
- 检查长度上限；
- 报告 `ja` / `en` 未填写的条数（不阻止构建，方便知道还差多少）。

`node tools/diag.mjs explain "expected ';' at end of declaration"` 会打印命中的规则、命中了哪些叶子、以及最终渲染的三语文案，用来排查「为什么提示这条」。

---

## 7. 写一条新规则的流程

1. 在对应类别的 `rules/cpp/*.json` 里加一条；`id` 用小写短横线，全局唯一。
2. 先写 `example`，把真实编译器输出里的 `error: ` 前缀去掉后粘进去。
3. 用 `when` 表达它，尽量不用 `regex`。
4. 写 `tests`（正例=example，反例挑一条相近但不同的）。
5. 填 `human` / `hint`（至少 chs）。
6. 跑 `node tools/diag.mjs check`。
7. 跑 `node tools/diag.mjs build` 生成 `catalog.json`，再构建站点。

---

## 8. 校验清单（提交前）

- [ ] `node tools/diag.mjs check` 通过（0 error）
- [ ] 三语占位符一致
- [ ] 每条规则至少 1 条正例测试
- [ ] `id` 唯一、小写短横线
- [ ] `catalog.json` 已重新生成
- [ ] 卡片里实际显示一遍（本地 `node js/build.js --force` 后看练习页）
