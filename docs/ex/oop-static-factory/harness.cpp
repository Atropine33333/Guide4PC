// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <optional>
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

bool valid(const std::string &text) {
    const std::optional<Username> name = Username::create(text);
    return name.has_value() && name->value() == text &&
           name->length() == text.size();
}

bool invalid(const std::string &text) {
    return !Username::create(text).has_value();
}

}  // namespace

int main() {
    report("valid", valid("abc") && valid("alice_01") &&
                        valid("A1_2") && valid("a_b") &&
                        valid("abcdefghijklmnop"));

    report("length", invalid("") && invalid("ab") && invalid("a") &&
                         invalid("abcdefghijklmnopq") &&
                         valid("abc") && valid("abcdefghijklmnop"));

    report("first-char", invalid("1abc") && invalid("_abc") &&
                             invalid(" abc") && invalid("-abc") &&
                             invalid(".abc") && valid("Z9a"));

    report("characters", invalid("ab-c") && invalid("ab c") &&
                             invalid("ab.c") && invalid("ab!") &&
                             valid("ab_") && valid("a_b_9"));

    {
        const std::optional<Username> a = Username::create("alice");
        const std::optional<Username> b = Username::create("alice");
        const std::optional<Username> c = Username::create("alice2");
        const bool ok = a.has_value() && b.has_value() && c.has_value() &&
                        *a == *b && !(*a == *c) && a->value() == "alice" &&
                        a->length() == 5;
        report("equality", ok);
    }

    {
        const std::string longest(16, 'a');
        const std::string candidate = "a" + std::string(15, 'z');
        const bool ok = valid(longest) && valid(candidate) &&
                        invalid(longest + "a");
        report("boundaries", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
