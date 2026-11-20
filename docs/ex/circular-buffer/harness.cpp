// Guide4PC harness：被拼在学生代码之后，同一个翻译单元。
// 学生不写 main；这里的 main 负责跑完全部检查，按协议打印结果。
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
        CircularBuffer buffer(3);
        report("empty-size", buffer.size() == 0 && buffer.capacity() == 3);
        int value = 0;
        report("empty-pop", !buffer.pop(value));
    }

    {
        CircularBuffer buffer(3);
        const bool pushed = buffer.push(1) && buffer.push(2) && buffer.push(3);
        int first = 0, second = 0, third = 0;
        const bool popped = buffer.pop(first) && buffer.pop(second) && buffer.pop(third);
        report("fifo-order",
               pushed && popped && first == 1 && second == 2 && third == 3 && buffer.size() == 0);
    }

    {
        CircularBuffer buffer(2);
        const bool filled = buffer.push(10) && buffer.push(20);
        const bool rejected = !buffer.push(30);
        report("full-rejects", filled && rejected && buffer.size() == 2);
    }

    {
        CircularBuffer buffer(3);
        buffer.push(1);
        buffer.push(2);
        buffer.push(3);
        int discarded = 0;
        buffer.pop(discarded);
        const bool pushed = buffer.push(4);
        int second = 0, third = 0, fourth = 0;
        const bool popped = buffer.pop(second) && buffer.pop(third) && buffer.pop(fourth);
        report("wrap-around", pushed && popped && second == 2 && third == 3 && fourth == 4);
    }

    {
        CircularBuffer buffer(0);
        int value = 0;
        report("zero-capacity",
               buffer.size() == 0 && buffer.capacity() == 0 && !buffer.push(1) && !buffer.pop(value));
    }

    {
        CircularBuffer buffer(1);
        int value = 0;
        report("single-slot",
               buffer.push(7) && !buffer.push(8) && buffer.pop(value) && value == 7 &&
                   !buffer.pop(value));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
