#include <vector>

namespace {

bool color_graph(const std::vector<std::vector<int>>& graph, std::vector<int>& colors) {
    const int n = static_cast<int>(graph.size());
    colors.assign(static_cast<std::size_t>(n), -1);
    std::vector<int> stack;

    for (int start = 0; start < n; ++start) {
        if (colors[static_cast<std::size_t>(start)] != -1) {
            continue;
        }
        colors[static_cast<std::size_t>(start)] = 0;
        stack.push_back(start);
        while (!stack.empty()) {
            const int vertex = stack.back();
            stack.pop_back();
            for (int neighbour : graph[static_cast<std::size_t>(vertex)]) {
                if (neighbour < 0 || neighbour >= n) {
                    continue;
                }
                if (colors[static_cast<std::size_t>(neighbour)] == -1) {
                    colors[static_cast<std::size_t>(neighbour)] =
                        1 - colors[static_cast<std::size_t>(vertex)];
                    stack.push_back(neighbour);
                } else if (colors[static_cast<std::size_t>(neighbour)] ==
                           colors[static_cast<std::size_t>(vertex)]) {
                    return false;
                }
            }
        }
    }
    return true;
}

}  // namespace

bool is_bipartite(const std::vector<std::vector<int>>& graph) {
    std::vector<int> colors;
    return color_graph(graph, colors);
}

std::vector<int> two_coloring(const std::vector<std::vector<int>>& graph) {
    std::vector<int> colors;
    if (!color_graph(graph, colors)) {
        return {};
    }
    return colors;
}
