// Reference solution: data-huffman-code
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

int main() {
    std::string text;
    std::getline(std::cin, text);

    if (text.empty()) {
        std::cout << '\n';
        return 0;
    }

    std::map<char, long long> frequency;
    for (char ch : text) ++frequency[ch];

    struct Node {
        long long count = 0;
        char smallest = 0;
        int left = -1;
        int right = -1;
        char character = 0;
    };

    std::vector<Node> nodes;
    using Key = std::tuple<long long, char, int>;
    std::priority_queue<Key, std::vector<Key>, std::greater<Key>> queue;

    for (const auto& entry : frequency) {
        const int index = static_cast<int>(nodes.size());
        Node node;
        node.count = entry.second;
        node.smallest = entry.first;
        node.character = entry.first;
        nodes.push_back(node);
        queue.push({node.count, node.smallest, index});
    }

    while (queue.size() > 1) {
        const Key first = queue.top();
        queue.pop();
        const Key second = queue.top();
        queue.pop();
        const int left = std::get<2>(first);
        const int right = std::get<2>(second);

        Node parent;
        parent.count = nodes[left].count + nodes[right].count;
        parent.smallest = nodes[left].smallest;
        parent.left = left;
        parent.right = right;
        const int index = static_cast<int>(nodes.size());
        nodes.push_back(parent);
        queue.push({parent.count, parent.smallest, index});
    }

    const int root = std::get<2>(queue.top());
    std::map<char, std::string> codes;
    std::vector<std::pair<int, std::string>> stack;
    stack.push_back({root, ""});
    while (!stack.empty()) {
        const auto item = stack.back();
        stack.pop_back();
        const Node& node = nodes[static_cast<std::size_t>(item.first)];
        if (node.left == -1) {
            codes[node.character] = item.second.empty() ? "0" : item.second;
        } else {
            stack.push_back({node.right, item.second + '1'});
            stack.push_back({node.left, item.second + '0'});
        }
    }

    for (char ch : text) std::cout << codes[ch];
    std::cout << '\n';
    for (const auto& entry : codes) {
        std::cout << entry.first << ' ' << entry.second << '\n';
    }
    return 0;
}
