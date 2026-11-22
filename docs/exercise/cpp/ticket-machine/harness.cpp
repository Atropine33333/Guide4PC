// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <iostream>

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
        TicketMachine machine;
        report("first-ticket", machine.next() == 1 && machine.issued() == 1);
    }

    {
        TicketMachine machine;
        const bool numbers = machine.next() == 1 && machine.next() == 2 && machine.next() == 3;
        report("sequence", numbers && machine.issued() == 3);
    }

    {
        TicketMachine first;
        TicketMachine second;
        const bool ok = first.next() == 1 && second.next() == 1 &&
                        first.next() == 2 && second.next() == 2 &&
                        first.issued() == 2 && second.issued() == 2;
        report("independent-machines", ok);
    }

    {
        TicketMachine machine;
        machine.next();
        machine.next();
        report("issued-count", machine.issued() == 2);
    }

    {
        TicketMachine machine;
        machine.next();
        machine.next();
        machine.reset();
        report("reset-restarts", machine.issued() == 0 && machine.next() == 1);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
