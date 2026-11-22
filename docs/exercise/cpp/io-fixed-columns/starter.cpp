#include <string>

struct Person {
    std::string name;
    int id;
    int score;
};

std::string slice(const std::string& line, int start, int width) {
    // TODO: 按列截取并去掉首尾空格。
    (void)line;
    (void)start;
    (void)width;
    return {};
}

Person parse_person(const std::string& line) {
    // TODO: 按固定列布局解析三个字段。
    (void)line;
    return Person{};
}
