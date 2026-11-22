#include <string>
#include <string_view>

std::string_view trim(std::string_view text) {
    // TODO: 去掉首尾空格，返回指向原缓冲区的视图。
    return text;
}

std::string_view first_word(std::string_view text) {
    // TODO: 返回第一个单词。
    (void)text;
    return {};
}

std::string_view between(std::string_view text, char open, char close) {
    // TODO: 返回 open 与 close 之间的内容。
    (void)text;
    (void)open;
    (void)close;
    return {};
}

bool starts_with(std::string_view text, std::string_view prefix) {
    // TODO: 判断前缀。
    (void)text;
    (void)prefix;
    return false;
}
