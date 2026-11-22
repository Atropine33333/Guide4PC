// 参考实现：debug-scoreboard
#include <map>
#include <string>

class ScoreBoard {
public:
    void add(const std::string& player, int points) {
        scores_[player] += points;
    }

    bool score(const std::string& player, int& out) const {
        const auto it = scores_.find(player);
        if (it == scores_.end()) return false;
        out = it->second;
        return true;
    }

    int players() const { return static_cast<int>(scores_.size()); }

    bool leader(std::string& out) const {
        if (scores_.empty()) return false;
        int best = -1;
        for (const auto& entry : scores_) {
            if (entry.second > best) {
                best = entry.second;
                out = entry.first;
            }
        }
        return true;
    }

private:
    std::map<std::string, int> scores_;
};
