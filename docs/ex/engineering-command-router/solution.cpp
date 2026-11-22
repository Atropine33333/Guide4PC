// 参考实现：engineering-command-router
#include <functional>
#include <map>
#include <string>
#include <utility>

class CommandRouter {
public:
    using Handler = std::function<std::string(const std::string&)>;

    void add(const std::string& name, Handler handler) {
        handlers_[name] = std::move(handler);
    }

    bool has(const std::string& name) const {
        return handlers_.find(name) != handlers_.end();
    }

    std::string dispatch(const std::string& name, const std::string& argument) const {
        const auto it = handlers_.find(name);
        if (it == handlers_.end()) return "unknown";
        return it->second(argument);
    }

    std::size_t size() const {
        return handlers_.size();
    }

private:
    std::map<std::string, Handler> handlers_;
};
