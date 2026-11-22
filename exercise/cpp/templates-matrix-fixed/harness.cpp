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
        Matrix<int, 2, 3> matrix;
        const bool ok = matrix.rows() == 2 && matrix.cols() == 3 &&
                        matrix.sum() == 0 && matrix.at(0, 0) == 0 &&
                        matrix.at(1, 2) == 0;
        report("default", ok);
    }

    {
        Matrix<int, 2, 3> matrix;
        matrix.fill(7);
        const bool ok = matrix.at(0, 0) == 7 && matrix.at(0, 2) == 7 &&
                        matrix.at(1, 0) == 7 && matrix.at(1, 2) == 7 &&
                        matrix.sum() == 42;
        report("fill", ok);
    }

    {
        Matrix<int, 2, 2> matrix;
        int value = 1;
        for (int row = 0; row < 2; ++row) {
            for (int col = 0; col < 2; ++col) matrix.at(row, col) = value++;
        }
        const Matrix<int, 2, 2>& view = matrix;
        const bool ok = view.at(0, 0) == 1 && view.at(0, 1) == 2 &&
                        view.at(1, 0) == 3 && view.at(1, 1) == 4 &&
                        view.sum() == 10;
        report("at", ok);
    }

    {
        Matrix<double, 1, 1> matrix;
        matrix.at(0, 0) = 2.5;
        const bool ok = matrix.sum() == 2.5 && matrix.rows() == 1 &&
                        matrix.cols() == 1;
        report("double", ok);
    }

    {
        Matrix<long long, 3, 3> matrix;
        matrix.fill(1000000000LL);
        const bool ok = matrix.sum() == 9000000000LL;
        report("long-long", ok);
    }

    {
        Matrix<int, 3, 2> matrix;
        const bool ok = matrix.rows() == 3 && matrix.cols() == 2 &&
                        matrix.at(2, 1) == 0;
        matrix.at(2, 1) = 9;
        const bool after = matrix.at(2, 1) == 9 && matrix.sum() == 9;
        report("shape", ok && after);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
