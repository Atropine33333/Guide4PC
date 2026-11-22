// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <memory>

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
        const std::shared_ptr<Node> head = build_chain(0);
        report("empty", head == nullptr && chain_length(nullptr) == 0 &&
                            links_consistent(nullptr) &&
                            build_chain(-5) == nullptr);
    }

    {
        const std::shared_ptr<Node> head = build_chain(1);
        const bool ok = head != nullptr && head->value == 0 &&
                        head->next == nullptr && head->previous.expired() &&
                        chain_length(head) == 1 && links_consistent(head);
        report("single", ok);
    }

    {
        const std::shared_ptr<Node> head = build_chain(5);
        bool values = true;
        int expected = 0;
        std::shared_ptr<Node> current = head;
        while (current != nullptr) {
            values = values && current->value == expected;
            ++expected;
            current = current->next;
        }
        report("values", values && expected == 5 && chain_length(head) == 5 &&
                             links_consistent(head));
    }

    {
        std::shared_ptr<Node> head = build_chain(3);
        std::shared_ptr<Node> tail = head;
        while (tail->next != nullptr) tail = tail->next;
        const std::weak_ptr<Node> weak_tail_previous = tail->previous;
        head.reset();
        report("weak-release", weak_tail_previous.expired() &&
                                   tail->previous.expired() &&
                                   tail->next == nullptr);
    }

    {
        std::shared_ptr<Node> head = build_chain(2);
        std::shared_ptr<Node> second = head->next;
        second->previous.reset();
        const bool broken = !links_consistent(head);
        second->previous = head;
        report("broken-link", broken && links_consistent(head));
    }

    {
        const std::shared_ptr<Node> head = build_chain(1000);
        auto last = head;
        while (last->next != nullptr) last = last->next;
        report("long", chain_length(head) == 1000 && last->value == 999 &&
                           links_consistent(head));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
