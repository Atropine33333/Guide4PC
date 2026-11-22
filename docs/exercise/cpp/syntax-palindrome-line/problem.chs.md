这一题不读 stdin，也不写 stdout。实现一个判断回文的函数，测试在隐藏的 harness 里。

```cpp
bool is_palindrome(const std::string& text);
```

规则：

- 只考虑 ASCII 字母和数字，其他字符（空格、标点、制表符）全部跳过；
- 比较字母时忽略大小写；
- 空串，或者没有任何字母和数字的串，视为回文；
- 不要写 `main`。

### 提示

用两个下标从两端向中间扫描，遇到非字母数字字符就跳过。判断字符类别时记得把 `char` 转成 `unsigned char` 再传给 `<cctype>` 的函数。
