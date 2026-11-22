THE CODE OF OVERLOAD RESOLUTION

Article 1 (Scope). Where several functions share one name, the compiler must choose one and use it. This Code governs that choice.

Article 2 (Candidates). Three versions only are enrolled here: `pick(int)`, `pick(long)`, and `pick(double)`. Any version whose parameter an argument can convert to joins the candidate set.

Article 3 (Exact Match). An argument whose type matches a parameter exactly is an exact match and stands above every conversion. Thus an `int` argument goes to `pick(int)`, a `long` argument to `pick(long)`, and a `double` argument to `pick(double)`.

Article 4 (Conversions). Where no exact match exists, standard conversions are permitted. Which conversion ranks above another is fixed by the C++ standard, and this Code does not restate it.

Article 5 (Reply). Each version replies with one word only: `pick(int)` returns `"int"`, `pick(long)` returns `"long"`, and `pick(double)` returns `"double"`.

Article 6 (Supplementary). Do not write `main`, and do not change the signatures.

**In plain words**: Implement three overloads named `pick`, taking `int`, `long`, and `double` respectively, each returning the string for its type. Arguments of different types must each select the corresponding overload; write the signatures as the standard requires and the choice takes care of itself.

```cpp
const char* pick(int value);
const char* pick(long value);
const char* pick(double value);
```
