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

int main() {
    {
        const bool ok = clamp_value(50) == 50 && clamp_value(-5) == 0 &&
                        clamp_value(150) == 100;
        report("default-clamp", ok);
    }

    {
        const bool ok = clamp_value(5, 3) == 5 && clamp_value(2, 3) == 3 &&
                        clamp_value(9, 3) == 9 && clamp_value(150, 0, 1000) == 150;
        report("one-override", ok);
    }

    {
        const bool ok = clamp_value(-10, -5, 5) == -5 && clamp_value(10, -5, 5) == 5 &&
                        clamp_value(0, -5, 5) == 0;
        report("two-overrides", ok);
    }

    {
        const bool ok = clamp_value(5, 10, 0) == 5 && clamp_value(-1, 10, 0) == 0 &&
                        clamp_value(11, 10, 0) == 10;
        report("inverted-bounds", ok);
    }

    {
        const std::vector<std::string> empty;
        const bool ok = join_text(empty).empty() &&
                        join_text({"a"}) == "a" &&
                        join_text({"a", "b"}) == "a,b" &&
                        join_text({"a", "b", "c"}, " | ") == "a | b | c";
        report("join-default", ok);
    }

    {
        const bool ok = join_text({"a", "b"}, "") == "ab" &&
                        join_text({"", ""}, ",") == "," &&
                        join_text({"x", "", "y"}, "--") == "x----y";
        report("join-custom", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
