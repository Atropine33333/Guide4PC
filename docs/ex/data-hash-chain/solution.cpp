// 参考实现：data-hash-chain
#include <cstddef>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>

namespace {

using Entry = std::pair<std::string, long long>;

unsigned hash_key(const std::string& key, int buckets) {
    unsigned sum = 0;
    for (char ch : key) sum += static_cast<unsigned char>(ch);
    return sum % static_cast<unsigned>(buckets);
}

}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int buckets = 0;
    if (!(std::cin >> buckets) || buckets <= 0) return 0;

    std::vector<std::list<Entry>> table(static_cast<std::size_t>(buckets));

    std::string command;
    while (std::cin >> command) {
        if (command == "put") {
            std::string key;
            long long value = 0;
            std::cin >> key >> value;
            std::list<Entry>& chain = table[hash_key(key, buckets)];
            bool found = false;
            for (Entry& entry : chain) {
                if (entry.first == key) {
                    entry.second = value;
                    found = true;
                    break;
                }
            }
            if (!found) chain.push_front({key, value});
        } else if (command == "erase") {
            std::string key;
            std::cin >> key;
            std::list<Entry>& chain = table[hash_key(key, buckets)];
            for (auto it = chain.begin(); it != chain.end(); ++it) {
                if (it->first == key) {
                    chain.erase(it);
                    break;
                }
            }
        } else if (command == "get") {
            std::string key;
            std::cin >> key;
            const std::list<Entry>& chain = table[hash_key(key, buckets)];
            bool found = false;
            for (const Entry& entry : chain) {
                if (entry.first == key) {
                    std::cout << entry.second << '\n';
                    found = true;
                    break;
                }
            }
            if (!found) std::cout << "none\n";
        } else if (command == "dump") {
            for (int i = 0; i < buckets; ++i) {
                std::cout << i << ':';
                for (const Entry& entry : table[static_cast<std::size_t>(i)]) {
                    std::cout << ' ' << entry.first;
                }
                std::cout << '\n';
            }
        }
    }
    return 0;
}
