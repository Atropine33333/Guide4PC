// Guide4PC harness appended to the student's code in the same translation unit.
#include <algorithm>
#include <cstddef>
#include <cstdint>
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

struct h_rng {
    std::uint32_t state;
    std::uint32_t next() {
        state = state * 1664525u + 1013904223u;
        return state >> 8;
    }
    int below(int limit) { return static_cast<int>(next() % static_cast<std::uint32_t>(limit)); }
};

std::string h_word(h_rng &rng, int max_len) {
    const int len = rng.below(max_len + 1);
    std::string out;
    for (int i = 0; i < len; ++i) out.push_back(static_cast<char>('a' + rng.below(3)));
    return out;
}

int h_len(const std::string &s) { return static_cast<int>(s.size()); }

}  // namespace

int main() {
    {
        const bool ok = edit_distance("", "") == 0 && edit_distance("", "abc") == 3 &&
                        edit_distance("abc", "") == 3 &&
                        longest_common_subsequence("", "abc") == 0 &&
                        longest_common_subsequence("abc", "") == 0;
        report("empty", ok);
    }

    {
        const bool ok = edit_distance("abcd", "abcd") == 0 &&
                        longest_common_subsequence("abcd", "abcd") == 4 &&
                        edit_distance("aaaa", "aaaa") == 0 &&
                        longest_common_subsequence("aaaa", "aaaa") == 4;
        report("identical", ok);
    }

    {
        const bool ok = edit_distance("kitten", "sitting") == 3 &&
                        longest_common_subsequence("kitten", "sitting") == 4 &&
                        edit_distance("intention", "execution") == 5 &&
                        longest_common_subsequence("intention", "execution") == 5;
        report("classic", ok);
    }

    {
        const bool ok = edit_distance("abc", "abxc") == 1 &&
                        edit_distance("abxc", "abc") == 1 &&
                        edit_distance("abc", "abd") == 1 &&
                        edit_distance("abc", "acb") == 2 &&
                        longest_common_subsequence("abc", "acb") == 2;
        report("edits", ok);
    }

    {
        h_rng rng{12345u};
        bool ok = true;
        for (int i = 0; i < 40 && ok; ++i) {
            const std::string a = h_word(rng, 8);
            const std::string b = h_word(rng, 8);
            const int d_ab = edit_distance(a, b);
            const int d_ba = edit_distance(b, a);
            const int l_ab = longest_common_subsequence(a, b);
            const int l_ba = longest_common_subsequence(b, a);
            if (d_ab != d_ba || l_ab != l_ba) ok = false;
            if (edit_distance(a, a) != 0) ok = false;
            if (longest_common_subsequence(a, a) != h_len(a)) ok = false;
            if (d_ab < 0 || d_ab > std::max(h_len(a), h_len(b))) ok = false;
            if (l_ab < 0 || l_ab > std::min(h_len(a), h_len(b))) ok = false;
        }
        report("symmetry", ok);
    }

    {
        h_rng rng{777u};
        bool ok = true;
        for (int i = 0; i < 40 && ok; ++i) {
            const std::string a = h_word(rng, 6);
            const std::string b = h_word(rng, 6);
            const std::string c = h_word(rng, 6);
            if (edit_distance(a, c) > edit_distance(a, b) + edit_distance(b, c)) ok = false;
            if (edit_distance(a, b) < std::abs(h_len(a) - h_len(b))) ok = false;
        }
        report("triangle", ok);
    }

    {
        std::string a;
        std::string b;
        for (int i = 0; i < 50; ++i) {
            a += "abcd";
            b += "abce";
        }
        const bool ok = a.size() == 200 && b.size() == 200 && edit_distance(a, b) == 50 &&
                        longest_common_subsequence(a, b) == 150;
        report("long", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
