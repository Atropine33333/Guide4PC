#include <string>
#include <utility>
#include <vector>

struct JournalEntry {
    std::string op;
    std::string key;
    int value;
};

std::vector<std::pair<std::string, int>> replay(
    const std::vector<JournalEntry>& entries) {
    // TODO: 维护已提交存储、事务缓冲和活跃标志，最后按 key 排序返回。
    (void)entries;
    return {};
}
