// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <variant>

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
    const Value i = 42;
    const Value d = 3.5;
    const Value s = std::string("hello");

    report("kind", kind_of(i) == "int" && kind_of(d) == "double" &&
                       kind_of(s) == "string" &&
                       kind_of(Value{std::string("")}) == "string");

    report("as-double", as_double(i) == 42.0 && as_double(d) == 3.5 &&
                            as_double(s) == 0.0 && as_double(Value{-8}) == -8.0 &&
                            as_double(Value{0.125}) == 0.125);

    report("describe-int", describe(Value{42}) == "int(42)" &&
                               describe(Value{-7}) == "int(-7)" &&
                               describe(Value{0}) == "int(0)");

    report("describe-double", describe(Value{3.5}) == "double(3.50)" &&
                                  describe(Value{-0.25}) == "double(-0.25)" &&
                                  describe(Value{100.0}) == "double(100.00)" &&
                                  describe(Value{0.5}) == "double(0.50)");

    report("describe-string", describe(std::string("hello")) == "string(hello)" &&
                                  describe(std::string("")) == "string()" &&
                                  describe(std::string("a b")) == "string(a b)");

    {
        Value changing = 1;
        const bool first = kind_of(changing) == "int" && as_double(changing) == 1.0;
        changing = 2.5;
        const bool second = kind_of(changing) == "double" && as_double(changing) == 2.5;
        changing = std::string("x");
        const bool third = kind_of(changing) == "string" && as_double(changing) == 0.0 &&
                           describe(changing) == "string(x)";
        report("reassign", first && second && third);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
