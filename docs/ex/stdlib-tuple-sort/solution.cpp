// 参考实现：stdlib-tuple-sort
#include <algorithm>
#include <string>
#include <vector>

struct Record {
    std::string name;
    int score;
    int id;
};

bool record_less(const Record& lhs, const Record& rhs) {
    if (lhs.score != rhs.score) return lhs.score > rhs.score;
    if (lhs.name != rhs.name) return lhs.name < rhs.name;
    return lhs.id < rhs.id;
}

void sort_records(std::vector<Record>& records) {
    std::sort(records.begin(), records.end(), record_less);
}
