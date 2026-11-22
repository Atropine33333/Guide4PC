#include <iostream>
#include <vector>

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    std::vector<std::vector<int>> graph(static_cast<std::size_t>(n));
    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        long long w = 0;
        std::cin >> u >> v >> w;
        graph[static_cast<std::size_t>(u)].push_back(v);
    }

    std::vector<long long> distance(static_cast<std::size_t>(n), -1);
    distance[0] = 0;

    // TODO: relax edges with a priority queue.
    for (int i = 0; i < n; ++i) {
        if (i != 0) std::cout << ' ';
        std::cout << distance[static_cast<std::size_t>(i)];
    }
    std::cout << '\n';
    return 0;
}
