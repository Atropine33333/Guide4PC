// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>
#include <type_traits>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    {
        report("size", TypeList<>::size == 0 &&
                           TypeList<int>::size == 1 &&
                           TypeList<int, double, std::string>::size == 3);
    }

    {
        report("front", std::is_same_v<TypeList<int, double>::front, int> &&
                            std::is_same_v<TypeList<std::string>::front,
                                           std::string>);
    }

    {
        using empty_front = TypeList<>::push_front<int>;
        using list = TypeList<int, double>;
        using extended = list::push_front<std::string>;
        report("push-front",
               std::is_same_v<empty_front, TypeList<int>> &&
                   std::is_same_v<extended, TypeList<std::string, int, double>> &&
                   extended::size == 3);
    }

    {
        using list = TypeList<int, double, int>;
        report("contains", list::contains<int> && list::contains<double> &&
                               !list::contains<std::string> &&
                               !list::contains<float> &&
                               !TypeList<>::contains<int>);
    }

    {
        using list = TypeList<int, double, std::string>;
        report("at", std::is_same_v<list::at<0>, int> &&
                         std::is_same_v<list::at<1>, double> &&
                         std::is_same_v<list::at<2>, std::string>);
    }

    {
        using chain = TypeList<>::push_front<int>::push_front<double>::
            push_front<std::string>;
        report("chain", chain::size == 3 &&
                            std::is_same_v<chain::front, std::string> &&
                            chain::contains<int> &&
                            chain::contains<double> &&
                            chain::contains<std::string> &&
                            std::is_same_v<chain::at<2>, int>);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
