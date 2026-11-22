// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
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
        TokenBucket bucket(5, 2);
        report("initial", bucket.tokens() == 5 && bucket.capacity() == 5);
    }

    {
        TokenBucket bucket(3, 0);
        const bool first = bucket.take(0) && bucket.tokens() == 2;
        const bool second = bucket.take(0) && bucket.take(0);
        const bool third = !bucket.take(0) && bucket.tokens() == 0;
        report("take", first && second && third);
    }

    {
        TokenBucket bucket(3, 1);
        bucket.take(0);
        bucket.take(0);
        bucket.take(0);
        const bool empty = !bucket.take(0);
        const bool refilled = bucket.take(1) && bucket.tokens() == 0;
        report("refill", empty && refilled);
    }

    {
        TokenBucket bucket(3, 100);
        bucket.advance(1);
        const bool capped = bucket.tokens() == 3;
        const bool taken = bucket.take(0) && bucket.tokens() == 2;
        bucket.advance(1000);
        report("cap", capped && taken && bucket.tokens() == 3);
    }

    {
        TokenBucket bucket(5, 2);
        bucket.advance(1);
        const bool full = bucket.tokens() == 5;
        bucket.take(0);
        bucket.advance(2);
        report("advance", full && bucket.tokens() == 5);
    }

    {
        TokenBucket bucket(0, 5);
        const bool ok = bucket.take(10) == false && bucket.tokens() == 0 &&
                        bucket.capacity() == 0 && bucket.take(0) == false;
        report("zero-capacity", ok);
    }

    {
        TokenBucket bucket(2, 0);
        const bool first = bucket.take(0) && bucket.take(0);
        const bool second = !bucket.take(1000000) && bucket.tokens() == 0;
        report("zero-refill", first && second);
    }

    {
        TokenBucket bucket(1000000000, 1000000000);
        bucket.advance(1000000000);
        const bool full = bucket.tokens() == 1000000000;
        const bool taken = bucket.take(0) && bucket.tokens() == 999999999;
        report("large", full && taken);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
