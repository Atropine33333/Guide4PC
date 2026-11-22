// 参考实现：oop-static-factory
#include <optional>
#include <string>
#include <utility>

class Username {
public:
    static std::optional<Username> create(const std::string& text) {
        if (text.size() < 3 || text.size() > 16) return std::nullopt;

        const char first = text[0];
        const bool first_is_letter = (first >= 'a' && first <= 'z') ||
                                     (first >= 'A' && first <= 'Z');
        if (!first_is_letter) return std::nullopt;

        for (std::size_t i = 1; i < text.size(); ++i) {
            const char c = text[i];
            const bool allowed = (c >= 'a' && c <= 'z') ||
                                 (c >= 'A' && c <= 'Z') ||
                                 (c >= '0' && c <= '9') || c == '_';
            if (!allowed) return std::nullopt;
        }
        return Username(text);
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
