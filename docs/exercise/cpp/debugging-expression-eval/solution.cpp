#include <string>

namespace {

class Parser {
public:
    explicit Parser(const std::string& text) : text_(text), pos_(0) {}

    int parse() { return parse_expression(); }

private:
    void skip_spaces() {
        while (pos_ < text_.size() && text_[pos_] == ' ') {
            ++pos_;
        }
    }

    int parse_expression() {
        int value = parse_term();
        for (;;) {
            skip_spaces();
            if (pos_ < text_.size() &&
                (text_[pos_] == '+' || text_[pos_] == '-')) {
                const char op = text_[pos_++];
                const int right = parse_term();
                value = (op == '+') ? value + right : value - right;
            } else {
                return value;
            }
        }
    }

    int parse_term() {
        int value = parse_factor();
        for (;;) {
            skip_spaces();
            if (pos_ < text_.size() && text_[pos_] == '*') {
                ++pos_;
                value *= parse_factor();
            } else {
                return value;
            }
        }
    }

    int parse_factor() {
        skip_spaces();
        if (pos_ < text_.size() && text_[pos_] == '(') {
            ++pos_;
            const int value = parse_expression();
            skip_spaces();
            if (pos_ < text_.size() && text_[pos_] == ')') {
                ++pos_;
            }
            return value;
        }
        if (pos_ < text_.size() && text_[pos_] == '-') {
            ++pos_;
            return -parse_factor();
        }
        if (pos_ < text_.size() && text_[pos_] >= '0' && text_[pos_] <= '9') {
            int value = 0;
            while (pos_ < text_.size() && text_[pos_] >= '0' &&
                   text_[pos_] <= '9') {
                value = value * 10 + (text_[pos_] - '0');
                ++pos_;
            }
            return value;
        }
        return 0;
    }

    const std::string& text_;
    std::size_t pos_;
};

}  // namespace

int eval_expression(const std::string& text) {
    Parser parser(text);
    return parser.parse();
}
