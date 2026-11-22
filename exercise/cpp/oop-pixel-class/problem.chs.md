这一题不读 stdin，也不写 stdout。实现一个 RGB 像素类，测试在隐藏的 harness 里。

实现：

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

要求：

- 三个分量在构造和赋值时都要截断到 `0..255` 闭区间；输入可能超出这个范围；
- `brightness()` 使用整数除法，结果仍然在 `0..255`；
- `is_black()` 只在三个分量全为 0 时返回 `true`。

不要写 `main`。
