// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

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
        Seq s{1, 2, 3};
        const bool ok = std::is_same_v<decltype(s), Seq<int>> && s.size() == 3 &&
                        s.sum() == 6 && s.at(1) == 2;
        report("init-int", ok);
    }

    {
        Seq d{1.5, 2.5, 4.0};
        const bool ok = std::is_same_v<decltype(d), Seq<double>> && d.size() == 3 &&
                        d.sum() == 8.0 && d.contains(2.5) && !d.contains(3.5);
        report("init-double", ok);
    }

    {
        Seq w{std::string("alpha"), std::string("beta")};
        const bool ok = std::is_same_v<decltype(w), Seq<std::string>> && w.size() == 2 &&
                        w.at(0) == "alpha" && w.contains("beta") && !w.contains("gamma");
        report("init-string", ok);
    }

    {
        std::vector<long long> values{3000000000LL, 2000000000LL, 5LL};
        Seq r(values.begin(), values.end());
        const std::vector<long long>& constant = values;
        Seq c(constant.begin(), constant.end());
        const bool ok = std::is_same_v<decltype(r), Seq<long long>> && r.size() == 3 &&
                        r.sum() == 5000000005LL &&
                        std::is_same_v<decltype(c), Seq<long long>> &&
                        c.sum() == 5000000005LL;
        report("iterator-range", ok);
    }

    {
        int raw[] = {4, 5, 6};
        Seq p(raw, raw + 3);
        const bool ok = std::is_same_v<decltype(p), Seq<int>> && p.size() == 3 &&
                        p.sum() == 15 && p.at(2) == 6;
        report("pointer-range", ok);
    }

    {
        long long raw[] = {1, 2, 3, 4};
        Seq a(raw);
        const bool ok = std::is_same_v<decltype(a), Seq<long long>> && a.size() == 4 &&
                        a.sum() == 10LL && a.contains(3LL);
        report("c-array", ok);
    }

    {
        Seq<int> empty;
        const bool ok = empty.size() == 0 && empty.sum() == 0 && !empty.contains(1);
        report("explicit-empty", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
