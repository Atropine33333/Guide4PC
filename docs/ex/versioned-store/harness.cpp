// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <map>
#include <random>
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

}  // namespace

int main() {
    {
        VersionedStore store;
        int out = 0;
        store.put(1, 10);
        store.put(2, 20);
        bool ok = store.size() == 2 && store.get(1, out) && out == 10 &&
                  store.get(2, out) && out == 20;
        store.put(1, 11);
        ok = ok && store.size() == 2 && store.get(1, out) && out == 11;
        report("put-get", ok);
    }

    {
        VersionedStore store;
        int out = 0;
        store.put(1, 10);
        store.put(1, 20);
        const bool ok = store.undo() && store.get(1, out) && out == 10 &&
                        store.undo() && !store.get(1, out) && store.size() == 0 &&
                        !store.undo();
        report("undo-update", ok);
    }

    {
        VersionedStore store;
        int out = 0;
        store.put(3, 30);
        const bool ok = store.undo() && !store.get(3, out) && store.size() == 0;
        report("undo-insert", ok);
    }

    {
        VersionedStore store;
        int out = 0;
        store.put(1, 1);
        store.put(2, 2);
        store.put(1, 3);
        bool ok = store.undo() && store.get(1, out) && out == 1 && store.get(2, out) && out == 2;
        ok = ok && store.undo() && !store.get(2, out) && store.get(1, out) && out == 1;
        ok = ok && store.undo() && !store.get(1, out) && store.size() == 0 && !store.undo();
        report("undo-order", ok);
    }

    {
        VersionedStore store;
        int out = 0;
        store.put(1, 1);
        store.clear();
        bool ok = store.size() == 0 && !store.get(1, out) && !store.undo();
        store.put(2, 2);
        ok = ok && store.size() == 1 && store.get(2, out) && out == 2;
        ok = ok && store.undo() && !store.get(2, out) && store.size() == 0;
        report("clear-resets-history", ok);
    }

    {
        VersionedStore store;
        int out = 0;
        const bool ok = store.size() == 0 && !store.get(0, out) && !store.undo() &&
                        store.get(-5, out) == false;
        report("undo-empty", ok);
    }

    {
        VersionedStore store;
        std::map<int, int> reference;
        std::vector<std::pair<int, std::pair<bool, int>>> history;
        std::mt19937 rng(777001u);
        bool ok = true;
        for (int step = 0; step < 500 && ok; ++step) {
            const int action = static_cast<int>(rng() % 10u);
            if (action < 5) {
                const int key = static_cast<int>(rng() % 8u) + 1;
                const int value = static_cast<int>(rng() % 100u);
                const auto it = reference.find(key);
                history.push_back(std::make_pair(
                    key, std::make_pair(it != reference.end(), it != reference.end() ? it->second : 0)));
                reference[key] = value;
                store.put(key, value);
            } else if (action < 8) {
                if (history.empty()) {
                    ok = ok && !store.undo();
                } else {
                    const auto change = history.back();
                    history.pop_back();
                    if (change.second.first) {
                        reference[change.first] = change.second.second;
                    } else {
                        reference.erase(change.first);
                    }
                    ok = ok && store.undo();
                }
            } else if (action == 8) {
                const int key = static_cast<int>(rng() % 10u);
                int value = 0;
                const auto it = reference.find(key);
                const bool expected = it != reference.end();
                ok = ok && store.get(key, value) == expected;
                if (expected) ok = ok && value == it->second;
            } else {
                reference.clear();
                history.clear();
                store.clear();
            }
            ok = ok && store.size() == static_cast<int>(reference.size());
        }
        report("random-trace", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
