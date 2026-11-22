// 参考实现：io-escape-expand
#include <string>

std::string expand_escapes(const std::string& input) {
    std::string result;
    result.reserve(input.size());

    std::size_t i = 0;
    while (i < input.size()) {
        if (input[i] != '\\') {
            result += input[i];
            ++i;
            continue;
        }
        if (i + 1 == input.size()) {
            result += '\\';
            ++i;
            continue;
        }

        const char next = input[i + 1];
        switch (next) {
            case 'n':
                result += '\n';
                break;
            case 't':
                result += '\t';
                break;
            case 'r':
                result += '\r';
                break;
            case '\\':
                result += '\\';
                break;
            default:
                result += '\\';
                result += next;
                break;
        }
        i += 2;
    }
    return result;
}
