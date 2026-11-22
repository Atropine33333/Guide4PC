// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <tuple>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

struct PlusOne {
    int operator()(int value) const { return value + 1; }
};

struct Twice {
    int operator()(int value) const { return value * 2; }
    double operator()(double value) const { return value * 2; }
    std::string operator()(const std::string& value) const {
        return value + value;
    }
};

int negate(int value) { return -value; }

}  // namespace

int main() {
    {
        const auto result = tuple_map(std::make_tuple(1, 2, 3), PlusOne{});
        report("homogeneous", result == std::make_tuple(2, 3, 4));
    }

    {
        const auto result = tuple_map(
            std::make_tuple(2, 2.5, std::string("hi")), Twice{});
        report("heterogeneous", result ==
                                    std::make_tuple(4, 5.0, std::string("hihi")) &&
                                    std::get<2>(result).size() == 4);
    }

    {
        const auto result = tuple_map(std::tuple<>{}, PlusOne{});
        report("empty", std::tuple_size_v<decltype(result)> == 0 &&
                            result == std::tuple<>{});
    }

    {
        const auto input = std::make_tuple(1, -2, 3);
        const auto result = tuple_map(input, PlusOne{});
        report("const", result == std::make_tuple(2, -1, 4) &&
                            std::get<0>(input) == 1);
    }

    {
        const auto result = tuple_map(std::make_tuple(9), negate);
        report("single", std::tuple_size_v<decltype(result)> == 1 &&
                             std::get<0>(result) == -9);
    }

    {
        const auto result = tuple_map(std::make_tuple(1, 2, 4), negate);
        report("function-pointer", result == std::make_tuple(-1, -2, -4));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
