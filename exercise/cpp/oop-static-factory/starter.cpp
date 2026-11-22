#include <optional>
#include <string>

class Username {
public:
    static std::optional<Username> create(const std::string& text) {
        (void)text;
        // TODO: 没有做任何校验。
        return std::nullopt;
    }


    const std::string& value() const { return text_; }

    std::size_t length() const { return text_.size(); }

    bool operator==(const Username& other) const {
        return text_ == other.text_;
    }

private:
    explicit Username(std::string text) : text_(std::move(text)) {}
    std::string text_;
};
