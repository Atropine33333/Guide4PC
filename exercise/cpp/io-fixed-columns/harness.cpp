// Guide4PC harness appended to the student's code in the same translation unit.
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

bool same_person(const Person& p, const std::string& name, int id, int score) {
    return p.name == name && p.id == id && p.score == score;
}

}  // namespace

int main() {
    {
        const Person p = parse_person("  42alice        950");
        report("full-row", same_person(p, "alice", 42, 950));
    }

    {
        const Person p = parse_person("   7bob       88");
        report("short-row", same_person(p, "bob", 7, 88));
    }

    {
        const Person p = parse_person(std::string(20, ' '));
        const Person empty = parse_person("");
        const bool ok = same_person(p, "", 0, 0) && same_person(empty, "", 0, 0);
        report("blank-fields", ok);
    }

    {
        const bool ok = slice("abcdefghij", 2, 4) == "cdef" &&
                        slice("abcdefghij", 0, 20) == "abcdefghij" &&
                        slice("abcdefghij", 10, 3).empty() &&
                        slice("abcdefghij", 9, 1) == "j";
        report("slice-basic", ok);
    }

    {
        const std::string line = "  ab  cd   ef  ";
        const bool ok = slice(line, 0, 4) == "ab" && slice(line, 4, 4) == "cd" &&
                        slice(line, 10, 3) == "ef" && slice(line, 2, 0).empty() &&
                        slice(line, -1, 4).empty() && slice("", 0, 5).empty();
        report("slice-spaces", ok);
    }

    {
        const Person p = parse_person("   9Mary Jane    100");
        report("name-with-space", same_person(p, "Mary Jane", 9, 100));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
