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
        Trie trie;
        report("empty", trie.size() == 0 && trie.count_prefix("") == 0 &&
                            !trie.contains(""));
    }

    {
        Trie trie;
        trie.insert("a");
        trie.insert("ab");
        trie.insert("abc");
        const bool ok = trie.size() == 3 && trie.count_prefix("") == 3 &&
                        trie.count_prefix("a") == 3 &&
                        trie.count_prefix("ab") == 2 &&
                        trie.count_prefix("abc") == 1 &&
                        trie.count_prefix("b") == 0 &&
                        trie.contains("a") && trie.contains("ab") &&
                        trie.contains("abc") && !trie.contains("abx");
        report("chain", ok);
    }

    {
        Trie trie;
        trie.insert("x");
        trie.insert("x");
        trie.insert("xyz");
        report("duplicates", trie.size() == 3 && trie.count_prefix("x") == 3 &&
                                 trie.count_prefix("xy") == 1 &&
                                 trie.contains("x"));
    }

    {
        Trie trie;
        trie.insert("cat");
        trie.insert("car");
        trie.insert("dog");
        const bool ok = trie.count_prefix("ca") == 2 &&
                        trie.count_prefix("car") == 1 &&
                        trie.count_prefix("care") == 0 &&
                        !trie.contains("ca") && trie.contains("car") &&
                        trie.count_prefix("d") == 1;
        report("branching", ok);
    }

    {
        Trie trie;
        trie.insert("");
        trie.insert("abc");
        report("empty-word", trie.size() == 2 && trie.contains("") &&
                                 trie.count_prefix("") == 2 &&
                                 trie.count_prefix("a") == 1);
    }

    {
        Trie trie;
        std::string long_word(1000, 'a');
        trie.insert(long_word);
        trie.insert(long_word);
        report("long", trie.count_prefix(long_word) == 2 &&
                           trie.contains(long_word) &&
                           trie.count_prefix(std::string(999, 'a')) == 2);
    }

    {
        Trie trie;
        for (int i = 0; i < 1000; ++i) trie.insert("word");
        report("many", trie.size() == 1000 && trie.count_prefix("w") == 1000 &&
                           trie.count_prefix("word") == 1000);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
