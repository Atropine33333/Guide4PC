// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
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

}  // namespace

int main() {
    {
        OpenHashMap map;
        int out = 123;
        const bool ok = map.size() == 0 && !map.get(1, out) && out == 123 &&
                        !map.erase(1) && map.size() == 0;
        report("empty-map", ok);
    }

    {
        OpenHashMap map;
        map.put(1, 10);
        map.put(2, 20);
        map.put(1, 11);
        int a = 0;
        int b = 0;
        const bool ok = map.size() == 2 && map.get(1, a) && a == 11 &&
                        map.get(2, b) && b == 20;
        report("put-get-update", ok);
    }

    {
        OpenHashMap map;
        map.put(1, 1);
        map.put(9, 9);
        map.put(17, 17);
        const bool erased = map.erase(9);
        int value = 0;
        const bool ok = erased && !map.get(9, value) && map.get(17, value) &&
                        value == 17 && map.size() == 2 && !map.erase(9);
        report("tombstone-order", ok);
    }

    {
        OpenHashMap map;
        map.put(-5, 50);
        map.put(-1, 10);
        int value = 0;
        const bool ok = map.get(-5, value) && value == 50 && map.get(-1, value) &&
                        value == 10 && map.erase(-5) && !map.get(-5, value);
        report("negative-keys", ok);
    }

    {
        OpenHashMap map;
        int value = -7;
        const bool miss = !map.get(42, value);
        report("missing-keeps-out", miss && value == -7);
    }

    {
        OpenHashMap map;
        for (int key = 0; key < 200; ++key) {
            map.put(key, key * 3);
        }
        bool ok = map.size() == 200;
        for (int key = 0; key < 200; ++key) {
            int value = 0;
            ok = ok && map.get(key, value) && value == key * 3;
        }
        for (int key = 0; key < 200; key += 3) {
            ok = ok && map.erase(key);
        }
        ok = ok && map.size() == 133;
        for (int key = 0; key < 200; ++key) {
            int value = 0;
            if (key % 3 == 0) {
                ok = ok && !map.get(key, value);
            } else {
                ok = ok && map.get(key, value) && value == key * 3;
            }
        }
        report("grow-and-erase", ok);
    }

    {
        OpenHashMap map;
        for (int key = 0; key < 100; ++key) {
            map.put(key, key + 1);
        }
        for (int key = 0; key < 100; key += 2) {
            map.erase(key);
        }
        for (int key = 0; key < 100; key += 4) {
            map.put(key, key + 2);
        }
        bool ok = map.size() == 75;
        for (int key = 0; key < 100; ++key) {
            int value = 0;
            const bool present = map.get(key, value);
            if (key % 4 == 0) {
                ok = ok && present && value == key + 2;
            } else if (key % 2 == 0) {
                ok = ok && !present;
            } else {
                ok = ok && present && value == key + 1;
            }
        }
        report("reinsert", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
