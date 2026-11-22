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

bool h_fields(const std::vector<std::string> &got,
              const std::vector<std::string> &want) {
    return got == want;
}

}  // namespace

int main() {
    report("quote-plain", csv_quote("abc") == "abc" && csv_quote("a b") == "a b" &&
                              csv_quote("") == "");

    report("quote-comma", csv_quote("a,b") == "\"a,b\"" &&
                              csv_quote(",") == "\",\"" &&
                              csv_quote("a,b,c") == "\"a,b,c\"");

    report("quote-double", csv_quote("say \"hi\"") == "\"say \"\"hi\"\"\"" &&
                               csv_quote("\"") == "\"\"\"\"");

    report("quote-newline", csv_quote("a\nb") == "\"a\nb\"");

    report("split-simple",
           h_fields(csv_split("a,b,c"), {"a", "b", "c"}) &&
               h_fields(csv_split(""), {""}) &&
               h_fields(csv_split("a,"), {"a", ""}) &&
               h_fields(csv_split(",,"), {"", "", ""}));

    report("split-quoted", h_fields(csv_split("\"a,b\",c"), {"a,b", "c"}) &&
                               h_fields(csv_split("x,\"y z\",w"), {"x", "y z", "w"}));

    report("split-escaped",
           h_fields(csv_split("\"he said \"\"hi\"\"\",x"), {"he said \"hi\"", "x"}) &&
               h_fields(csv_split("\"a\"\"b\"\"c\""), {"a\"b\"c"}));

    {
        const std::string samples[] = {"abc", "a,b", "say \"hi\"", "a\nb", "",
                                       "x y", "-_.~"};
        bool ok = true;
        for (const std::string &a : samples) {
            for (const std::string &b : samples) {
                const std::string line = csv_quote(a) + "," + csv_quote(b);
                if (!h_fields(csv_split(line), {a, b})) ok = false;
            }
        }
        report("round-trip", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
