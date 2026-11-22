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
        const bool ok = Date::is_leap(2000) && !Date::is_leap(1900) &&
                        Date::is_leap(2024) && !Date::is_leap(2023) &&
                        Date::is_leap(4) && !Date::is_leap(100) &&
                        Date::is_leap(400);
        report("leap", ok);
    }

    {
        const bool ok = Date::days_in_month(2024, 2) == 29 &&
                        Date::days_in_month(2023, 2) == 28 &&
                        Date::days_in_month(1900, 2) == 28 &&
                        Date::days_in_month(2000, 2) == 29 &&
                        Date::days_in_month(2024, 4) == 30 &&
                        Date::days_in_month(2024, 12) == 31 &&
                        Date::days_in_month(2024, 0) == 0 &&
                        Date::days_in_month(2024, 13) == 0;
        report("days", ok);
    }

    {
        const bool ok = Date(2024, 2, 29).valid() && Date(2023, 2, 28).valid() &&
                        Date(9999, 12, 31).valid() && Date(1, 1, 1).valid() &&
                        !Date(2023, 2, 29).valid() && !Date(2024, 4, 31).valid() &&
                        !Date(2024, 13, 1).valid() && !Date(0, 1, 1).valid() &&
                        !Date(2024, 0, 1).valid() && !Date(2024, 1, 0).valid();
        report("valid", ok);
    }

    {
        Date leap(2024, 2, 28);
        leap.next_day();
        const bool first = leap.year() == 2024 && leap.month() == 2 && leap.day() == 29;

        Date common(2023, 2, 28);
        common.next_day();
        const bool second = common.month() == 3 && common.day() == 1;

        Date end(2024, 12, 31);
        end.next_day();
        const bool third = end.year() == 2025 && end.month() == 1 && end.day() == 1;

        Date april(2024, 4, 30);
        april.next_day();
        const bool fourth = april.month() == 5 && april.day() == 1;
        report("next", first && second && third && fourth);
    }

    {
        Date invalid(2023, 2, 29);
        invalid.next_day();
        const bool first = invalid.year() == 2023 && invalid.month() == 2 &&
                           invalid.day() == 29;

        Date out_of_range(2024, 13, 40);
        out_of_range.next_day();
        const bool second = out_of_range.month() == 13 && out_of_range.day() == 40;

        Date last(9999, 12, 31);
        last.next_day();
        const bool third = last.year() == 9999 && last.month() == 12 && last.day() == 31;
        report("invalid-next", first && second && third);
    }

    {
        const bool ok = Date(2023, 12, 31) < Date(2024, 1, 1) &&
                        Date(2024, 1, 1) < Date(2024, 1, 2) &&
                        Date(2024, 2, 1) < Date(2024, 10, 1) &&
                        !(Date(2024, 5, 5) < Date(2024, 5, 5)) &&
                        Date(2024, 5, 5) == Date(2024, 5, 5) &&
                        !(Date(2024, 5, 5) == Date(2024, 5, 6));
        report("compare", ok);
    }

    {
        const Date date(2024, 6, 15);
        report("accessors", date.year() == 2024 && date.month() == 6 && date.day() == 15);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
