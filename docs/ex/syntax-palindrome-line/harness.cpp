// Guide4PC harness appended to the student's code in the same translation unit.
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
    report("empty", is_palindrome(""));

    report("single", is_palindrome("a") && is_palindrome("7") && is_palindrome("!"));

    {
        const bool ok = is_palindrome("A man, a plan, a canal: Panama") &&
                        is_palindrome("No 'x' in Nixon");
        report("phrase", ok);
    }

    {
        const bool ok = !is_palindrome("race a car") && !is_palindrome("hello");
        report("not-palindrome", ok);
    }

    {
        const bool ok = is_palindrome("!!!") && is_palindrome("   ") &&
                        is_palindrome("... ,, ;;");
        report("punctuation", ok);
    }

    {
        const bool ok = is_palindrome("12 21") && is_palindrome("1,2,1") &&
                        !is_palindrome("123");
        report("digits", ok);
    }

    {
        std::string long_text(200, 'x');
        const bool same = is_palindrome(long_text);
        long_text[199] = 'y';
        const bool different = !is_palindrome(long_text);
        report("long", same && different);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
