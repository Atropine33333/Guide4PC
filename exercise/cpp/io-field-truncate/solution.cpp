// Reference solution: io-field-truncate
#include <cstddef>
#include <string>

std::string truncate_field(const std::string& text, std::size_t width) {
    if (text.size() <= width) return text;
    return text.substr(0, width);
}

std::string center_field(const std::string& text, std::size_t width) {
    if (text.size() >= width) return truncate_field(text, width);
    const std::size_t padding = width - text.size();
    const std::size_t left = padding / 2;
    const std::size_t right = padding - left;
    return std::string(left, ' ') + text + std::string(right, ' ');
}
