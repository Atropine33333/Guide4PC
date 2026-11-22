#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> find_all(const std::string& text,
                                  const std::string& pattern) {
    std::vector<std::size_t> result;
    if (pattern.empty() || pattern.size() > text.size()) return result;

    // TODO: this only finds the first occurrence; implement KMP for all of them.
    const std::size_t pos = text.find(pattern);
    if (pos != std::string::npos) result.push_back(pos);
    return result;
}
