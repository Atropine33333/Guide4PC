// 参考实现：templates-generic-join
#include <sstream>
#include <string>

template <typename Container>
std::string join(const Container& items, const std::string& separator) {
    std::ostringstream out;
    bool first = true;
    for (const auto& item : items) {
        if (!first) out << separator;
        out << item;
        first = false;
    }
    return out.str();
}
