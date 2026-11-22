class Pixel {
public:
    Pixel(int red, int green, int blue) : red_(red), green_(green), blue_(blue) {}

    int red() const { return red_; }
    int green() const { return green_; }
    int blue() const { return blue_; }

    void set_red(int red) { red_ = red; }
    void set_green(int green) { green_ = green; }
    void set_blue(int blue) { blue_ = blue; }

    int brightness() const {
        // TODO: 先截断，再除以 3。
        return red_ + green_ + blue_;
    }

    bool is_black() const {
        // TODO: 三个分量都为 0 才是黑色。
        return red_ == 0;
    }

private:
    int red_ = 0;
    int green_ = 0;
    int blue_ = 0;
};
