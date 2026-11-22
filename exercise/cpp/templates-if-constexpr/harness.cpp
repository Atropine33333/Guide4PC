// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <array>
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

struct Point {
    int x = 0;
    int y = 0;
};

}  // namespace

int main() {
    {
        const bool ok = classify(0) == "integral" && classify(-5) == "integral" &&
                        classify(static_cast<unsigned char>('a')) == "integral" &&
                        classify(1234567890123LL) == "integral";
        report("classify-integral", ok);
    }

    {
        const bool ok = classify(3.5) == "floating" && classify(1.0f) == "floating" &&
                        classify(2.0L) == "floating";
        report("classify-floating", ok);
    }

    {
        const bool ok = classify(std::string("hi")) == "string" && classify("hi") == "other";
        report("classify-string", ok);
    }

    {
        const bool ok = classify(std::array<int, 2>{1, 2}) == "other" &&
                        classify(Point{1, 2}) == "other" && classify(nullptr) == "other";
        report("classify-other", ok);
    }

    {
        const int number = 7;
        const int& number_ref = number;
        std::string text = "abc";
        const std::string& text_ref = text;
        bool flag = true;
        const bool ok = classify(number_ref) == "integral" && classify(text_ref) == "string" &&
                        classify(flag) == "integral" && classify(static_cast<const double&>(3.0)) ==
                                                              "floating";
        report("classify-decay", ok);
    }

    {
        const bool ok = render(42) == "42" && render(-7) == "-7" && render(true) == "1" &&
                        render(static_cast<long long>(9000000000LL)) == "9000000000";
        report("render-integral", ok);
    }

    {
        const bool ok = render(3.14159) == "3.14" && render(2.5) == "2.50" &&
                        render(-0.001) == "-0.00" && render(1.0f) == "1.00";
        report("render-floating", ok);
    }

    {
        const bool ok = render(std::string("hey")) == "hey" &&
                        render(std::array<int, 2>{1, 2}) == "?" && render(Point{3, 4}) == "?";
        report("render-other", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
