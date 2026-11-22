// Guide4PC harness appended to the student's code in the same translation unit.
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

}  // namespace

int main() {
    {
        Notebook book;
        const bool ok =
            book.size() == 0 && book.at(0) == "?" && book.join(",").empty();
        report("empty-notebook", ok);
    }

    {
        Notebook book;
        book.add("one");
        book.add("two");
        book.add("three");
        const bool ok = book.size() == 3 && book.at(0) == "one" &&
                        book.at(1) == "two" && book.at(2) == "three" &&
                        book.at(3) == "?";
        report("add-and-at", ok);
    }

    {
        Notebook book;
        book.add("a");
        book.add("b");
        const bool ok = book.join(" | ") == "a | b" && book.join("") == "ab";
        report("join", ok);
    }

    {
        Notebook original;
        original.add("keep");
        Notebook copy = original;
        copy.add("extra");
        const bool ok = original.size() == 1 && original.at(0) == "keep" &&
                        copy.size() == 2 && copy.at(1) == "extra";
        report("copy-independent", ok);
    }

    {
        Notebook original;
        original.add("alpha");
        original.add("beta");
        Notebook assigned;
        assigned.add("old");
        assigned = original;
        original.add("gamma");
        const bool ok = assigned.size() == 2 && assigned.at(0) == "alpha" &&
                        assigned.at(1) == "beta" &&
                        original.size() == 3;
        report("assign-independent", ok);
    }

    {
        Notebook source;
        source.add("moved");
        source.add("content");
        Notebook target = std::move(source);
        const bool ok = target.size() == 2 && target.at(0) == "moved" &&
                        target.at(1) == "content";
        report("move-target", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
