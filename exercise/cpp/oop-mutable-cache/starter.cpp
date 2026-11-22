#include <cctype>
#include <cstddef>
#include <string>

class TextStats {
public:
    explicit TextStats(std::string text) : text_(std::move(text)) {}

    void append(const std::string& extra) { text_ += extra; }

    int word_count() const {
        // TODO: 每次都重新计算，没有使用缓存。
        int count = 0;
        bool in_word = false;
        for (char ch : text_) {
            if (std::isalpha(static_cast<unsigned char>(ch))) {
                if (!in_word) {
                    ++count;
                    in_word = true;
                }
            } else {
                in_word = false;
            }
        }
        return count;
    }

    int letter_count() const {
        // TODO: 没有缓存，也没有维护 computations()。
        int count = 0;
        for (char ch : text_) {
            if (std::isalpha(static_cast<unsigned char>(ch))) ++count;
        }
        return count;
    }

    static int computations() { return 0; }

private:
    std::string text_;
};
