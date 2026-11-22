// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>
#include <utility>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool matches(const IntVector& values, const int* expected, int count) {
    if (values.size() != count) return false;
    for (int i = 0; i < count; ++i) {
        if (values[i] != expected[i]) return false;
    }
    return true;
}

}  // namespace

int main() {
    {
        IntVector values;
        const bool ok = values.size() == 0 && values.capacity() == 0 &&
                        values.data() == nullptr;
        report("default", ok);
    }

    {
        IntVector values;
        values.push_back(1);
        values.push_back(2);
        values.push_back(3);
        const int expected[] = {1, 2, 3};
        report("push-preserve", matches(values, expected, 3) &&
                                values.data() != nullptr);
    }

    {
        IntVector values;
        bool ok = true;
        for (int i = 0; i < 100; ++i) {
            values.push_back(i * 2);
            if (values.capacity() < values.size()) ok = false;
        }
        for (int i = 0; ok && i < 100; ++i) {
            if (values[i] != i * 2) ok = false;
        }
        report("growth", ok);
    }

    {
        IntVector values;
        for (int i = 0; i < 10; ++i) values.push_back(i);
        values.clear();
        const bool empty_after = values.size() == 0;
        values.push_back(42);
        report("clear", empty_after && values.size() == 1 && values[0] == 42);
    }

    {
        IntVector original;
        original.push_back(1);
        original.push_back(2);
        IntVector copy = original;
        const bool same = copy.size() == 2 && copy[0] == 1 && copy[1] == 2;
        copy[0] = 99;
        copy.push_back(3);
        const bool independent = original.size() == 2 && original[0] == 1 &&
                                 copy.size() == 3 && copy.capacity() >= 3;
        report("copy", same && independent);
    }

    {
        IntVector source;
        source.push_back(5);
        source.push_back(6);
        IntVector target;
        target.push_back(0);
        target = source;
        source[0] = 50;
        const bool ok = target.size() == 2 && target[0] == 5 && target[1] == 6 &&
                        target.capacity() >= 2;
        report("assign", ok);
    }

    {
        IntVector values;
        values.push_back(1);
        values.push_back(2);
        IntVector& reference = values;
        values = reference;
        report("self-assign", values.size() == 2 && values[0] == 1 && values[1] == 2);
    }

    {
        IntVector source;
        for (int i = 0; i < 5; ++i) source.push_back(i);
        IntVector moved(std::move(source));
        bool ok = moved.size() == 5 && moved[0] == 0 && moved[4] == 4 &&
                  source.size() == 0 && source.data() == nullptr;
        source.push_back(77);
        ok = ok && source.size() == 1 && source[0] == 77;
        report("move-ctor", ok);
    }

    {
        IntVector source;
        source.push_back(8);
        source.push_back(9);
        IntVector target;
        target.push_back(1);
        target = std::move(source);
        bool ok = target.size() == 2 && target[0] == 8 && target[1] == 9 &&
                  source.size() == 0 && source.data() == nullptr;
        source.push_back(1);
        target.push_back(10);
        ok = ok && source.size() == 1 && target.size() == 3;
        report("move-assign", ok);
    }

    {
        IntVector values;
        values.push_back(10);
        values.push_back(20);
        int& first = values[0];
        first = 11;
        const IntVector& read_only = values;
        const bool ok = values[0] == 11 && read_only[1] == 20 &&
                        values.data()[0] == 11;
        report("index", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
