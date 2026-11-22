// Guide4PC harness appended to the student's code in the same translation unit.
#include <algorithm>
#include <iostream>
#include <map>
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

std::string h_key(int index) { return "key-" + std::to_string(index); }

std::vector<std::string> h_keys(int count) {
    std::vector<std::string> keys;
    for (int i = 0; i < count; ++i) keys.push_back(h_key(i));
    return keys;
}

}  // namespace

int main() {
    {
        HashRing ring(20);
        const bool ok = ring.size() == 0 && ring.locate("anything").empty() &&
                        ring.locate("").empty();
        ring.remove_node("ghost");
        report("empty", ok && ring.size() == 0);
    }

    {
        HashRing ring(1);
        ring.add_node("solo");
        bool ok = ring.size() == 1;
        for (const std::string &key : h_keys(50)) {
            if (ring.locate(key) != "solo") ok = false;
        }
        report("single-node", ok);
    }

    {
        HashRing ring(30);
        ring.add_node("a");
        ring.add_node("a");
        const std::size_t one = ring.size();
        ring.add_node("b");
        ring.add_node("b");
        const bool ok = one == 1 && ring.size() == 2;
        report("duplicate-add", ok);
    }

    {
        HashRing ring(30);
        ring.add_node("a");
        ring.add_node("b");
        ring.remove_node("a");
        ring.remove_node("a");
        bool ok = ring.size() == 1;
        for (const std::string &key : h_keys(80)) {
            if (ring.locate(key) != "b") ok = false;
        }
        report("remove-all", ok);
    }

    {
        HashRing ring(50);
        ring.add_node("a");
        ring.add_node("b");
        ring.add_node("c");
        std::map<std::string, int> counts;
        bool known = true;
        for (const std::string &key : h_keys(300)) {
            const std::string owner = ring.locate(key);
            ++counts[owner];
            if (owner != "a" && owner != "b" && owner != "c") known = false;
        }
        const bool ok = known && counts.size() == 3 && counts["a"] >= 40 &&
                        counts["b"] >= 40 && counts["c"] >= 40;
        report("balance", ok);
    }

    {
        HashRing ring(50);
        ring.add_node("a");
        ring.add_node("b");
        std::vector<std::string> before;
        for (const std::string &key : h_keys(400)) before.push_back(ring.locate(key));
        ring.add_node("c");
        bool ok = true;
        for (std::size_t i = 0; i < before.size(); ++i) {
            const std::string after = ring.locate(h_key(static_cast<int>(i)));
            if (after != before[i] && after != "c") ok = false;
        }
        report("add-stability", ok);
    }

    {
        HashRing ring(50);
        ring.add_node("a");
        ring.add_node("b");
        ring.add_node("c");
        std::vector<std::string> before;
        for (const std::string &key : h_keys(400)) before.push_back(ring.locate(key));
        ring.remove_node("a");
        bool ok = true;
        for (std::size_t i = 0; i < before.size(); ++i) {
            const std::string after = ring.locate(h_key(static_cast<int>(i)));
            if (before[i] == "a") {
                if (after != "b" && after != "c") ok = false;
            } else if (after != before[i]) {
                ok = false;
            }
        }
        report("remove-stability", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
