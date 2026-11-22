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
    {
        const int before = TextStats::computations();
        TextStats stats("hello world");
        const bool ok = stats.word_count() == 2 && TextStats::computations() - before == 1;
        report("words", ok);
    }

    {
        const int before = TextStats::computations();
        TextStats stats("one two three");
        const int first = stats.word_count();
        const int after_first = TextStats::computations();
        const int second = stats.word_count();
        const bool ok = first == 3 && second == 3 && after_first - before == 1 &&
                        TextStats::computations() - before == 1;
        report("cached", ok);
    }

    {
        const int before = TextStats::computations();
        TextStats stats("ab12 c!");
        const bool ok = stats.letter_count() == 3 && TextStats::computations() - before == 1;
        report("letters", ok);
    }

    {
        const int before = TextStats::computations();
        TextStats stats("one");
        const int first = stats.word_count();
        const int after_first = TextStats::computations() - before;
        stats.append(" two");
        const int second = stats.word_count();
        const int after_second = TextStats::computations() - before;
        const bool ok = first == 1 && after_first == 1 && second == 2 && after_second == 2;
        report("append-words", ok);
    }

    {
        const int before = TextStats::computations();
        TextStats stats("abc");
        const int first = stats.letter_count();
        stats.append(" de");
        const int second = stats.letter_count();
        const bool ok = first == 3 && second == 5 && TextStats::computations() - before == 2;
        report("append-letters", ok);
    }

    {
        const int before = TextStats::computations();
        TextStats stats("red green blue");
        const TextStats& constant = stats;
        const int words = constant.word_count();
        const int letters = constant.letter_count();
        const int words_again = constant.word_count();
        const bool ok = words == 3 && letters == 12 && words_again == 3 &&
                        TextStats::computations() - before == 2;
        report("const-alias", ok);
    }

    {
        TextStats stats("");
        const bool ok = stats.word_count() == 0 && stats.letter_count() == 0;
        report("empty", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
