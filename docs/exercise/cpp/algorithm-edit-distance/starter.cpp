#include <cstddef>
#include <string>
#include <vector>

int edit_distance(const std::string& a, const std::string& b) {
    // TODO: 这里只计算了长度差，插入、删除和替换都还没有实现。
    return static_cast<int>(a.size() > b.size() ? a.size() - b.size() : b.size() - a.size());
}

int longest_common_subsequence(const std::string& a, const std::string& b) {
    // TODO: 还没有实现，先返回 0 占位。
    (void)a;
    (void)b;
    return 0;
}
