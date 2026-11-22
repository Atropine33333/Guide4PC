#include <memory>
#include <string>
#include <utility>

class AnyValue {
public:
    AnyValue() = default;

    AnyValue(int value) : holder_(new IntHolder(value)) {}

    AnyValue(const std::string& value) : holder_(new StringHolder(value)) {}

    AnyValue(const AnyValue& other)
        : holder_(other.holder_ ? other.holder_->clone() : nullptr) {}

    AnyValue& operator=(const AnyValue& other) {
        if (this != &other) {
            std::unique_ptr<Base> next(other.holder_ ? other.holder_->clone()
                                                      : nullptr);
            holder_ = std::move(next);
        }
        return *this;
    }

    ~AnyValue() = default;

    bool empty() const { return holder_ == nullptr; }
    bool is_int() const {
        return holder_ != nullptr && holder_->kind() == Kind::kInt;
    }
    bool is_string() const {
        return holder_ != nullptr && holder_->kind() == Kind::kString;
    }

    int as_int(int fallback) const {
        return holder_ ? holder_->as_int(fallback) : fallback;
    }

    std::string as_string(const std::string& fallback) const {
        return holder_ ? holder_->as_string(fallback) : fallback;
    }

    std::string describe() const {
        return holder_ ? holder_->describe() : std::string("empty");
    }

private:
    enum class Kind { kInt, kString };

    struct Base {
        virtual ~Base() = default;
        virtual Base* clone() const = 0;
        virtual Kind kind() const = 0;
        virtual int as_int(int fallback) const = 0;
        virtual std::string as_string(const std::string& fallback) const = 0;
        virtual std::string describe() const = 0;
    };

    struct IntHolder : Base {
        explicit IntHolder(int value) : value(value) {}

        Base* clone() const override { return new IntHolder(value); }
        Kind kind() const override { return Kind::kInt; }
        int as_int(int) const override { return value; }
        std::string as_string(const std::string& fallback) const override {
            return fallback;
        }
        std::string describe() const override {
            return "int:" + std::to_string(value);
        }

        int value;
    };

    struct StringHolder : Base {
        explicit StringHolder(std::string value) : value(std::move(value)) {}

        Base* clone() const override { return new StringHolder(value); }
        Kind kind() const override { return Kind::kString; }
        int as_int(int fallback) const override { return fallback; }
        std::string as_string(const std::string&) const override { return value; }
        std::string describe() const override { return "string:" + value; }

        std::string value;
    };

    std::unique_ptr<Base> holder_;
};
