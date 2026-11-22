// Reference solution: algorithm-dijkstra
#include <cstddef>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

int main() {
    int n = 0;
    int m = 0;
    if (!(std::cin >> n >> m)) return 0;

    using Edge = std::pair<int, long long>;
    std::vector<std::vector<Edge>> graph(static_cast<std::size_t>(n));
    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        long long w = 0;
        std::cin >> u >> v >> w;
        graph[static_cast<std::size_t>(u)].push_back({v, w});
    }

    const long long kInfinity = std::numeric_limits<long long>::max();
    std::vector<long long> distance(static_cast<std::size_t>(n), kInfinity);
    distance[0] = 0;

    using State = std::pair<long long, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> queue;
    queue.push({0, 0});

    while (!queue.empty()) {
        const State current = queue.top();
        queue.pop();
        const long long dist = current.first;
        const int node = current.second;
        if (dist != distance[static_cast<std::size_t>(node)]) continue;

        for (const Edge& edge : graph[static_cast<std::size_t>(node)]) {
            const long long next = dist + edge.second;
            const std::size_t target = static_cast<std::size_t>(edge.first);
            if (next < distance[target]) {
                distance[target] = next;
                queue.push({next, edge.first});
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (i != 0) std::cout << ' ';
        const long long value = distance[static_cast<std::size_t>(i)];
        std::cout << (value == kInfinity ? -1 : value);
    }
    std::cout << '\n';
    return 0;
}
