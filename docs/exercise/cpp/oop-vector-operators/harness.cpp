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

bool same(const Vec2& value, double x, double y) {
    return value.x == x && value.y == y;
}

}  // namespace

int main() {
    {
        Vec2 value(1.0, 2.0);
        Vec2 zero;
        report("construction", same(value, 1.0, 2.0) && same(zero, 0.0, 0.0));
    }

    {
        const Vec2 sum = Vec2(1.0, 2.0) + Vec2(3.0, 4.0);
        report("add", same(sum, 4.0, 6.0));
    }

    {
        const Vec2 difference = Vec2(3.0, 4.0) - Vec2(1.0, 2.0);
        report("sub", same(difference, 2.0, 2.0));
    }

    {
        const Vec2 left = Vec2(1.0, 2.0) * 3.0;
        const Vec2 right = 3.0 * Vec2(1.0, 2.0);
        report("scalar", same(left, 3.0, 6.0) && same(right, 3.0, 6.0));
    }

    {
        const Vec2 negated = -Vec2(1.0, -2.0);
        report("negate", same(negated, -1.0, 2.0));
    }

    {
        const bool ok = Vec2(1.0, 2.0) == Vec2(1.0, 2.0) &&
                        !(Vec2(1.0, 2.0) == Vec2(1.0, 3.0));
        report("equality", ok);
    }

    {
        const Vec2 a(3.0, 4.0);
        const Vec2 b(1.0, 2.0);
        report("dot-length", a.dot(b) == 11.0 && a.length_squared() == 25.0);
    }

    {
        Vec2 a(1.0, 2.0);
        Vec2 b(3.0, 4.0);
        Vec2& reference = (a += b);
        report("plus-assign", same(a, 4.0, 6.0) && &reference == &a);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
