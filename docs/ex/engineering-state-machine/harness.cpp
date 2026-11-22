// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool h_name_is(State state, const char *expected) {
    return std::string(Order::name(state)) == expected;
}

}  // namespace

int main() {
    {
        Order order;
        const bool ok = order.state() == State::Created && order.rejected() == 0;
        report("initial", ok);
    }

    {
        Order order;
        const bool ok = order.apply(Event::Pay) && order.state() == State::Paid &&
                        order.apply(Event::Ship) && order.state() == State::Shipped &&
                        order.apply(Event::Deliver) && order.state() == State::Delivered &&
                        order.rejected() == 0;
        report("happy-path", ok);
    }

    {
        Order order;
        order.apply(Event::Pay);
        const bool ok = order.apply(Event::Cancel) && order.state() == State::Cancelled &&
                        order.rejected() == 0;
        report("cancel-paid", ok);
    }

    {
        Order order;
        order.apply(Event::Pay);
        order.apply(Event::Ship);
        const bool ok = !order.apply(Event::Cancel) && order.state() == State::Shipped &&
                        order.rejected() == 1 && !order.apply(Event::Pay) &&
                        order.state() == State::Shipped && order.rejected() == 2;
        report("reject-unchanged", ok);
    }

    {
        Order delivered;
        delivered.apply(Event::Pay);
        delivered.apply(Event::Ship);
        delivered.apply(Event::Deliver);
        Order cancelled;
        cancelled.apply(Event::Cancel);
        Order returned;
        returned.apply(Event::Pay);
        returned.apply(Event::Ship);
        returned.apply(Event::Deliver);
        returned.apply(Event::Return);
        const bool ok = !delivered.apply(Event::Pay) && delivered.state() == State::Delivered &&
                        !cancelled.apply(Event::Ship) &&
                        cancelled.state() == State::Cancelled &&
                        !returned.apply(Event::Return) && returned.state() == State::Returned;
        report("terminal", ok);
    }

    {
        Order order;
        order.apply(Event::Pay);
        order.apply(Event::Ship);
        order.apply(Event::Deliver);
        const bool ok = order.apply(Event::Return) && order.state() == State::Returned;
        report("return", ok);
    }

    {
        const bool ok = h_name_is(State::Created, "created") &&
                        h_name_is(State::Paid, "paid") &&
                        h_name_is(State::Shipped, "shipped") &&
                        h_name_is(State::Delivered, "delivered") &&
                        h_name_is(State::Cancelled, "cancelled") &&
                        h_name_is(State::Returned, "returned");
        report("names", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
