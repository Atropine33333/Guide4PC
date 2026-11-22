// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <string_view>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool aliases(std::string_view view, const std::string& source, std::size_t offset) {
    return view.data() == source.data() + offset;
}

}  // namespace

int main() {
    {
        const std::string s = "  hi  ";
        const std::string_view view = trim(s);
        report("trim-both-ends", view == "hi" && view.size() == 2 && aliases(view, s, 2));
    }

    {
        const std::string spaces = "   ";
        const std::string plain = "abc";
        const std::string empty;
        const bool ok = trim(spaces).empty() && trim(plain) == "abc" &&
                        aliases(trim(plain), plain, 0) && trim(empty).empty();
        report("trim-edges", ok);
    }

    {
        const std::string s = "  alpha beta";
        const std::string_view word = first_word(s);
        const std::string blanks = "   ";
        const bool ok = word == "alpha" && aliases(word, s, 2) &&
                        first_word(blanks).empty() && first_word("x") == "x" &&
                        first_word("").empty();
        report("first-word", ok);
    }

    {
        const std::string s = "[abc]x";
        const std::string_view inner = between(s, '[', ']');
        const bool ok = inner == "abc" && aliases(inner, s, 1) &&
                        between("[abc", '[', ']').empty() &&
                        between("a]b[c", '[', ']').empty() &&
                        between("[]", '[', ']').empty();
        report("between", ok);
    }

    {
        const std::string s = "prefix-tail";
        const bool ok = starts_with(s, "pre") && starts_with(s, "") &&
                        starts_with(s, s) && !starts_with("pre", "prefix") &&
                        !starts_with(s, "prefixtailx") && !starts_with(s, "Prefix");
        report("starts-with", ok);
    }

    {
        const std::string s = " view ";
        const std::string_view view = trim(s);
        const bool ok = !view.empty() && view.back() == 'w' &&
                        aliases(view, s, 1) && view.substr(1) == "iew";
        report("view-aliases-source", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
