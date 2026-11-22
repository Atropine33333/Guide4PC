// 参考实现：oop-mutable-cache
#include <cctype>
#include <cstddef>
#include <string>
#include <utility>

class TextStats {
public:
    explicit TextStats(std::string text) : text_(std::move(text)) {}

    void append(const std::string& extra) {
        text_ += extra;
        word_cache_ = -1;
        letter_cache_ = -1;
    }

    int word_count() const {
        if (word_cache_ < 0) {
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
            word_cache_ = count;
            ++computations_;
        }
        return word_cache_;
    }

    int letter_count() const {
        if (letter_cache_ < 0) {
            int count = 0;
            for (char ch : text_) {
                if (std::isalpha(static_cast<unsigned char>(ch))) ++count;
            }
            letter_cache_ = count;
            ++computations_;
        }
        return letter_cache_;
    }

    static int computations() { return computations_; }

private:
    std::string text_;
    mutable int word_cache_ = -1;
    mutable int letter_cache_ = -1;
    inline static int computations_ = 0;
};
