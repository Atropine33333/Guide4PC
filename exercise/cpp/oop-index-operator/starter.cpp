#include <cstddef>
#include <vector>

class Matrix {
public:
    Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
        // TODO: 没有分配数据，元素也不是 0。
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    class Row {
    public:
        Row(int* data, int cols) : data_(data), cols_(cols) {}

        int& operator[](int col) const {
            // TODO: 应该返回 data_[col] 的引用。
            return data_[col];
        }

    private:
        int* data_ = nullptr;
        int cols_ = 0;
    };

    class ConstRow {
    public:
        ConstRow(const int* data, int cols) : data_(data), cols_(cols) {}

        const int& operator[](int col) const {
            // TODO: 应该返回 data_[col] 的常量引用。
            return data_[col];
        }

    private:
        const int* data_ = nullptr;
        int cols_ = 0;
    };

    Row operator[](int row) {
        // TODO: 还没有按行切分数据。
        return Row(data_.data(), cols_);
    }

    ConstRow operator[](int row) const {
        // TODO: 还没有按行切分数据。
        return ConstRow(data_.data(), cols_);
    }

private:
    std::vector<int> data_;
    int rows_ = 0;
    int cols_ = 0;
};
