// 参考实现：stdlib-sort-records
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Record {
    std::string name;
    int score = 0;
};

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<Record> records(n);
    for (Record& record : records) {
        std::cin >> record.name >> record.score;
    }

    std::sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.name < b.name;
    });

    for (const Record& record : records) {
        std::cout << record.name << ' ' << record.score << '\n';
    }
    return 0;
}
