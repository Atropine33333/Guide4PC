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
    (void)text;
    // TODO: 追加字符串并返回 *this。
    return *this;
}

TextBuilder& TextBuilder::append(char c) {
    (void)c;
    // TODO: 追加一个字符并返回 *this。
    return *this;
}

TextBuilder& TextBuilder::operator+=(const std::string& text) {
    (void)text;
    // TODO: 复用 append。
    return *this;
}

TextBuilder TextBuilder::operator+(const std::string& text) const {
    (void)text;
    // TODO: 返回新对象，原对象不变。
    return *this;
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
    // TODO: 清空文本。
}

char& TextBuilder::operator[](int index) {
    return text_[static_cast<std::size_t>(index)];
}

const char& TextBuilder::operator[](int index) const {
    return text_[static_cast<std::size_t>(index)];
}

bool TextBuilder::operator==(const TextBuilder& other) const {
    (void)other;
    // TODO: 比较文本内容。
    return false;
}

bool TextBuilder::operator<(const TextBuilder& other) const {
    (void)other;
    // TODO: 字典序比较。
    return false;
}

std::ostream& operator<<(std::ostream& os, const TextBuilder& builder) {
    (void)builder;
    // TODO: 输出内部文本。
    return os;
}
