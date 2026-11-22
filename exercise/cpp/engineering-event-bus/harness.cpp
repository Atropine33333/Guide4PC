// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;
std::vector<int> g_log;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

void record(void* context, int payload) {
    g_log.push_back(*static_cast<int*>(context) + payload);
}

bool same(const std::vector<int>& actual, const std::vector<int>& expected) {
    return actual == expected;
}

}  // namespace

int main() {
    int base_a = 100;
    int base_b = 200;

    {
        EventBus bus;
        const int id = bus.subscribe(1, record, &base_a);
        report("subscribe", id > 0 && bus.count(1) == 1 && bus.total() == 1);
    }

    {
        EventBus bus;
        bus.subscribe(7, record, &base_a);
        bus.subscribe(7, record, &base_b);
        g_log.clear();
        bus.publish(7, 5);
        report("publish-order", same(g_log, {105, 205}));
    }

    {
        EventBus bus;
        bus.subscribe(1, record, &base_a);
        bus.subscribe(2, record, &base_b);
        g_log.clear();
        bus.publish(1, 0);
        const bool first = same(g_log, {100});
        g_log.clear();
        bus.publish(2, 0);
        report("filtering", first && same(g_log, {200}) && bus.count(1) == 1 &&
                                bus.count(2) == 1 && bus.count(3) == 0);
    }

    {
        EventBus bus;
        const int id = bus.subscribe(3, record, &base_a);
        const bool removed = bus.unsubscribe(id);
        const bool again = bus.unsubscribe(id);
        g_log.clear();
        bus.publish(3, 1);
        report("unsubscribe", removed && !again && bus.total() == 0 && g_log.empty());
    }

    {
        EventBus bus;
        bool ok = true;
        int ids[8] = {};
        for (int i = 0; i < 8; ++i) {
            ids[i] = bus.subscribe(i, record, &base_a);
            ok = ok && ids[i] > 0;
        }
        ok = ok && bus.total() == 8;
        ok = ok && bus.subscribe(1, record, &base_a) == -1;
        ok = ok && bus.unsubscribe(ids[0]);
        const int replacement = bus.subscribe(9, record, &base_a);
        ok = ok && replacement > 0 && bus.total() == 8;
        report("limit", ok);
    }

    {
        EventBus bus;
        bus.subscribe(5, record, &base_a);
        bus.subscribe(5, record, &base_b);
        bus.clear();
        g_log.clear();
        bus.publish(5, 2);
        report("clear", bus.total() == 0 && bus.count(5) == 0 && g_log.empty());
    }

    {
        EventBus bus;
        const int first = bus.subscribe(4, record, &base_a);
        bus.subscribe(4, record, &base_b);
        bus.unsubscribe(first);
        bus.subscribe(4, record, &base_a);
        g_log.clear();
        bus.publish(4, 0);
        report("resubscribe", same(g_log, {200, 100}));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
