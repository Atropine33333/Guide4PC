// Guide4PC harness appended to the student's code in the same translation unit.
#include <iostream>
#include <string>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    {
        ScoreBoard board;
        board.add("alice", 10);
        board.add("alice", 5);
        int total = 0;
        const bool ok = board.score("alice", total) && total == 15;
        report("accumulates", ok);
    }

    {
        ScoreBoard board;
        board.add("alice", 10);
        board.add("bob", 20);
        board.add("alice", 1);
        int total = 0;
        const bool ok = board.players() == 2 && board.score("alice", total) && total == 11 &&
                        !board.score("carol", total);
        report("players-count", ok);
    }

    {
        ScoreBoard board;
        board.add("alice", 10);
        board.add("bob", 20);
        std::string leader;
        const bool ok = board.leader(leader) && leader == "bob";
        report("leader-highest", ok);
    }

    {
        ScoreBoard board;
        board.add("carol", 5);
        board.add("alice", 5);
        board.add("bob", 5);
        std::string leader;
        const bool ok = board.leader(leader) && leader == "alice";
        report("leader-tie", ok);
    }

    {
        ScoreBoard board;
        std::string leader = "unchanged";
        const bool ok = !board.leader(leader) && leader == "unchanged" && board.players() == 0;
        report("empty-leader", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
