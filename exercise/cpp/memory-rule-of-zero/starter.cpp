#include <string>
#include <vector>

class Notebook {
public:
    void add(const std::string& line) {
        // TODO: 追加一行。
        (void)line;
    }

    std::size_t size() const {
        return 0;  // TODO
    }

    std::string at(std::size_t index) const {
        // TODO: 越界返回 "?"。
        (void)index;
        return {};
    }

    std::string join(const std::string& separator) const {
        // TODO: 用 separator 连接所有行。
        (void)separator;
        return {};
    }
};
