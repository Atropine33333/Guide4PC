// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
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
    {
        std::vector<int> a{1, 2, 3, 4, 5};
        rotate_left(a, 2);
        const bool ok = a == std::vector<int>({3, 4, 5, 1, 2});
        report("basic", ok);
    }

    {
        std::vector<int> zero{1, 2, 3, 4};
        rotate_left(zero, 0);
        std::vector<int> same{1, 2, 3, 4};
        rotate_left(same, 4);
        const bool ok = zero == std::vector<int>({1, 2, 3, 4}) &&
                        same == std::vector<int>({1, 2, 3, 4});
        report("zero-and-size", ok);
    }

    {
        std::vector<int> big{10, 20, 30, 40, 50};
        rotate_left(big, 1000000000);
        std::vector<int> big2{10, 20, 30, 40, 50};
        rotate_left(big2, 999999999);
        const bool ok = big == std::vector<int>({10, 20, 30, 40, 50}) &&
                        big2 == std::vector<int>({50, 10, 20, 30, 40});
        report("large", ok);
    }

    {
        std::vector<int> right{1, 2, 3, 4, 5};
        rotate_left(right, -1);
        std::vector<int> right_two{1, 2, 3, 4, 5};
        rotate_left(right_two, -12);
        const bool ok = right == std::vector<int>({5, 1, 2, 3, 4}) &&
                        right_two == std::vector<int>({4, 5, 1, 2, 3});
        report("negative", ok);
    }

    {
        std::vector<int> empty;
        rotate_left(empty, 12345);
        rotate_left(empty, -12345);
        report("empty", empty.empty());
    }

    {
        std::vector<int> one{7};
        rotate_left(one, 99);
        rotate_left(one, -99);
        report("single", one == std::vector<int>({7}));
    }

    {
        std::vector<int> a{1, 2, 3};
        rotate_left(a, -3);
        std::vector<int> b{1, 2, 3};
        rotate_left(b, -6);
        std::vector<int> c{1, 2, 3};
        rotate_left(c, 6);
        const bool ok = a == std::vector<int>({1, 2, 3}) &&
                        b == std::vector<int>({1, 2, 3}) &&
                        c == std::vector<int>({1, 2, 3});
        report("negative-multiple", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
