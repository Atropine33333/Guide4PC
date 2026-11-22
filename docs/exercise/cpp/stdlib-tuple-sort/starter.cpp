#include <algorithm>
#include <string>
#include <vector>

struct Record {
    std::string name;
    int score;
    int id;
};

bool record_less(const Record& lhs, const Record& rhs) {
    // TODO: 只比较了分数，分数相同时的并列规则还没有实现。
    return lhs.score > rhs.score;
}

void sort_records(std::vector<Record>& records) {
    std::sort(records.begin(), records.end(), record_less);
}
