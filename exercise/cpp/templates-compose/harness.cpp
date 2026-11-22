// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <utility>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

int plus_one(int value) { return value + 1; }
int times_two(int value) { return value * 2; }

struct Add {
    int amount;
    int operator()(int value) const { return value + amount; }
};

}  // namespace

int main() {
    {
        const auto add_then_double =
            compose([](int x) { return x * 2; }, [](int x) { return x + 1; });
        report("basic", add_then_double(3) == 8 && add_then_double(-1) == 0 &&
                            add_then_double(0) == 2);
    }

    {
        const auto length_after_bang = compose(
            [](const std::string& text) { return text.size(); },
            [](int value) { return std::to_string(value) + "!"; });
        const auto twice = compose(
            [](double value) { return static_cast<int>(value * 2); },
            [](int value) { return value / 2.0; });
        report("types", length_after_bang(41) == 3 && twice(5) == 5 &&
                            twice(7) == 7);
    }

    {
        const auto via_pointers = compose(plus_one, times_two);
        report("function-pointer", via_pointers(5) == 11 &&
                                       via_pointers(0) == 1);
    }

    {
        const auto h = [](int x) { return x + 1; };
        const auto g = [](int x) { return x * 3; };
        const auto f = [](int x) { return x - 2; };
        const auto first = compose(f, compose(g, h));
        report("nested", first(2) == 7 && first(-1) == -2);
    }

    {
        const auto add_three = compose(Add{3}, Add{4});
        report("functor", add_three(10) == 17 && add_three(0) == 7);
    }

    {
        int calls = 0;
        const auto counting = compose(
            [&calls](int x) {
                ++calls;
                return x * 10;
            },
            [](int x) { return x + 1; });
        const int first = counting(2);
        const int second = counting(4);
        report("stateful", first == 30 && second == 50 && calls == 2);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
