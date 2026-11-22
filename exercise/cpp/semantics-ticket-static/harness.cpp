// Guide4PC harness appended to the student's code in the same translation unit.
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
        reset_tickets();
        const bool ok = tickets_issued() == 0 && next_ticket() == 1 && tickets_issued() == 1;
        report("first-ticket", ok);
    }

    {
        reset_tickets();
        bool ok = next_ticket() == 1;
        ok = ok && next_ticket() == 2;
        ok = ok && next_ticket() == 3;
        ok = ok && tickets_issued() == 3;
        report("sequence", ok);
    }

    {
        reset_tickets();
        next_ticket();
        next_ticket();
        reset_tickets();
        const bool ok = tickets_issued() == 0 && next_ticket() == 1 && tickets_issued() == 1;
        report("reset-restarts", ok);
    }

    {
        reset_tickets();
        next_ticket();
        next_ticket();
        bool ok = tickets_issued() == 2;
        ok = ok && tickets_issued() == 2;
        ok = ok && next_ticket() == 3 && tickets_issued() == 3;
        report("count-stable", ok);
    }

    {
        reset_tickets();
        bool ok = true;
        for (int i = 1; i <= 100; ++i) {
            ok = ok && next_ticket() == i;
        }
        ok = ok && tickets_issued() == 100;
        report("many-tickets", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
