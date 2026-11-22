// Guide4PC harness appended to the student's code in the same translation unit.
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool h_less(const Record &lhs, const Record &rhs) {
    if (lhs.score != rhs.score) return lhs.score > rhs.score;
    if (lhs.name != rhs.name) return lhs.name < rhs.name;
    return lhs.id < rhs.id;
}

std::vector<Record> h_sorted(std::vector<Record> records) {
    std::sort(records.begin(), records.end(), h_less);
    return records;
}

bool same(const std::vector<Record> &a, const std::vector<Record> &b) {
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i].name != b[i].name || a[i].score != b[i].score || a[i].id != b[i].id) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    {
        std::vector<Record> records{{"a", 10, 1}, {"b", 30, 2}, {"c", 20, 3}};
        sort_records(records);
        report("by-score", same(records, h_sorted({{"a", 10, 1}, {"b", 30, 2}, {"c", 20, 3}})));
    }

    {
        std::vector<Record> records{{"b", 5, 1}, {"a", 5, 2}, {"c", 5, 3}};
        sort_records(records);
        report("tie-name", same(records, h_sorted({{"b", 5, 1}, {"a", 5, 2}, {"c", 5, 3}})));
    }

    {
        std::vector<Record> records{{"x", 5, 9}, {"x", 5, 3}, {"x", 5, 5}};
        sort_records(records);
        report("tie-id", same(records, h_sorted({{"x", 5, 9}, {"x", 5, 3}, {"x", 5, 5}})));
    }

    {
        std::vector<Record> records{{"n", -5, 1}, {"p", 3, 2}, {"z", -10, 3}};
        sort_records(records);
        report("negative-scores", same(records, h_sorted({{"n", -5, 1}, {"p", 3, 2}, {"z", -10, 3}})));
    }

    {
        std::vector<Record> empty;
        sort_records(empty);
        std::vector<Record> single{{"only", 1, 1}};
        sort_records(single);
        std::vector<Record> twins{{"same", 2, 4}, {"same", 2, 4}};
        sort_records(twins);
        report("edges", empty.empty() && single.size() == 1 && single[0].name == "only" &&
                           twins[0].id == 4 && twins[1].id == 4);
    }

    {
        std::vector<Record> records{{"a", 1, 1}, {"a", 1, 2}, {"b", 1, 1}, {"a", 2, 1}};
        bool ok = true;
        for (const Record &x : records) {
            if (record_less(x, x)) ok = false;
            for (const Record &y : records) {
                if (&x == &y) continue;
                if (record_less(x, y) == record_less(y, x)) ok = false;
            }
        }
        report("strict-order", ok);
    }

    {
        std::vector<Record> records;
        std::uint32_t state = 123456789u;
        for (int i = 0; i < 200; ++i) {
            state = state * 1664525u + 1013904223u;
            const int score = static_cast<int>(state % 11) - 5;
            state = state * 1664525u + 1013904223u;
            const std::string name(1, static_cast<char>('a' + state % 5));
            records.push_back(Record{name, score, i});
        }
        const std::vector<Record> expected = h_sorted(records);
        sort_records(records);
        bool ok = same(records, expected);
        for (std::size_t i = 0; i + 1 < records.size(); ++i) {
            if (!h_less(records[i], records[i + 1])) ok = false;
        }
        report("random-large", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
