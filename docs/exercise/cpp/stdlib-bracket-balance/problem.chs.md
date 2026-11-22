这一题不读 stdin，也不写 stdout。实现一个括号匹配判断函数，测试在隐藏的 harness 里。

实现：

- `bool balanced(const std::string& text)`：当且仅当 `text` 中的圆括号、方括号和花括号都按正确的嵌套顺序成对出现时返回 `true`。

`text` 只包含 `(`、`)`、`[`、`]`、`{`、`}` 六种字符，可能为空。不要写 `main`。

提示：遇到左括号就压栈；遇到右括号时检查栈顶是否为对应类型，最后栈必须为空。可以使用 `std::vector<char>` 或 `std::stack<char>`。
