《形状仓库值夜规则》

你被雇来看守一间形状仓库。以下规则全部为真，只是常识在这里未必管用，请照办。

一、形状凭 `Shape*` 进出；删除时也只经 `Shape*`，不得向下转型。

二、`Shape` 的析构函数必须为虚函数。若它不虚，通过 `Shape*` 删除派生对象时派生类的析构函数不会被调用——标准把这种情况写作未定义行为，本仓库写作“东西会不见”。

三、若你让基类析构函数保持非虚，第四条以下的规则一律作废，因为程序可能根本走不到那里。

四、以下各条与上方规则同效，逐条为真：`Rect::label()` 报 `"rect"`，面积 `width * height`；`Circle::label()` 报 `"circle"`，面积 `π * radius * radius`，`π` 取 `3.14159265358979323846`；尺寸为 `0` 时面积为 `0`。

五、用 `Shape*` 删除派生类对象时，派生析构函数必须被调用。

六、不得写 `main`。

**人话**：实现形状类层次：`Shape` 有纯虚函数 `std::string label() const`、`double area() const`，析构函数必须是虚函数；`Rect(width, height)` 报 `"rect"`、面积 `width * height`；`Circle(radius)` 报 `"circle"`、面积 `π * radius * radius`。

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

### 约定

- `Rect::label()` 返回 `"rect"`，面积是 `width * height`。
- `Circle::label()` 返回 `"circle"`，面积是 `π * radius * radius`，其中 `π` 取 `3.14159265358979323846`。
- 尺寸可以为 `0`，此时面积为 `0`。
- `Shape` 的析构函数必须是 `virtual`（或 `= default` 的虚函数）；用 `Shape*` 删除派生类对象时，派生析构函数必须被调用。
- 不要写 `main`。

### 提示

starter 里的 `~Shape()` 尚未声明为虚函数，把它改成 `virtual ~Shape() = default;`。派生类的析构函数若要写 `override`，基类析构函数须先成为虚函数。
