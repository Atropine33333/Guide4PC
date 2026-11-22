#include <string>

class Trie {
public:
    Trie() = default;
    ~Trie() = default;

    void insert(const std::string& word) {
        (void)word;
        // TODO: 还没有真正保存单词。
    }

    int count_prefix(const std::string& prefix) const {
        (void)prefix;
        // TODO: 还没有实现前缀计数。
        return 0;
    }

    int size() const {
        // TODO: 还没有统计插入次数。
        return 0;
    }

    bool contains(const std::string& word) const {
        (void)word;
        // TODO: 还没有实现查找。
        return false;
    }
};
