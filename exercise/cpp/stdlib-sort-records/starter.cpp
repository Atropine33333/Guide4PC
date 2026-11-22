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
        // TODO: 分数相同时还要按名字升序比较。
        return a.score > b.score;
    });

    for (const Record& record : records) {
        std::cout << record.name << ' ' << record.score << '\n';
    }
    return 0;
}
