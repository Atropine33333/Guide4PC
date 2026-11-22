# 题目编写指南

每一道题是一个目录，放在 `exercise/cpp/<id>/` 下（以后 C、Python 用 `exercise/c/`、`exercise/python/`）。
构建脚本 `node js/build.js` 会扫描并校验这些目录，把结果发布到 `docs/exercise/` 和 `docs/ex/`。

## 目录结构

```
exercise/cpp/<id>/
  spec.json          # 元数据 + 测试（必填）
  problem.chs.md     # 中文题面（必填）
  problem.en.md      # 英文题面（必填）
  starter.cpp        # 学生拿到的初始代码（必填，必须能编译）
  solution.cpp       # 参考实现（必填，必须全过）
  harness.cpp        # 可选：harness 题的测试入口
  starter-<variant>.cpp  # 可选：变体的 starter
```

`<id>` 用小写短横线，例如 `count-digit-occurrences`，必须和目录名一致。

## spec.json

```json
{
  "$schema": "../../schema.json",
  "id": "count-digit-occurrences",
  "lang": "cpp",
  "category": "syntax",
  "rank": 1,
  "title": { "chs": "数字出现计数", "en": "Count Digit Occurrences" },
  "limits": { "timeoutMs": 5000, "outputBytes": 65536 },
  "checks": [
    {
      "id": "basic",
      "label": { "chs": "普通正整数", "en": "Ordinary positive integer" },
      "cases": [
        { "in": ["676767", "6"], "out": ["3"] },
        { "in": "0\n0\n", "out": "1\n" }
      ]
    }
  ]
}
```

- `category`：`syntax`、`semantics`、`types`、`stdlib`、`io`、`algorithm`、`data`、`memory`、`oop`、`templates`、`engineering`、`debugging`。
- `rank`：1–5，1 入门、2 基础、3 进阶、4 综合、5 挑战（同一分类内的相对难度）。
- `checks`：至少一条；`cases` 的 `in`/`out` 可以是多行字符串，也可以是行数组（数组更省转义）。
- 程序输出按行精确比较（忽略行尾空白）。评分题的输出尽量只用数字或 ASCII，避免中英两套期望。
- `harness`：写 `"harness": "harness.cpp"` 表示 harness 题，此时 checks 只保留 `id` 和 `label`（不写 cases）。

### harness 题

学生只写函数/类，不写 `main`；`harness.cpp` 会被拼在学生代码之后并提供 `main`。
harness 里每检查一项就输出一行：

```
@@G4PC {"t":"check","id":"empty-size","ok":true}
```

最后输出：

```
@@G4PC {"t":"done","passed":5,"total":7}
```

有失败时 `main` 返回 1。检查项 id 用短横线，覆盖：正常路径、边界（空/满/0/1）、状态不变量、异常释放（不要用 C++ 异常，只能检查析构是否释放资源等可观测行为）。
随机测试必须用固定种子，失败信息里能复现输入。

## 题面

- 题面从第一段正文直接开始；卡片标题取自 `spec.title`，不要在 md 里重复写 H1。
- 依次写：题意、输入格式、输出格式、示例、约束。工程题写清楚要实现的接口（函数签名/类成员）。
- 中文和英文题面内容要一致；不要写 `:::exercise` 块，练习页会自动挂载编辑器与测试。
- 变体（例如“测试机没有 `<string>`”）在 spec 的 `variants` 里声明，题面里用一句“切到对应变体”即可。

## 质量要求

- 原创题面与数据；可以参考洛谷等题库的**题型**，不要抄题面、样例或数据。
- starter 必须能编译，且明显不完整（学生会看到“运行失败”）。
- solution 必须通过全部 checks。
- 至少 3 条 check，必须包含边界情况；不要依赖未定义行为、时间/随机（除非固定种子）、文件、网络、线程。
- 单文件、无第三方库；只用标准库（`<iostream>`、`<string>`、`<vector>` 等）。
- 不支持 C++ 异常（编译器关闭了异常），题面与 harness 都不要用 `throw`/`try`。
- 输出不要有多余空格；错误信息不要混进 stdout。

## 自检

```bash
node js/build.js
```

构建会校验 spec 的必填字段、文件名、id/目录一致、starter/solution 是否存在，并生成 `docs/exercise/index.json`。
如果出现 `[exercise] ... skipped` 的告警，按提示修好再提交。
