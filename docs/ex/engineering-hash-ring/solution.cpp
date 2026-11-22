// 参考实现：engineering-hash-ring
#include <cstddef>
#include <cstdint>
#include <map>
#include <set>
#include <string>

class HashRing {
public:
    explicit HashRing(int virtual_nodes = 50)
        : virtual_nodes_(virtual_nodes < 1 ? 1 : virtual_nodes) {}

    void add_node(const std::string& name) {
        if (!nodes_.insert(name).second) return;
        for (int i = 0; i < virtual_nodes_; ++i) {
            ring_.emplace(position(replica(name, i)), name);
        }
    }

    void remove_node(const std::string& name) {
        if (nodes_.erase(name) == 0) return;
        for (int i = 0; i < virtual_nodes_; ++i) {
            const int pos = position(replica(name, i));
            auto range = ring_.equal_range(pos);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == name) {
                    ring_.erase(it);
                    break;
                }
            }
        }
    }

    std::string locate(const std::string& key) const {
        if (ring_.empty()) return "";
        auto it = ring_.lower_bound(position(key));
        if (it == ring_.end()) it = ring_.begin();
        return it->second;
    }

    std::size_t size() const { return nodes_.size(); }

private:
    static std::uint32_t fnv1a(const std::string& text) {
        std::uint32_t hash = 2166136261u;
        for (unsigned char byte : text) {
            hash ^= static_cast<std::uint32_t>(byte);
            hash *= 16777619u;
        }
        return hash;
    }

    static std::uint32_t mix(std::uint32_t hash) {
        hash ^= hash >> 16;
        hash *= 0x7feb352du;
        hash ^= hash >> 15;
        hash *= 0x846ca68bu;
        hash ^= hash >> 16;
        return hash;
    }

    static int position(const std::string& text) {
        return static_cast<int>(mix(fnv1a(text)) % 360u);
    }

    static std::string replica(const std::string& name, int index) {
        return name + "#" + std::to_string(index);
    }

    int virtual_nodes_ = 50;
    std::set<std::string> nodes_;
    std::multimap<int, std::string> ring_;
};
