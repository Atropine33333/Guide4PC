#include <string>
#include <string_view>

std::string_view trim(std::string_view text) {
    std::size_t begin = 0;
    std::size_t end = text.size();
    while (begin < end && text[begin] == ' ') {
        ++begin;
    }
    while (end > begin && text[end - 1] == ' ') {
        --end;
    }
    return text.substr(begin, end - begin);
}

std::string_view first_word(std::string_view text) {
    std::size_t begin = 0;
    while (begin < text.size() && text[begin] == ' ') {
        ++begin;
    }
    std::size_t end = begin;
    while (end < text.size() && text[end] != ' ') {
        ++end;
    }
    return text.substr(begin, end - begin);
}

std::string_view between(std::string_view text, char open, char close) {
    const std::size_t opening = text.find(open);
    if (opening == std::string_view::npos) {
        return {};
    }
    const std::size_t closing = text.find(close, opening + 1);
    if (closing == std::string_view::npos) {
        return {};
    }
    return text.substr(opening + 1, closing - opening - 1);
}

bool starts_with(std::string_view text, std::string_view prefix) {
    return prefix.size() <= text.size() && text.substr(0, prefix.size()) == prefix;
}
