// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <iostream>

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
        bool ok = Tracked::alive() == 0;
        {
            Tracked first;
            ok = ok && Tracked::alive() == 1 && first.value() == 0;
            {
                Tracked second(5);
                ok = ok && Tracked::alive() == 2 && second.value() == 5;
            }
            ok = ok && Tracked::alive() == 1;
        }
        ok = ok && Tracked::alive() == 0;
        report("construct-destruct", ok);
    }

    {
        bool ok = Tracked::alive() == 0;
        {
            Tracked original(3);
            Tracked copy(original);
            ok = ok && Tracked::alive() == 2;
            ok = ok && copy.value() == 3 && original.value() == 3;
            copy.setValue(8);
            ok = ok && copy.value() == 8 && original.value() == 3;
        }
        ok = ok && Tracked::alive() == 0;
        report("copy-constructor", ok);
    }

    {
        bool ok = Tracked::alive() == 0;
        {
            Tracked first(1);
            Tracked second(2);
            Tracked third(3);
            third = first;
            ok = ok && Tracked::alive() == 3;
            ok = ok && third.value() == 1 && first.value() == 1 && second.value() == 2;
        }
        ok = ok && Tracked::alive() == 0;
        report("assignment-count", ok);
    }

    {
        bool ok = Tracked::alive() == 0;
        {
            Tracked object(6);
            Tracked *alias = &object;
            object = *alias;
            ok = ok && Tracked::alive() == 1 && object.value() == 6;
        }
        ok = ok && Tracked::alive() == 0;
        report("self-assignment", ok);
    }

    {
        bool ok = Tracked::alive() == 0;
        {
            Tracked items[3];
            ok = ok && Tracked::alive() == 3;
            items[0].setValue(10);
            items[1].setValue(20);
            items[2].setValue(30);
            ok = ok && items[0].value() == 10 && items[1].value() == 20 && items[2].value() == 30;
            Tracked *last = &items[2];
            items[2] = items[0];
            ok = ok && Tracked::alive() == 3 && last->value() == 10;
        }
        ok = ok && Tracked::alive() == 0;
        report("array-lifetime", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
