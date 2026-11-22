#include <functional>
#include <string>

class CommandRouter {
public:
    using Handler = std::function<std::string(const std::string&)>;

    void add(const std::string& name, Handler handler) {
        (void)name;
        (void)handler;
        // TODO: 没有保存处理函数。
    }

    bool has(const std::string& name) const {
        (void)name;
        return false;
    }

    std::string dispatch(const std::string& name, const std::string& argument) const {
        (void)name;
        (void)argument;
        // TODO: 总是返回未知命令。
        return "unknown";
    }

    std::size_t size() const {
        return 0;
    }
};
