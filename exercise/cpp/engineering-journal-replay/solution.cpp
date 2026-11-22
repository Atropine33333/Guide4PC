#include <map>
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
    std::map<std::string, int> store;
    std::map<std::string, int> staged;
    bool active = false;

    for (const JournalEntry& entry : entries) {
        if (entry.op == "begin") {
            if (!active) {
                active = true;
                staged.clear();
            }
        } else if (entry.op == "commit") {
            if (active) {
                for (const auto& item : staged) {
                    store[item.first] = item.second;
                }
                staged.clear();
                active = false;
            }
        } else if (entry.op == "rollback") {
            if (active) {
                staged.clear();
                active = false;
            }
        } else if (entry.op == "put") {
            if (active) {
                staged[entry.key] = entry.value;
            } else {
                store[entry.key] = entry.value;
            }
        }
    }

    return {store.begin(), store.end()};
}
