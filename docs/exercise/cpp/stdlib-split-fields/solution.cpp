// 参考实现：stdlib-split-fields
#include <string>
#include <vector>

std::vector<std::string> split_fields(const std::string& text, char delimiter) {
    std::vector<std::string> fields;
    if (text.empty()) return fields;

    std::size_t start = 0;
    while (true) {
        const std::size_t position = text.find(delimiter, start);
        if (position == std::string::npos) {
            fields.push_back(text.substr(start));
            break;
        }
        fields.push_back(text.substr(start, position - start));
        start = position + 1;
    }
    return fields;
}

std::string join_fields(const std::vector<std::string>& fields,
                        const std::string& separator) {
    std::string result;
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (i != 0) result += separator;
        result += fields[i];
    }
    return result;
}
