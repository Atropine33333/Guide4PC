#include <vector>

struct Data {
    int id;
    std::vector<int> values;
};

int total(const Data& data) {
    (void)data;
    // TODO: 累加所有元素。
    return 0;
}

void append_copy(Data data, int value) {
    (void)data;
    (void)value;
    // TODO: 这里拿到的是副本，追加后调用方不应看到变化。
}

void append_ref(Data& data, int value) {
    (void)data;
    (void)value;
    // TODO: 在 data.values 末尾追加 value。
}

Data make_data(int id, int count) {
    (void)id;
    (void)count;
    // TODO: 构造 0 到 count-1 的向量。
    return Data{0, {}};
}

const int& first_value(const Data& data) {
    static const int zero = 0;
    (void)data;
    // TODO: 返回 data.values[0] 的引用，而不是这个占位对象。
    return zero;
}
