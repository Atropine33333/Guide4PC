// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
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

std::vector<int> values_of(const IntList& list) {
    std::vector<int> values;
    for (const Node *node = list.head(); node != nullptr; node = node->next) {
        values.push_back(node->value);
    }
    return values;
}

}  // namespace

int main() {
    {
        IntList list;
        const bool ok = list.size() == 0 && list.head() == nullptr;
        list.reverse();
        report("empty", ok && list.size() == 0 && list.head() == nullptr);
    }

    {
        IntList list;
        list.push_front(42);
        const bool ok = list.size() == 1 && list.head() != nullptr &&
                        list.head()->value == 42 && list.head()->next == nullptr;
        list.reverse();
        report("single", ok && values_of(list) == std::vector<int>{42});
    }

    {
        IntList list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        report("push-front", values_of(list) == std::vector<int>({3, 2, 1}) &&
                             list.size() == 3);
    }

    {
        IntList list;
        for (int value = 1; value <= 4; ++value) list.push_front(value);
        list.reverse();
        report("reverse", values_of(list) == std::vector<int>({1, 2, 3, 4}) &&
                          list.size() == 4);
    }

    {
        IntList list;
        for (int value = 1; value <= 5; ++value) list.push_front(value);
        list.reverse();
        list.reverse();
        report("double-reverse", values_of(list) == std::vector<int>({5, 4, 3, 2, 1}) &&
                                 list.size() == 5);
    }

    {
        IntList list;
        list.push_front(5);
        list.push_front(5);
        list.push_front(7);
        list.push_front(7);
        list.reverse();
        report("duplicates", values_of(list) == std::vector<int>({5, 5, 7, 7}) &&
                             list.size() == 4);
    }

    {
        IntList list;
        list.push_front(1);
        list.push_front(2);
        list.push_front(3);
        list.reverse();
        list.push_front(0);
        report("push-after-reverse",
               values_of(list) == std::vector<int>({0, 1, 2, 3}) && list.size() == 4);
    }

    {
        IntList list;
        for (int value = 0; value < 1000; ++value) list.push_front(value);
        list.reverse();
        bool ok = list.size() == 1000;
        int expected = 0;
        for (const Node *node = list.head(); ok && node != nullptr; node = node->next) {
            if (node->value != expected) ok = false;
            ++expected;
        }
        report("long", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
