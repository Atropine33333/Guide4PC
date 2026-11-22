// 参考实现：data-topo-order
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    int n = 0, m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<int>> graph(static_cast<std::size_t>(n + 1));
    std::vector<int> indegree(static_cast<std::size_t>(n + 1), 0);
    for (int i = 0; i < m; ++i) {
        int u = 0, v = 0;
        std::cin >> u >> v;
        graph[static_cast<std::size_t>(u)].push_back(v);
        ++indegree[static_cast<std::size_t>(v)];
    }

    std::priority_queue<int, std::vector<int>, std::greater<int>> ready;
    for (int v = 1; v <= n; ++v) {
        if (indegree[static_cast<std::size_t>(v)] == 0) ready.push(v);
    }

    std::vector<int> order;
    order.reserve(static_cast<std::size_t>(n));
    while (!ready.empty()) {
        const int u = ready.top();
        ready.pop();
        order.push_back(u);
        for (int v : graph[static_cast<std::size_t>(u)]) {
            if (--indegree[static_cast<std::size_t>(v)] == 0) ready.push(v);
        }
    }

    if (static_cast<int>(order.size()) != n) {
        std::cout << "cycle\n";
        return 0;
    }
    for (int v : order) std::cout << v << '\n';
    return 0;
}
