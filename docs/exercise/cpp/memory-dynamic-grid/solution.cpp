#include <vector>

class Grid {
public:
    Grid(int rows, int cols)
        : rows_(rows),
          cols_(cols),
          data_(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols),
                0) {}

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    int& at(int row, int col) {
        return data_[static_cast<std::size_t>(row) *
                         static_cast<std::size_t>(cols_) +
                     static_cast<std::size_t>(col)];
    }

    const int& at(int row, int col) const {
        return data_[static_cast<std::size_t>(row) *
                         static_cast<std::size_t>(cols_) +
                     static_cast<std::size_t>(col)];
    }

    void fill(int value) {
        for (int& element : data_) {
            element = value;
        }
    }

    std::vector<int> row_major() const { return data_; }

private:
    int rows_;
    int cols_;
    std::vector<int> data_;
};
