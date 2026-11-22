深夜电视，灯光一亮。

“看这里，朋友！一颗像素，三路分量——红、绿、蓝，各走各的道！输入给的数超了 `255`？不要紧，我们替您截到 `0..255`，多一分不要，少一分不给！

“平均亮度？三路相加，整数一除，齐活！三路一齐归零？那是纯黑，一眼就认得出！

“还有呢：构造要截，赋值也要截，绝不放野数值进画面！这样一颗像素，别处要价十九块九毛九；今天，只要您把它写出来！

“等等——还有！`set_red`、`set_green`、`set_blue`，三个设定器，随叫随到！”

**人话**：实现 RGB 像素类 `Pixel`：构造与三个 setter 都把分量截断到 `0..255`；`red()`、`green()`、`blue()` 读分量；`brightness()` 返回三分量之和整除 3；`is_black()` 仅在三分量全为 0 时返回 `true`。

```cpp
class Pixel {
public:
    Pixel(int red, int green, int blue);
    int red() const;
    int green() const;
    int blue() const;
    void set_red(int red);
    void set_green(int green);
    void set_blue(int blue);
    int brightness() const;  // (red + green + blue) / 3，整数除法
    bool is_black() const;   // 三个分量是否都为 0
};
```

### 要求

- 三个分量在构造和赋值时都要截断到 `0..255` 闭区间；输入可能超出这个范围；
- `brightness()` 使用整数除法，结果仍然在 `0..255`；
- `is_black()` 只在三个分量全为 0 时返回 `true`。

不要写 `main`。
