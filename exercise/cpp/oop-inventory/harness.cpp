// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>

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
        Inventory inventory;
        report("empty", inventory.distinct() == 0 && inventory.total() == 0 &&
                            inventory.quantity("ghost") == 0 &&
                            !inventory.remove("ghost", 1));
    }

    {
        Inventory inventory;
        inventory.add("apple", 3);
        inventory.add("pear", 2);
        inventory.add("apple", 2);
        report("add", inventory.quantity("apple") == 5 &&
                          inventory.quantity("pear") == 2 &&
                          inventory.distinct() == 2 && inventory.total() == 7);
    }

    {
        Inventory inventory;
        inventory.add("apple", 5);
        inventory.add("pear", 2);
        const bool partial = inventory.remove("apple", 2);
        const bool full = inventory.remove("pear", 2);
        report("remove", partial && full && inventory.quantity("apple") == 3 &&
                             inventory.quantity("pear") == 0 &&
                             inventory.distinct() == 1 &&
                             inventory.total() == 3);
    }

    {
        Inventory inventory;
        inventory.add("apple", 3);
        const bool too_many = inventory.remove("apple", 4);
        const bool unknown = inventory.remove("melon", 1);
        const bool zero = inventory.remove("apple", 0);
        const bool negative = inventory.remove("apple", -2);
        inventory.add("apple", 0);
        inventory.add("apple", -5);
        report("failure", !too_many && !unknown && !zero && !negative &&
                              inventory.quantity("apple") == 3 &&
                              inventory.distinct() == 1 &&
                              inventory.total() == 3);
    }

    {
        Inventory inventory;
        inventory.add("", 4);
        inventory.add("red apple", 2);
        inventory.add("Red Apple", 1);
        inventory.add("a b c", 3);
        report("names", inventory.quantity("") == 4 &&
                            inventory.quantity("red apple") == 2 &&
                            inventory.quantity("Red Apple") == 1 &&
                            inventory.quantity("a b c") == 3 &&
                            inventory.distinct() == 4 &&
                            inventory.total() == 10);
    }

    {
        Inventory inventory;
        for (int i = 0; i < 1000; ++i) {
            inventory.add("sku-" + std::to_string(i % 50), 1000000);
        }
        report("large", inventory.distinct() == 50 &&
                            inventory.total() == 1000000000LL &&
                            inventory.quantity("sku-0") == 20000000);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
