// 参考实现：oop-pixel-class
class Pixel {
public:
    Pixel(int red, int green, int blue)
        : red_(clamp(red)), green_(clamp(green)), blue_(clamp(blue)) {}

    int red() const { return red_; }
    int green() const { return green_; }
    int blue() const { return blue_; }

    void set_red(int red) { red_ = clamp(red); }
    void set_green(int green) { green_ = clamp(green); }
    void set_blue(int blue) { blue_ = clamp(blue); }

    int brightness() const { return (red_ + green_ + blue_) / 3; }

    bool is_black() const { return red_ == 0 && green_ == 0 && blue_ == 0; }

private:
    static int clamp(int value) {
        if (value < 0) return 0;
        if (value > 255) return 255;
        return value;
    }

    int red_ = 0;
    int green_ = 0;
    int blue_ = 0;
};
