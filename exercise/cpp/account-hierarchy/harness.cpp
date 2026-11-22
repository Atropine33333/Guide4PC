// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstring>
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
        Checking account(100);
        const bool ok = account.deposit(50) && account.balance() == 150 &&
                        account.withdraw(30) && account.balance() == 120 &&
                        std::strcmp(account.kind(), "checking") == 0;
        report("checking-basic", ok);
    }

    {
        Savings account(5000);
        bool ok = account.withdraw(4000) && account.balance() == 1000;
        ok = ok && !account.withdraw(1) && account.balance() == 1000;
        ok = ok && account.deposit(500) && account.withdraw(500) && account.balance() == 1000;
        ok = ok && std::strcmp(account.kind(), "savings") == 0;
        report("savings-minimum", ok);
    }

    {
        Checking account(100);
        bool ok = !account.withdraw(101) && account.balance() == 100;
        ok = ok && account.withdraw(100) && account.balance() == 0;
        ok = ok && !account.withdraw(1) && account.balance() == 0;
        report("checking-insufficient", ok);
    }

    {
        Checking checking(500);
        Savings savings(5000);
        bool ok = !checking.deposit(0) && !checking.deposit(-10) && checking.balance() == 500;
        ok = ok && !checking.withdraw(0) && !checking.withdraw(-10) && checking.balance() == 500;
        ok = ok && !savings.withdraw(0) && !savings.withdraw(-10) && savings.balance() == 5000;
        report("non-positive", ok);
    }

    {
        Checking checking(1500);
        Savings savings(2000);
        Account *accounts[2] = {&checking, &savings};
        const bool first = accounts[0]->withdraw(1200);
        const bool second = accounts[1]->withdraw(1200);
        const bool ok = first && checking.balance() == 300 &&
                        !second && savings.balance() == 2000 &&
                        std::strcmp(accounts[0]->kind(), "checking") == 0 &&
                        std::strcmp(accounts[1]->kind(), "savings") == 0;
        report("base-dispatch", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
