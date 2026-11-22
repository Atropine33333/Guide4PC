#include <vector>

class Grid {
public:
    Grid(int rows, int cols) : rows_(rows), cols_(cols), data_(1, 0) {
        (void)rows;
        (void)cols;
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    int& at(int row, int col) {
        // TODO: 按行优先下标访问。
        (void)row;
        (void)col;
        return data_[0];
    }

    const int& at(int row, int col) const {
        // TODO: 按行优先下标访问。
        (void)row;
        (void)col;
        return data_[0];
    }

    void fill(int value) {
        // TODO: 填充所有元素。
        (void)value;
    }

    std::vector<int> row_major() const {
        // TODO: 返回元素副本。
        return {};
    }

private:
    int rows_;
    int cols_;
    std::vector<int> data_;
};
