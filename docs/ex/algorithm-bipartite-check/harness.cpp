// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

bool valid_coloring(const std::vector<std::vector<int>>& graph,
                    const std::vector<int>& colors) {
    if (colors.size() != graph.size()) {
        return false;
    }
    for (int color : colors) {
        if (color != 0 && color != 1) {
            return false;
        }
    }
    for (std::size_t u = 0; u < graph.size(); ++u) {
        for (int v : graph[u]) {
            if (v < 0 || static_cast<std::size_t>(v) >= graph.size()) {
                return false;
            }
            if (colors[u] == colors[static_cast<std::size_t>(v)]) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace

int main() {
    {
        const std::vector<std::vector<int>> empty;
        const std::vector<std::vector<int>> single(1);
        report("empty-and-single", is_bipartite(empty) &&
                                       is_bipartite(single) &&
                                       two_coloring(empty).empty() &&
                                       valid_coloring(single, two_coloring(single)));
    }

    {
        const std::vector<std::vector<int>> square = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};
        const std::vector<int> colors = two_coloring(square);
        report("even-cycle", is_bipartite(square) && valid_coloring(square, colors) &&
                                 colors.size() == 4);
    }

    {
        const std::vector<std::vector<int>> triangle = {{1, 2}, {0, 2}, {0, 1}};
        report("odd-cycle", !is_bipartite(triangle) && two_coloring(triangle).empty());
    }

    {
        const std::vector<std::vector<int>> path = {{1}, {0, 2}, {1, 3}, {2}};
        const std::vector<int> colors = two_coloring(path);
        report("path", is_bipartite(path) && valid_coloring(path, colors));
    }

    {
        const std::vector<std::vector<int>> loop = {{0}};
        const std::vector<std::vector<int>> parallel = {{1, 1}, {0, 0}};
        report("self-loop-and-parallel",
               !is_bipartite(loop) && two_coloring(loop).empty() &&
                   is_bipartite(parallel) && valid_coloring(parallel, two_coloring(parallel)));
    }

    {
        std::vector<std::vector<int>> tree(1000);
        for (int i = 0; i + 1 < 1000; ++i) {
            tree[static_cast<std::size_t>(i)].push_back(i + 1);
            tree[static_cast<std::size_t>(i + 1)].push_back(i);
        }
        const bool tree_ok = is_bipartite(tree);
        const std::vector<int> colors = two_coloring(tree);
        std::vector<std::vector<int>> graph = tree;
        graph.resize(1001);
        graph[1000] = {1000};
        const bool loop_ok = !is_bipartite(graph) && two_coloring(graph).empty();
        report("large-disconnected",
               tree_ok && valid_coloring(tree, colors) && loop_ok);
    }

    {
        const std::vector<std::vector<int>> star = {{1, 2, 3}, {0}, {0}, {0}};
        const std::vector<int> colors = two_coloring(star);
        bool leaves_same = colors.size() == 4 && colors[1] == colors[2] &&
                           colors[2] == colors[3] && colors[0] != colors[1];
        report("star", is_bipartite(star) && valid_coloring(star, colors) && leaves_same);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
