这一题不读 stdin。实现一个形状类层次，并通过基类指针正确销毁派生对象。

```cpp
#include <string>

class Shape {
public:
    virtual ~Shape() = default;  // 必须保持虚析构
    virtual std::string label() const = 0;
    virtual double area() const = 0;
};

class Rect : public Shape {
public:
    Rect(double width, double height);
    std::string label() const override;
    double area() const override;
};

class Circle : public Shape {
public:
    explicit Circle(double radius);
    std::string label() const override;
    double area() const override;
};
```

约定：

- `Rect::label()` 返回 `"rect"`，面积是 `width * height`。
- `Circle::label()` 返回 `"circle"`，面积是 `π * radius * radius`，其中 `π` 取 `3.14159265358979323846`。
- 尺寸可以为 `0`，此时面积为 `0`。
- `Shape` 的析构函数必须是 `virtual`（或 `= default` 的虚函数）；harness 会定义自己的派生类，并用 `Shape*` 删除它来检查派生析构函数是否被调用。
- 不要写 `main`。

### 提示

starter 里的 `~Shape()` 还不是虚函数，把它改成 `virtual ~Shape() = default;`。如果派生类的析构函数要写 `override`，基类析构函数必须先变成虚函数。
