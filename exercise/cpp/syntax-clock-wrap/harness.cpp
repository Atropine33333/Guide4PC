// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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
        Clock clock(1, 2, 3);
        const bool ok = clock.hour() == 1 && clock.minute() == 2 && clock.second() == 3 &&
                        clock.total_seconds() == 3723;
        report("construct-read", ok);
    }

    {
        Clock clock(25, 61, 61);
        const bool ok = clock.hour() == 2 && clock.minute() == 2 && clock.second() == 1 &&
                        clock.total_seconds() == 7321;
        report("normalize-overflow", ok);
    }

    {
        Clock clock(-1, 0, 0);
        const bool ok = clock.hour() == 23 && clock.minute() == 0 && clock.second() == 0 &&
                        clock.total_seconds() == 82800;
        report("negative-total", ok);
    }

    {
        Clock clock(23, 59, 59);
        clock.advance(2);
        const bool ok = clock.hour() == 0 && clock.minute() == 0 && clock.second() == 1 &&
                        clock.total_seconds() == 1;
        report("advance-forward", ok);
    }

    {
        Clock clock(0, 0, 0);
        clock.advance(-1);
        const bool ok = clock.hour() == 23 && clock.minute() == 59 && clock.second() == 59 &&
                        clock.total_seconds() == 86399;
        report("advance-backward", ok);
    }

    {
        Clock clock(12, 0, 0);
        clock.advance(86400 * 3);
        bool ok = clock.hour() == 12 && clock.minute() == 0 && clock.second() == 0;
        clock.advance(-86400 * 2);
        ok = ok && clock.hour() == 12 && clock.minute() == 0 && clock.second() == 0;
        report("full-cycle", ok);
    }

    {
        Clock first(1, 0, 0);
        Clock second(0, 60, 0);
        Clock different(1, 0, 1);
        report("equality", first == second && !(first == different));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
