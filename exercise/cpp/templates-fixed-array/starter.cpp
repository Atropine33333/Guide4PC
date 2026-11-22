template <typename T, int N>
class FixedArray {
public:
    FixedArray() {
        // TODO: 这里应当保证元素被值初始化。
    }

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
        // TODO: 漏掉了最后一个元素。
        for (int i = 0; i < N - 1; ++i) items_[i] = value;
    }

    bool try_get(int index, T& out) const {
        // TODO: 越界时应当返回 false。
        if (index < 0 || index >= N) return true;
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
