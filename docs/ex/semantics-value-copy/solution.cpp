// 参考实现：semantics-value-copy
#include <vector>

struct Data {
    int id;
    std::vector<int> values;
};

int total(const Data& data) {
    int sum = 0;
    for (int value : data.values) sum += value;
    return sum;
}

void append_copy(Data data, int value) {
    data.values.push_back(value);
}

void append_ref(Data& data, int value) {
    data.values.push_back(value);
}

Data make_data(int id, int count) {
    Data result;
    result.id = id;
    for (int i = 0; i < count; ++i) result.values.push_back(i);
    return result;
}

const int& first_value(const Data& data) {
    return data.values[0];
}
