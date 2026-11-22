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

bool same(const std::vector<std::string>& got,
          const std::vector<std::string>& want) {
    return got == want;
}

}  // namespace

int main() {
    {
        const std::vector<std::string> got = split_fields("", ',');
        const bool ok = got.empty() && join_fields({}, "-") == "" &&
                        join_fields({}, "") == "";
        report("empty", ok);
    }

    {
        const bool ok = same(split_fields("a,b,c", ','), {"a", "b", "c"}) &&
                        same(split_fields("abc", ','), {"abc"}) &&
                        same(split_fields("a", ','), {"a"});
        report("fields", ok);
    }

    {
        const bool ok = same(split_fields(",", ','), {"", ""}) &&
                        same(split_fields(",a", ','), {"", "a"}) &&
                        same(split_fields("a,", ','), {"a", ""}) &&
                        same(split_fields(",a,", ','), {"", "a", ""}) &&
                        same(split_fields(",,\t", ','), {"", "", "\t"});
        report("empty-fields", ok);
    }

    {
        const bool ok = same(split_fields(" a , b ", ','), {" a ", " b "}) &&
                        same(split_fields("a b c", ','), {"a b c"}) &&
                        same(split_fields("a\nb", ','), {"a\nb"});
        report("preserve", ok);
    }

    {
        const bool ok = join_fields({"a", "b", "c"}, "-") == "a-b-c" &&
                        join_fields({"a"}, "-") == "a" &&
                        join_fields({"", ""}, ",") == "," &&
                        join_fields({"x", "", "y"}, "--") == "x----y" &&
                        join_fields({"a", "b"}, "") == "ab";
        report("join", ok);
    }

    {
        const std::vector<std::string> original = {",a,,b,", "c"};
        const std::string text = ",a,,b,";
        const bool ok = join_fields(split_fields(text, ','), ",") == text &&
                        join_fields(split_fields("a\tb\t", '\t'), "\t") == "a\tb\t" &&
                        join_fields(original, ",") == ",a,,b,,c";
        report("roundtrip", ok);
    }

    {
        std::string text;
        for (int i = 0; i < 1000; ++i) {
            if (i != 0) text += ',';
            text += 'x';
        }
        const std::vector<std::string> got = split_fields(text, ',');
        bool ok = got.size() == 1000;
        for (std::size_t i = 0; ok && i < got.size(); ++i) {
            if (got[i] != "x") ok = false;
        }
        report("long", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
