#include <string>

class AnyValue {
public:
    AnyValue() = default;
    AnyValue(int value) { (void)value; }
    AnyValue(const std::string& value) { (void)value; }
    AnyValue(const AnyValue& other) = default;
    AnyValue& operator=(const AnyValue& other) = default;
    ~AnyValue() = default;

    bool empty() const { return true; }  // TODO

    bool is_int() const { return false; }  // TODO

    bool is_string() const { return false; }  // TODO

    int as_int(int fallback) const {
        // TODO
        return fallback;
    }

    std::string as_string(const std::string& fallback) const {
        // TODO
        return fallback;
    }

    std::string describe() const {
        // TODO
        return "empty";
    }
};
