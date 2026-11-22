// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>
#include <vector>

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
        const AnyValue value;
        report("empty-state",
               value.empty() && !value.is_int() && !value.is_string() &&
                   value.as_int(7) == 7 && value.as_string("none") == "none" &&
                   value.describe() == "empty");
    }

    {
        const AnyValue value(42);
        report("int-state", !value.empty() && value.is_int() &&
                                !value.is_string() && value.as_int(0) == 42 &&
                                value.as_string("x") == "x" &&
                                value.describe() == "int:42");
    }

    {
        const AnyValue zero(0);
        const AnyValue negative(-7);
        report("int-edges", zero.is_int() && zero.as_int(9) == 0 &&
                                negative.describe() == "int:-7" &&
                                negative.as_int(0) == -7);
    }

    {
        const AnyValue empty_text{std::string()};
        const AnyValue text(std::string("hello world"));
        report("string-state",
               empty_text.is_string() && empty_text.as_string("fallback").empty() &&
                   empty_text.as_int(3) == 3 && empty_text.describe() == "string:" &&
                   text.describe() == "string:hello world" &&
                   text.as_string("fallback") == "hello world");
    }

    {
        AnyValue original(5);
        AnyValue copy = original;
        copy = AnyValue(std::string("hi"));
        const bool ok = original.is_int() && original.as_int(0) == 5 &&
                        copy.is_string() && copy.as_string("") == "hi";
        report("copy-deep", ok);
    }

    {
        AnyValue a(1);
        AnyValue b(std::string("bee"));
        a = b;
        const bool first = a.is_string() && a.as_string("") == "bee";
        AnyValue& alias = a;
        a = alias;
        const bool second = a.is_string() && a.as_string("") == "bee";
        report("assign-and-self-assign", first && second);
    }

    {
        std::vector<AnyValue> values;
        values.push_back(AnyValue(10));
        for (int i = 0; i < 5; ++i) {
            values.push_back(values[0]);
        }
        values[0] = AnyValue(std::string("changed"));
        bool ok = values[0].is_string() && values[0].as_string("") == "changed";
        for (std::size_t i = 1; i < values.size(); ++i) {
            ok = ok && values[i].is_int() && values[i].as_int(0) == 10;
        }
        report("independent-copies", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
