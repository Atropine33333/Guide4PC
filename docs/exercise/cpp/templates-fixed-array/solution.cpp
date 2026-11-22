// 参考实现：templates-fixed-array
template <typename T, int N>
class FixedArray {
public:
    FixedArray() = default;

    int size() const {
        return N;
    }

    T& operator[](int index) {
        return items_[index];
    }

    const T& operator[](int index) const {
        return items_[index];
    }

    void fill(const T& value) {
        for (int i = 0; i < N; ++i) items_[i] = value;
    }

    bool try_get(int index, T& out) const {
        if (index < 0 || index >= N) return false;
        out = items_[index];
        return true;
    }

    T* data() {
        return items_;
    }

    const T* data() const {
        return items_;
    }

private:
    T items_[N]{};
};
