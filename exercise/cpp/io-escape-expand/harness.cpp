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
    {
        const bool ok = expand_escapes("hello") == "hello" &&
                        expand_escapes("a b c") == "a b c" &&
                        expand_escapes("!@#$%^&*()") == "!@#$%^&*()";
        report("none", ok);
    }

    {
        const bool ok = expand_escapes("a\\nb") == "a\nb" &&
                        expand_escapes("x\\ty") == "x\ty" &&
                        expand_escapes("\\n") == "\n" &&
                        expand_escapes("\\t") == "\t";
        report("newline-tab", ok);
    }

    {
        const bool ok = expand_escapes("a\\\\b") == "a\\b" &&
                        expand_escapes("\\\\") == "\\" &&
                        expand_escapes("\\\\\\\\") == "\\\\";
        report("backslash", ok);
    }

    {
        const bool ok = expand_escapes("\\q") == "\\q" &&
                        expand_escapes("a\\zb") == "a\\zb" &&
                        expand_escapes("\\1") == "\\1" &&
                        expand_escapes("\\ ") == "\\ ";
        report("unknown", ok);
    }

    {
        const bool ok = expand_escapes("").empty();
        report("empty", ok);
    }

    {
        const bool ok = expand_escapes("line1\\nline2\\t\\\\end") ==
                        "line1\nline2\t\\end";
        report("mixed", ok);
    }

    {
        const bool ok = expand_escapes("a\\rb") == "a\rb" &&
                        expand_escapes("\\r\\n") == "\r\n";
        report("cr", ok);
    }

    {
        const bool ok = expand_escapes("abc\\") == "abc\\" &&
                        expand_escapes("\\") == "\\" &&
                        expand_escapes("\\\\\\") == "\\\\";
        report("trailing", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
