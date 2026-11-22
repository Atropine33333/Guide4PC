// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>
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

static_assert(Numeric<int>);
static_assert(Numeric<long long>);
static_assert(Numeric<double>);
static_assert(Numeric<float>);
static_assert(Numeric<unsigned>);
static_assert(!Numeric<bool>);
static_assert(Numeric<char>);
static_assert(!Numeric<std::string>);

int main() {
    report("concept", Numeric<int> && !Numeric<bool>);

    report("empty", average_of<int>({}, 42) == 42 &&
                        average_of<double>({}, -1.5) == -1.5);

    report("int", average_of<int>({1, 2}, 0) == 1 &&
                      average_of<int>({5}, 0) == 5 &&
                      average_of<int>({2, 4, 6}, 0) == 4);

    report("negative", average_of<int>({-1, -2}, 0) == -1 &&
                           average_of<int>({-1, 0}, 0) == 0 &&
                           average_of<int>({-3, -4}, 0) == -3);

    report("double", average_of<double>({1.5, 2.5}, 0.0) == 2.0 &&
                         average_of<double>({1.0}, 0.0) == 1.0);

    report("unsigned", average_of<unsigned>({1u, 2u}, 0u) == 1u &&
                           average_of<unsigned>({3u, 4u}, 0u) == 3u);

    report("large", average_of<long long>({1000000000LL, 1000000000LL,
                                           1000000000LL}, 0LL) == 1000000000LL);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
