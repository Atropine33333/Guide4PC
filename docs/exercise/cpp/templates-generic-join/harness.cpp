// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <deque>
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
    report("empty", join(std::vector<int>{}, ", ").empty() &&
                        join(std::vector<std::string>{}, "-").empty());

    report("single", join(std::vector<int>{5}, ", ") == "5" &&
                         join(std::vector<std::string>{"solo"}, "-") == "solo");

    report("ints", join(std::vector<int>{1, 2, 3}, ", ") == "1, 2, 3" &&
                       join(std::vector<int>{4, 5}, "") == "45" &&
                       join(std::vector<int>{-1, 0, 1}, "+") == "-1+0+1");

    report("strings", join(std::vector<std::string>{"a", "b"}, "-") == "a-b" &&
                          join(std::vector<std::string>{"", "x", ""}, "/") == "/x/" &&
                          join(std::vector<std::string>{"one"}, "::") == "one");

    report("deque", join(std::deque<int>{7, 8, 9}, " -> ") == "7 -> 8 -> 9");

    report("double", join(std::vector<double>{1.5, 2.25}, ";") == "1.5;2.25");

    {
        std::string expected;
        for (int i = 0; i < 100; ++i) {
            if (i > 0) expected += ',';
            expected += '9';
        }
        report("long", join(std::vector<int>(100, 9), ",") == expected);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
