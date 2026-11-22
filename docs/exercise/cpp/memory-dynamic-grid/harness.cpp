// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
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
        Grid grid(2, 3);
        const std::vector<int> flat = grid.row_major();
        report("dimensions", grid.rows() == 2 && grid.cols() == 3 &&
                                 flat.size() == 6 &&
                                 flat == std::vector<int>(6, 0));
    }

    {
        Grid grid(2, 3);
        grid.at(0, 0) = 1;
        grid.at(1, 2) = 6;
        const std::vector<int> flat = grid.row_major();
        report("write-elements", grid.at(0, 0) == 1 && grid.at(1, 2) == 6 &&
                                      grid.at(0, 2) == 0 &&
                                      flat == std::vector<int>{1, 0, 0, 0, 0, 6});
    }

    {
        Grid grid(3, 2);
        grid.fill(7);
        bool ok = grid.rows() == 3 && grid.cols() == 2;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 2; ++col) {
                ok = ok && grid.at(row, col) == 7;
            }
        }
        report("fill", ok);
    }

    {
        Grid grid(2, 2);
        grid.at(0, 0) = 5;
        Grid copy = grid;
        copy.at(0, 0) = 9;
        copy.at(1, 1) = 8;
        report("deep-copy", grid.at(0, 0) == 5 && grid.at(1, 1) == 0 &&
                                copy.at(0, 0) == 9 && copy.at(1, 1) == 8);
    }

    {
        Grid source(3, 2);
        int value = 1;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 2; ++col) {
                source.at(row, col) = value++;
            }
        }
        const std::vector<int> flat = source.row_major();
        report("row-major-order", flat == std::vector<int>{1, 2, 3, 4, 5, 6});
    }

    {
        Grid grid(2, 2);
        grid.fill(4);
        const Grid& view = grid;
        report("const-access", view.at(0, 0) == 4 && view.at(1, 1) == 4 &&
                                   view.rows() == 2 && view.cols() == 2);
    }

    {
        Grid grid(100, 100);
        grid.fill(5);
        grid.at(0, 0) = 1;
        grid.at(99, 99) = 2;
        bool ok = grid.row_major().size() == 10000;
        for (int row = 0; row < 100; ++row) {
            for (int col = 0; col < 100; ++col) {
                const int expected = (row == 0 && col == 0)
                                         ? 1
                                         : (row == 99 && col == 99) ? 2 : 5;
                ok = ok && grid.at(row, col) == expected;
            }
        }
        report("large-grid", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
