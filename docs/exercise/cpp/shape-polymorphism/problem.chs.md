这一题不读 stdin，也不写 stdout。实现一组图形类，测试在隐藏的 harness 里。

定义抽象基类 `Shape`，并实现 `Rectangle` 与 `Circle`：

- `Shape` 有纯虚函数 `double area() const` 和 `const char* name() const`，以及虚析构函数。
- `Rectangle(double width, double height)`：面积是 `width * height`，`name()` 返回 `"rectangle"`。
- `Circle(double radius)`：面积是 `π * radius * radius`，`name()` 返回 `"circle"`。请使用 `3.14159265358979323846` 作为 `π`。
- 两个派生类都通过 `public` 继承 `Shape`，并重写上述两个虚函数。

harness 会通过 `Shape*` 调用这些函数。面积比较使用相对误差 `1e-9`；名字必须逐字符完全一致。
