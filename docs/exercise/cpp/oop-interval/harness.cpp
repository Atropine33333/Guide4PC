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
        const Interval interval(5, 2);
        report("normalize", interval.low() == 2 && interval.high() == 5 &&
                                Interval(3, 3).low() == 3);
    }

    {
        const Interval interval(1, 4);
        report("contains", interval.contains(1) && interval.contains(4) &&
                               interval.contains(2) &&
                               !interval.contains(0) && !interval.contains(5));
    }

    {
        const Interval a(1, 2);
        const Interval b(2, 5);
        const Interval c(3, 4);
        const Interval d(6, 8);
        const bool ok = a.overlaps(b) && b.overlaps(a) && !a.overlaps(c) &&
                        !a.overlaps(d) && !d.overlaps(a) &&
                        Interval(1, 10).overlaps(Interval(4, 5));
        report("overlaps", ok);
    }

    {
        const Interval union1 = Interval(1, 3).united(Interval(5, 7));
        const Interval union2 = Interval(1, 5).united(Interval(3, 4));
        const Interval union3 = Interval(-5, -1).united(Interval(-3, 2));
        const bool ok = union1.low() == 1 && union1.high() == 7 &&
                        union2.low() == 1 && union2.high() == 5 &&
                        union3.low() == -5 && union3.high() == 2;
        report("united", ok);
    }

    {
        const bool ok = Interval(1, 2) == Interval(1, 2) &&
                        !(Interval(1, 2) == Interval(1, 3)) &&
                        Interval(5, 2) == Interval(2, 5);
        report("equality", ok);
    }

    {
        const Interval point(7, 7);
        const bool ok = point.low() == 7 && point.high() == 7 &&
                        point.contains(7) && !point.contains(6) &&
                        point.overlaps(Interval(7, 9));
        report("point", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
