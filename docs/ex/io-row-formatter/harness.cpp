// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <sstream>
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
    report("short", format_row("ann", 5) == "ann         |         5" &&
                        format_row("bob", -42) == "bob         |       -42");

    report("exact", format_row("abcdefghijkl", 1234567890) ==
                         "abcdefghijkl|1234567890");

    report("zero", format_row("", 0) == "            |         0" &&
                       format_row("x", 0) == "x           |         0");

    report("longer", format_row("averylongname", 12345678901LL) ==
                          "averylongname|12345678901" &&
                         format_row("abcdefghijklmnop", -1234567890LL) ==
                          "abcdefghijklmnop|-1234567890");

    report("max", format_row("m", 9223372036854775807LL) ==
                      "m           |9223372036854775807" &&
                      format_row("n", -9223372036854775807LL - 1) ==
                      "n           |-9223372036854775808");

    {
        std::ostringstream out;
        print_row(out, "cat", 7);
        print_row(out, "dog", -8);
        const bool ok = out.str() == "cat         |         7\n"
                                    "dog         |        -8\n";
        report("print-row", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
