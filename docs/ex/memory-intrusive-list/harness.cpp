// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
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

std::vector<Node> make_nodes() {
    std::vector<Node> nodes;
    nodes.reserve(5);
    for (int i = 0; i < 5; ++i) {
        nodes.push_back(Node{i, nullptr, nullptr});
    }
    return nodes;
}

}  // namespace

int main() {
    {
        IntrusiveList list;
        report("empty-list", list.empty() && list.size() == 0 &&
                                 list.head() == nullptr &&
                                 list.tail() == nullptr);
    }

    {
        std::vector<Node> nodes = make_nodes();
        IntrusiveList list;
        list.push_back(&nodes[0]);
        list.push_back(&nodes[1]);
        list.push_back(&nodes[2]);
        const bool ok = list.size() == 3 && list.head() == &nodes[0] &&
                        list.tail() == &nodes[2] &&
                        nodes[0].next == &nodes[1] &&
                        nodes[1].prev == &nodes[0] &&
                        nodes[2].prev == &nodes[1] &&
                        nodes[2].next == nullptr;
        report("push-back", ok);
    }

    {
        std::vector<Node> nodes = make_nodes();
        IntrusiveList list;
        for (int i = 0; i < 4; ++i) {
            list.push_back(&nodes[static_cast<std::size_t>(i)]);
        }
        list.unlink(&nodes[1]);
        const bool ok = list.size() == 3 && list.head() == &nodes[0] &&
                        list.head()->next == &nodes[2] &&
                        nodes[2].prev == &nodes[0] &&
                        nodes[1].prev == nullptr &&
                        nodes[1].next == nullptr;
        report("unlink-middle", ok);
    }

    {
        std::vector<Node> nodes = make_nodes();
        IntrusiveList list;
        for (int i = 0; i < 3; ++i) {
            list.push_back(&nodes[static_cast<std::size_t>(i)]);
        }
        list.unlink(&nodes[0]);
        const bool head_ok = list.head() == &nodes[1] &&
                             nodes[1].prev == nullptr && list.size() == 2;
        list.unlink(&nodes[2]);
        const bool ok = head_ok && list.tail() == &nodes[1] &&
                        nodes[1].next == nullptr && list.size() == 1;
        report("unlink-ends", ok);
    }

    {
        std::vector<Node> nodes = make_nodes();
        IntrusiveList list;
        list.push_back(&nodes[0]);
        list.unlink(nullptr);
        list.unlink(&nodes[3]);
        list.unlink(&nodes[0]);
        list.unlink(&nodes[0]);
        const bool ok = list.empty() && list.size() == 0 &&
                        nodes[0].prev == nullptr && nodes[0].next == nullptr;
        report("unlink-foreign", ok);
    }

    {
        std::vector<Node> nodes = make_nodes();
        IntrusiveList list;
        list.push_back(&nodes[0]);
        list.push_back(&nodes[1]);
        list.unlink(&nodes[0]);
        list.push_back(&nodes[0]);
        const bool ok = list.size() == 2 && list.head() == &nodes[1] &&
                        list.tail() == &nodes[0] &&
                        nodes[0].prev == &nodes[1] &&
                        nodes[1].next == &nodes[0];
        report("reinsert", ok);
    }

    {
        std::vector<Node> nodes = make_nodes();
        {
            IntrusiveList list;
            list.push_back(&nodes[0]);
            list.push_back(&nodes[1]);
        }
        const bool ok = nodes[0].value == 0 && nodes[1].value == 1 &&
                        nodes[0].next == &nodes[1];
        report("no-ownership", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
