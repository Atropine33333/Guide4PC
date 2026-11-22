enum class Light { Red, Yellow, Green };

class TrafficLight {
public:
    TrafficLight() = default;

    Light current() const {
        // TODO: 颜色永远是红灯。
        return Light::Red;
    }

    void advance() {
        // TODO: 只增加计数，没有切换颜色。
        ++ticks_;
    }

    void advance(int steps) {
        ticks_ += steps;
    }

    int ticks() const {
        return ticks_;
    }

    static const char* name(Light light) {
        (void)light;
        // TODO: 没有区分颜色。
        return "Red";
    }

private:
    int ticks_ = 0;
};
