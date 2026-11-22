// 参考实现：oop-traffic-light
enum class Light { Red, Yellow, Green };

class TrafficLight {
public:
    TrafficLight() = default;

    Light current() const {
        return current_;
    }

    void advance() {
        switch (current_) {
            case Light::Red: current_ = Light::Green; break;
            case Light::Green: current_ = Light::Yellow; break;
            case Light::Yellow: current_ = Light::Red; break;
        }
        ++ticks_;
    }

    void advance(int steps) {
        for (int i = 0; i < steps; ++i) advance();
    }

    int ticks() const {
        return ticks_;
    }

    static const char* name(Light light) {
        switch (light) {
            case Light::Red: return "Red";
            case Light::Green: return "Green";
            case Light::Yellow: return "Yellow";
        }
        return "Unknown";
    }

private:
    Light current_ = Light::Red;
    int ticks_ = 0;
};
