// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

using State = std::vector<std::pair<std::string, int>>;

bool same(const State& actual, const State& expected) {
    return actual == expected;
}

}  // namespace

int main() {
    {
        const std::vector<JournalEntry> log = {
            {"put", "a", 1}, {"put", "b", 2}};
        report("direct-puts",
               same(replay(log), State{{"a", 1}, {"b", 2}}));
    }

    {
        const std::vector<JournalEntry> log = {{"put", "a", 1},
                                               {"begin", "", 0},
                                               {"put", "a", 5},
                                               {"put", "b", 3},
                                               {"commit", "", 0}};
        report("commit", same(replay(log), State{{"a", 5}, {"b", 3}}));
    }

    {
        const std::vector<JournalEntry> log = {{"put", "a", 1},
                                               {"begin", "", 0},
                                               {"put", "a", 9},
                                               {"put", "b", 2},
                                               {"rollback", "", 0}};
        report("rollback", same(replay(log), State{{"a", 1}}));
    }

    {
        const std::vector<JournalEntry> log = {{"begin", "", 0},
                                               {"put", "a", 2},
                                               {"begin", "", 0},
                                               {"put", "b", 3},
                                               {"commit", "", 0}};
        report("nested-begin-ignored",
               same(replay(log), State{{"a", 2}, {"b", 3}}));
    }

    {
        const std::vector<JournalEntry> log = {{"commit", "", 0},
                                               {"rollback", "", 0},
                                               {"put", "c", 4}};
        report("orphan-controls", same(replay(log), State{{"c", 4}}));
    }

    {
        const std::vector<JournalEntry> log = {{"begin", "", 0},
                                               {"put", "k", 1},
                                               {"put", "k", 2},
                                               {"put", "k", 3},
                                               {"commit", "", 0}};
        report("overwrite-in-transaction", same(replay(log), State{{"k", 3}}));
    }

    {
        const std::vector<JournalEntry> log = {{"put", "z", 1},
                                               {"put", "a", 2},
                                               {"put", "m", 3}};
        report("key-order",
               same(replay(log), State{{"a", 2}, {"m", 3}, {"z", 1}}));
    }

    {
        const std::vector<JournalEntry> log = {{"begin", "", 0},
                                               {"put", "a", 2},
                                               {"rollback", "", 0},
                                               {"commit", "", 0}};
        report("rollback-then-commit", same(replay(log), State{}));
    }

    {
        const std::vector<JournalEntry> log = {{"begin", "", 0},
                                               {"put", "x", 7}};
        report("unfinished-transaction", same(replay(log), State{}));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
