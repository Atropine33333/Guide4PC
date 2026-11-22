#include <string>
#include <vector>

std::string csv_quote(const std::string& field) {
    // TODO: 含逗号、引号或换行的字段需要加引号，内部引号要双写。
    return field;
}

std::vector<std::string> csv_split(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    for (char c : line) {
        // TODO: 引号内的逗号不是分隔符，双写引号要还原成一个。
        if (c == ',') {
            fields.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    fields.push_back(current);
    return fields;
}
