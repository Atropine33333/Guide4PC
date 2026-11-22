// 参考实现：templates-matrix-fixed
template <typename T, int Rows, int Cols>
class Matrix {
public:
    Matrix() = default;

    T& at(int row, int col) {
        return data_[row * Cols + col];
    }

    const T& at(int row, int col) const {
        return data_[row * Cols + col];
    }

    int rows() const {
        return Rows;
    }

    int cols() const {
        return Cols;
    }

    T sum() const {
        T total{};
        for (int i = 0; i < Rows * Cols; ++i) total += data_[i];
        return total;
    }

    void fill(const T& value) {
        for (int i = 0; i < Rows * Cols; ++i) data_[i] = value;
    }

private:
    T data_[Rows * Cols]{};
};
