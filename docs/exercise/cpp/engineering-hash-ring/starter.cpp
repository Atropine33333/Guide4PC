#include <cstddef>
#include <set>
#include <string>
#include <vector>

class HashRing {
public:
    explicit HashRing(int virtual_nodes = 50) : virtual_nodes_(virtual_nodes) {}

    void add_node(const std::string& name) {
        // TODO: 没有生成虚拟节点，也没有放入哈希环。
        nodes_.insert(name);
    }

    void remove_node(const std::string& name) { nodes_.erase(name); }

    std::string locate(const std::string& key) const {
        // TODO: 直接返回第一个节点，没有按环查找。
        (void)key;
        if (nodes_.empty()) return "";
        return *nodes_.begin();
    }

    std::size_t size() const { return nodes_.size(); }

private:
    int virtual_nodes_ = 50;
    std::set<std::string> nodes_;
};
