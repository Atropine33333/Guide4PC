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

bool balance_is(const Ledger& ledger, int account, long long expected) {
    const std::optional<long long> value = ledger.balance(account);
    return value.has_value() && *value == expected;
}

}  // namespace

int main() {
    {
        Ledger ledger;
        const bool first = ledger.open(1);
        const bool second = ledger.open(2);
        const bool duplicate = ledger.open(1);
        report("open", first && second && !duplicate &&
                           ledger.accounts() == 2 &&
                           balance_is(ledger, 1, 0) &&
                           balance_is(ledger, 2, 0) &&
                           !ledger.balance(3).has_value() && ledger.audit());
    }

    {
        Ledger ledger;
        ledger.open(1);
        ledger.open(2);
        const bool one = ledger.deposit(1, 100);
        const bool two = ledger.deposit(2, 50);
        const bool zero = ledger.deposit(1, 0);
        const bool negative = ledger.deposit(1, -5);
        const bool unknown = ledger.deposit(9, 1);
        report("deposit", one && two && !zero && !negative && !unknown &&
                              balance_is(ledger, 1, 100) &&
                              balance_is(ledger, 2, 50) &&
                              ledger.total() == 150 && ledger.audit());
    }

    {
        Ledger ledger;
        ledger.open(1);
        ledger.deposit(1, 100);
        const bool first = ledger.withdraw(1, 30);
        const bool too_much = ledger.withdraw(1, 71);
        const bool exact = ledger.withdraw(1, 70);
        const bool empty = ledger.withdraw(1, 1);
        report("withdraw", first && !too_much && exact && !empty &&
                               balance_is(ledger, 1, 0) &&
                               ledger.total() == 0 && ledger.audit());
    }

    {
        Ledger ledger;
        ledger.open(1);
        ledger.open(2);
        ledger.deposit(1, 100);
        ledger.deposit(2, 50);
        const bool moved = ledger.transfer(1, 2, 30);
        report("transfer", moved && balance_is(ledger, 1, 70) &&
                               balance_is(ledger, 2, 80) &&
                               ledger.total() == 150 && ledger.audit());
    }

    {
        Ledger ledger;
        ledger.open(1);
        ledger.open(2);
        ledger.deposit(1, 10);
        ledger.deposit(2, 5);
        const bool too_much = ledger.transfer(1, 2, 20);
        const bool unknown = ledger.transfer(1, 9, 10);
        const bool same = ledger.transfer(1, 1, 1);
        report("atomic", !too_much && !unknown && !same &&
                             balance_is(ledger, 1, 10) &&
                             balance_is(ledger, 2, 5) &&
                             ledger.total() == 15 && ledger.audit());
    }

    {
        Ledger ledger;
        ledger.open(-3);
        ledger.open(-4);
        ledger.deposit(-3, 1000000000000LL);
        const bool moved = ledger.transfer(-3, -4, 400000000000LL);
        const bool before = balance_is(ledger, -3, 600000000000LL) &&
                            balance_is(ledger, -4, 400000000000LL) &&
                            ledger.total() == 1000000000000LL &&
                            ledger.accounts() == 2 && ledger.audit();
        const bool drained = ledger.withdraw(-4, 400000000000LL);
        report("audit", moved && before && drained &&
                            balance_is(ledger, -4, 0) &&
                            ledger.total() == 600000000000LL &&
                            ledger.audit());
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
