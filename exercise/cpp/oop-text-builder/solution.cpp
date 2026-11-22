// 参考实现：oop-text-builder
#include <ostream>
#include <string>

class TextBuilder {
public:
    TextBuilder();
    explicit TextBuilder(const std::string& initial);

    TextBuilder& append(const std::string& text);
    TextBuilder& append(char c);
    TextBuilder& operator+=(const std::string& text);
    TextBuilder operator+(const std::string& text) const;

    const std::string& str() const;
    int size() const;
    bool empty() const;
    void clear();

    char& operator[](int index);
    const char& operator[](int index) const;

    bool operator==(const TextBuilder& other) const;
    bool operator<(const TextBuilder& other) const;

private:
    std::string text_;
};

TextBuilder::TextBuilder() = default;

TextBuilder::TextBuilder(const std::string& initial) : text_(initial) {}

TextBuilder& TextBuilder::append(const std::string& text) {
    text_ += text;
    return *this;
}

TextBuilder& TextBuilder::append(char c) {
    text_ += c;
    return *this;
}

TextBuilder& TextBuilder::operator+=(const std::string& text) {
    text_ += text;
    return *this;
}

TextBuilder TextBuilder::operator+(const std::string& text) const {
    TextBuilder result(*this);
    result.append(text);
    return result;
}

const std::string& TextBuilder::str() const {
    return text_;
}

int TextBuilder::size() const {
    return static_cast<int>(text_.size());
}

bool TextBuilder::empty() const {
    return text_.empty();
}

void TextBuilder::clear() {
    text_.clear();
}

char& TextBuilder::operator[](int index) {
    return text_[static_cast<std::size_t>(index)];
}

const char& TextBuilder::operator[](int index) const {
    return text_[static_cast<std::size_t>(index)];
}

bool TextBuilder::operator==(const TextBuilder& other) const {
    return text_ == other.text_;
}

bool TextBuilder::operator<(const TextBuilder& other) const {
    return text_ < other.text_;
}

std::ostream& operator<<(std::ostream& os, const TextBuilder& builder) {
    return os << builder.str();
}
