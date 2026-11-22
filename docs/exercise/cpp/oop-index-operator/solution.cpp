// 参考实现：oop-index-operator
#include <cstddef>
#include <vector>

class Matrix {
public:
    Matrix(int rows, int cols)
        : data_(static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols), 0),
          rows_(rows),
          cols_(cols) {}

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    class Row {
    public:
        Row(int* data, int cols) : data_(data), cols_(cols) {}

        int& operator[](int col) const { return data_[col]; }

    private:
        int* data_ = nullptr;
        int cols_ = 0;
    };

    class ConstRow {
    public:
        ConstRow(const int* data, int cols) : data_(data), cols_(cols) {}

        const int& operator[](int col) const { return data_[col]; }

    private:
        const int* data_ = nullptr;
        int cols_ = 0;
    };

    Row operator[](int row) {
        return Row(data_.data() + static_cast<std::size_t>(row) * static_cast<std::size_t>(cols_),
                   cols_);
    }

    ConstRow operator[](int row) const {
        return ConstRow(
            data_.data() + static_cast<std::size_t>(row) * static_cast<std::size_t>(cols_), cols_);
    }

private:
    std::vector<int> data_;
    int rows_ = 0;
    int cols_ = 0;
};
