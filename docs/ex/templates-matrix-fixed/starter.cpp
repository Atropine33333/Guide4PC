template <typename T, int Rows, int Cols>
class Matrix {
public:
    Matrix() = default;

    T& at(int row, int col) {
        (void)row;
        (void)col;
        // TODO: 总是访问第一个元素。
        return data_[0];
    }

    const T& at(int row, int col) const {
        (void)row;
        (void)col;
        return data_[0];
    }

    int rows() const {
        // TODO: 还没有返回行数。
        return 0;
    }

    int cols() const {
        // TODO: 还没有返回列数。
        return 0;
    }

    T sum() const {
        // TODO: 还没有求和。
        return T{};
    }

    void fill(const T& value) {
        (void)value;
        // TODO: 还没有填充。
    }

private:
    T data_[Rows * Cols]{};
};
