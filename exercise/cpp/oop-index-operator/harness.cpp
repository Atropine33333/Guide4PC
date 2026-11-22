// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <cstddef>
#include <iostream>
#include <type_traits>

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
        Matrix matrix(3, 4);
        report("shape", matrix.rows() == 3 && matrix.cols() == 4);
    }

    {
        Matrix matrix(2, 3);
        bool ok = true;
        for (int r = 0; r < 2; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (matrix[r][c] != 0) ok = false;
            }
        }
        report("default-zero", ok);
    }

    {
        Matrix matrix(3, 4);
        matrix[1][2] = 7;
        const bool ok = matrix[1][2] == 7 && matrix[0][2] == 0 && matrix[1][0] == 0;
        report("write-read", ok);
    }

    {
        Matrix matrix(3, 3);
        int& cell = matrix[2][0];
        cell = 5;
        const bool ok = matrix[2][0] == 5;
        report("reference", ok);
    }

    {
        Matrix matrix(2, 2);
        auto row = matrix[0];
        row[1] = 9;
        auto copy = row;
        copy[0] = 4;
        const bool ok = matrix[0][1] == 9 && matrix[0][0] == 4;
        report("row-copy", ok);
    }

    {
        Matrix matrix(2, 2);
        matrix[1][1] = 6;
        const Matrix& constant = matrix;
        static_assert(std::is_same_v<decltype(constant[1][1]), const int&>);
        const bool ok = constant[1][1] == 6 && constant[0][0] == 0;
        report("const-access", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
