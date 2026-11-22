// Reference solution: algorithm-kmp-search
#include <cstddef>
#include <string>
#include <vector>

std::vector<std::size_t> find_all(const std::string& text,
                                  const std::string& pattern) {
    std::vector<std::size_t> result;
    if (pattern.empty() || pattern.size() > text.size()) return result;

    std::vector<std::size_t> prefix(pattern.size(), 0);
    for (std::size_t i = 1; i < pattern.size(); ++i) {
        std::size_t length = prefix[i - 1];
        while (length > 0 && pattern[i] != pattern[length]) {
            length = prefix[length - 1];
        }
        if (pattern[i] == pattern[length]) ++length;
        prefix[i] = length;
    }

    std::size_t matched = 0;
    for (std::size_t i = 0; i < text.size(); ++i) {
        while (matched > 0 && text[i] != pattern[matched]) {
            matched = prefix[matched - 1];
        }
        if (text[i] == pattern[matched]) ++matched;
        if (matched == pattern.size()) {
            result.push_back(i + 1 - pattern.size());
            matched = prefix[matched - 1];
        }
    }
    return result;
}
