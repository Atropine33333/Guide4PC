// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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

}  // namespace

int main() {
    report("zero", format_duration(0) == "0:00:00");

    report("under-minute", format_duration(59) == "0:00:59");

    {
        const bool ok = format_duration(60) == "0:01:00" &&
                        format_duration(61) == "0:01:01" &&
                        format_duration(3599) == "0:59:59";
        report("minute-carry", ok);
    }

    {
        const bool ok = format_duration(3600) == "1:00:00" &&
                        format_duration(3601) == "1:00:01" &&
                        format_duration(3661) == "1:01:01";
        report("hour-carry", ok);
    }

    report("day-length", format_duration(86399) == "23:59:59");

    {
        const bool ok = format_duration(360000) == "100:00:00" &&
                        format_duration(1000000000000LL) == "277777777:46:40";
        report("large-hours", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
