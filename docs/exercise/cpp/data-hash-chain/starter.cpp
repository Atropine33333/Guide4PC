#include <iostream>
#include <list>
#include <string>
#include <vector>

int main() {
    int buckets = 0;
    std::cin >> buckets;

    // TODO: 还没有实现链表顺序；目前只保存键值，dump 时什么也不输出。
    std::vector<std::list<std::pair<std::string, long long>>> table(
        static_cast<std::size_t>(buckets));

    std::string command;
    while (std::cin >> command) {
        if (command == "put") {
            std::string key;
            long long value = 0;
            std::cin >> key >> value;
            unsigned sum = 0;
            for (char ch : key) sum += static_cast<unsigned char>(ch);
            table[sum % static_cast<unsigned>(buckets)].push_front({key, value});
        } else if (command == "get") {
            std::string key;
            std::cin >> key;
            std::cout << "none\n";
        } else if (command == "erase") {
            std::string key;
            std::cin >> key;
        } else if (command == "dump") {
            for (int i = 0; i < buckets; ++i) std::cout << i << ":\n";
        }
    }
    return 0;
}
