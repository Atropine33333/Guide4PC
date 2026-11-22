#include <string>
#include <vector>

std::vector<std::string> split_fields(const std::string& text, char delimiter) {
    (void)delimiter;
    // TODO: 空串返回空 vector；连续分隔符要产生空字段。
    if (text.empty()) return {};
    return {text};
}

std::string join_fields(const std::vector<std::string>& fields,
                        const std::string& separator) {
    (void)separator;
    if (fields.empty()) return "";
    // TODO: 用分隔符连接所有字段，空字段也要保留。
    return fields[0];
}
