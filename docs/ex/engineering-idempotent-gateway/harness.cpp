// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <optional>

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
        Gateway gateway;
        report("accepted", gateway.submit(10, 500) == SubmitStatus::accepted &&
                               gateway.pending() == 1 &&
                               gateway.completed() == 0 &&
                               !gateway.is_completed(10) &&
                               !gateway.result_of(10).has_value());
    }

    {
        Gateway gateway;
        gateway.submit(10, 500);
        const bool first = gateway.submit(10, 500) == SubmitStatus::duplicate;
        const bool second = gateway.submit(10, 500) == SubmitStatus::duplicate;
        report("duplicate", first && second && gateway.pending() == 1 &&
                                 gateway.completed() == 0 &&
                                 !gateway.result_of(10).has_value());
    }

    {
        Gateway gateway;
        gateway.submit(10, 500);
        const bool conflict = gateway.submit(10, 501) == SubmitStatus::conflict;
        report("conflict", conflict && gateway.pending() == 1 &&
                               !gateway.is_completed(10) &&
                               !gateway.result_of(10).has_value());
    }

    {
        Gateway gateway;
        gateway.submit(10, 500);
        const bool done = gateway.complete(10, 7);
        const bool again = gateway.complete(10, 8);
        report("complete", done && !again && gateway.is_completed(10) &&
                               gateway.pending() == 0 &&
                               gateway.completed() == 1 &&
                               gateway.result_of(10) == std::optional<int>(7));
    }

    {
        Gateway gateway;
        gateway.submit(10, 500);
        gateway.complete(10, 7);
        const bool duplicate =
            gateway.submit(10, 500) == SubmitStatus::duplicate;
        const bool conflict = gateway.submit(10, 600) == SubmitStatus::conflict;
        const bool again = gateway.complete(10, 9);
        report("after-complete", duplicate && conflict && !again &&
                                      gateway.completed() == 1 &&
                                      gateway.result_of(10) ==
                                          std::optional<int>(7));
    }

    {
        Gateway gateway;
        const bool negative =
            gateway.submit(-5, -10) == SubmitStatus::accepted;
        const bool zero = gateway.submit(0, 0) == SubmitStatus::accepted;
        gateway.complete(-5, -77);
        report("multiple", negative && zero && gateway.pending() == 1 &&
                               gateway.completed() == 1 &&
                               gateway.result_of(-5) == std::optional<int>(-77) &&
                               !gateway.result_of(0).has_value() &&
                               !gateway.result_of(99).has_value());
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
