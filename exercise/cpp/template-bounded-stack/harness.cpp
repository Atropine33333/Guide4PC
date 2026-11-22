// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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

}  // namespace

int main() {
    {
        BoundedStack<int, 3> stack;
        int top = 0;
        const bool ok = stack.size() == 0 && stack.push(1) && stack.push(2) &&
                        stack.size() == 2 && stack.top(top) && top == 2 &&
                        stack.pop() && stack.top(top) && top == 1;
        report("int-basics", ok);
    }

    {
        BoundedStack<int, 3> stack;
        bool ok = stack.push(1) && stack.push(2) && stack.push(3);
        ok = ok && !stack.push(4) && stack.size() == 3;
        int top = 0;
        ok = ok && stack.top(top) && top == 3;
        report("overflow", ok);
    }

    {
        BoundedStack<int, 1> stack;
        int top = 0;
        bool ok = BoundedStack<int, 1>::capacity() == 1;
        ok = ok && stack.push(5) && !stack.push(6) && stack.size() == 1 &&
             stack.top(top) && top == 5 && stack.pop() && !stack.pop() && stack.size() == 0;
        report("capacity-one", ok);
    }

    {
        BoundedStack<std::string, 2> stack;
        std::string top;
        bool ok = stack.push("a") && stack.push("b") && !stack.push("c");
        ok = ok && stack.top(top) && top == "b" && stack.pop();
        ok = ok && stack.top(top) && top == "a" && stack.pop() && !stack.top(top);
        report("string-stack", ok);
    }

    {
        BoundedStack<double, 2> stack;
        double top = 0.0;
        bool ok = stack.push(1.5) && stack.push(-2.25) && stack.size() == 2;
        ok = ok && stack.top(top) && top == -2.25 && stack.pop();
        ok = ok && stack.top(top) && top == 1.5;
        report("double-stack", ok);
    }

    {
        BoundedStack<int, 2> first;
        BoundedStack<int, 2> second;
        int top = 0;
        const bool ok = first.push(1) && second.push(2) &&
                        first.size() == 1 && second.size() == 1 &&
                        first.top(top) && top == 1 &&
                        second.top(top) && top == 2;
        report("independent-stacks", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
