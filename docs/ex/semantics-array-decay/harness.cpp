// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    int values[6] = {2, 4, 6, 8, 10, 12};

    report("array-bytes", array_bytes(values) == sizeof(values) &&
                              array_bytes(values) == 6 * sizeof(int));

    report("pointer-bytes", pointer_bytes(values) == sizeof(int*));

    report("overload-array", which_overload(values) == "array");

    const bool pointer_ok = which_overload(values + 0) == "pointer" &&
                            which_overload(&values[0]) == "pointer" &&
                            which_overload(static_cast<const int*>(values)) == "pointer";
    report("overload-pointer", pointer_ok);

    report("sum-array", sum_array(values) == 42);

    const bool range_ok = sum_range(values, values + 6) == 42 &&
                          sum_range(values, values) == 0 &&
                          sum_range(values + 1, values + 4) == 18 &&
                          sum_range(values + 5, values + 6) == 12;
    report("sum-range", range_ok);

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
