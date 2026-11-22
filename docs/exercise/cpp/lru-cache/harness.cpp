// Guide4PC harness appended to the student's code in the same translation unit.
#include <algorithm>
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

struct Reference {
    int capacity = 0;
    std::map<int, int> values;
    std::vector<int> order;  // front is least recently used

    void eraseFromOrder(int key) {
        const auto it = std::find(order.begin(), order.end(), key);
        if (it != order.end()) order.erase(it);
    }

    bool get(int key, int &out) {
        const auto it = values.find(key);
        if (it == values.end()) return false;
        out = it->second;
        eraseFromOrder(key);
        order.push_back(key);
        return true;
    }

    void put(int key, int value) {
        if (capacity <= 0) return;
        const auto it = values.find(key);
        if (it != values.end()) {
            it->second = value;
            eraseFromOrder(key);
            order.push_back(key);
            return;
        }
        if (static_cast<int>(order.size()) >= capacity) {
            values.erase(order.front());
            order.erase(order.begin());
        }
        values[key] = value;
        order.push_back(key);
    }
};

}  // namespace

int main() {
    {
        LruCache cache(2);
        int value = 0;
        cache.put(1, 10);
        cache.put(2, 20);
        const bool ok = cache.capacity() == 2 && cache.size() == 2 &&
                        cache.get(1, value) && value == 10;
        report("basic", ok);
    }

    {
        LruCache cache(2);
        int value = 0;
        cache.put(1, 10);
        cache.put(1, 20);
        const bool ok = cache.size() == 1 && cache.get(1, value) && value == 20;
        report("update", ok);
    }

    {
        LruCache cache(2);
        int value = 0;
        cache.put(1, 1);
        cache.put(2, 2);
        cache.put(3, 3);
        const bool ok = !cache.get(1, value) && cache.get(2, value) && value == 2 &&
                        cache.get(3, value) && value == 3 && cache.size() == 2;
        report("eviction", ok);
    }

    {
        LruCache cache(2);
        int value = 0;
        cache.put(1, 1);
        cache.put(2, 2);
        cache.get(1, value);
        cache.put(3, 3);
        const bool ok = !cache.get(2, value) && cache.get(1, value) && value == 1;
        report("get-refreshes", ok);
    }

    {
        LruCache cache(0);
        int value = 0;
        cache.put(1, 1);
        cache.put(2, 2);
        const bool ok = cache.size() == 0 && cache.capacity() == 0 && !cache.get(1, value);
        report("zero-capacity", ok);
    }

    {
        LruCache cache(1);
        int value = 0;
        cache.put(1, 1);
        cache.put(2, 2);
        bool ok = !cache.get(1, value) && cache.get(2, value) && value == 2;
        cache.put(2, 20);
        ok = ok && cache.size() == 1 && cache.get(2, value) && value == 20;
        report("capacity-one", ok);
    }

    {
        LruCache cache(4);
        Reference reference;
        reference.capacity = 4;
        std::mt19937 rng(424242u);
        bool ok = true;
        for (int step = 0; step < 600 && ok; ++step) {
            const int action = static_cast<int>(rng() % 3u);
            const int key = static_cast<int>(rng() % 8u);
            if (action == 0) {
                int expected = 0;
                int value = 0;
                const bool expectedFound = reference.get(key, expected);
                ok = ok && cache.get(key, value) == expectedFound;
                if (expectedFound) ok = ok && value == expected;
            } else {
                const int value = static_cast<int>(rng() % 100u);
                reference.put(key, value);
                cache.put(key, value);
            }
            ok = ok && cache.size() == static_cast<int>(reference.values.size());
        }
        for (const auto &entry : reference.values) {
            int value = 0;
            ok = ok && cache.get(entry.first, value) && value == entry.second;
        }
        for (int key = 0; key < 8 && ok; ++key) {
            if (reference.values.count(key) == 0) {
                int value = 0;
                ok = ok && !cache.get(key, value);
            }
        }
        report("random-trace", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
