《重载决议法典》

第一条（适用）凡同名而有数版者，编译器须择其一而用之。本法典专定择法。

第二条（候选）本题只列三版：`pick(int)`、`pick(long)`、`pick(double)`。凡实参可转换为某版形参者，皆入候选之列。

第三条（精确）实参类型与某版形参完全一致者，为精确匹配，其位在一切转换之上。故 `int` 实参归 `pick(int)`，`long` 实参归 `pick(long)`，`double` 实参归 `pick(double)`。

第四条（转换）无精确匹配时，允许标准转换。转换之间孰优孰劣，由 C++ 标准定之，本法典不复赘述。

第五条（回答）每版只回一词：`pick(int)` 回 `"int"`，`pick(long)` 回 `"long"`，`pick(double)` 回 `"double"`。

第六条（附则）不得写 `main`，不得改动签名。

**人话**：实现三个同名重载 `pick`，分别接受 `int`、`long`、`double`，各返回对应的类型名字符串。不同类型的实参应当各自选中对应的重载版本；照着标准写签名即可。

```cpp
const char* pick(int value);
const char* pick(long value);
const char* pick(double value);
```
