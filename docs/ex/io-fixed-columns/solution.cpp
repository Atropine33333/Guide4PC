#include <algorithm>
#include <charconv>
#include <string>

struct Person {
    std::string name;
    int id;
    int score;
};

std::string slice(const std::string& line, int start, int width) {
    if (start < 0 || width <= 0) {
        return {};
    }
    const std::size_t begin = static_cast<std::size_t>(start);
    if (begin >= line.size()) {
        return {};
    }
    const std::size_t end =
        std::min(line.size(), begin + static_cast<std::size_t>(width));
    std::size_t first = begin;
    std::size_t last = end;
    while (first < last && line[first] == ' ') {
        ++first;
    }
    while (last > first && line[last - 1] == ' ') {
        --last;
    }
    return line.substr(first, last - first);
}

namespace {

int parse_int_field(const std::string& text) {
    if (text.empty()) {
        return 0;
    }
    int value = 0;
    const char* begin = text.data();
    const char* end = text.data() + text.size();
    const auto result = std::from_chars(begin, end, value);
    return result.ec == std::errc() ? value : 0;
}

}  // namespace

Person parse_person(const std::string& line) {
    Person person;
    person.id = parse_int_field(slice(line, 0, 4));
    person.name = slice(line, 4, 10);
    person.score = parse_int_field(slice(line, 14, 6));
    return person;
}
