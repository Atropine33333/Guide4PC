// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool close(double a, double b) { return std::fabs(a - b) <= 1e-9; }

bool same(const std::vector<double>& actual,
          const std::vector<double>& expected) {
    if (actual.size() != expected.size()) return false;
    for (std::size_t i = 0; i < actual.size(); ++i) {
        if (!close(actual[i], expected[i])) return false;
    }
    return true;
}

}  // namespace

int main() {
    {
        const std::vector<double> got =
            window_average({1, 2, 3, 4, 5}, 2);
        report("basic", same(got, {1.5, 2.5, 3.5, 4.5}));
    }

    {
        const std::vector<double> single = window_average({4, -2, 9}, 1);
        const std::vector<double> whole = window_average({1, 2, 3, 4}, 4);
        report("size-one", same(single, {4.0, -2.0, 9.0}) &&
                               same(whole, {2.5}));
    }

    {
        const std::vector<double> empty = window_average({1, 2, 3}, 0);
        const std::vector<double> too_wide = window_average({1, 2, 3}, 4);
        report("invalid", empty.empty() && too_wide.empty());
    }

    {
        const std::vector<double> got =
            window_average({-3, 1, 4, -2}, 3);
        report("negative", same(got, {2.0 / 3.0, 1.0}));
    }

    {
        const std::vector<double> got = window_average({0, 0, 3, 3}, 2);
        report("fractional", same(got, {0.0, 1.5, 3.0}));
    }

    {
        std::vector<int> values(1000);
        for (std::size_t i = 0; i < values.size(); ++i) {
            values[i] = static_cast<int>((i * 37 + 11) % 101) - 50;
        }
        const std::vector<double> got = window_average(values, 100);
        bool ok = got.size() == 901;
        for (std::size_t start = 0; ok && start < got.size(); ++start) {
            long long sum = 0;
            for (std::size_t j = start; j < start + 100; ++j) {
                sum += values[j];
            }
            ok = close(got[start], static_cast<double>(sum) / 100.0);
        }
        report("long", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
