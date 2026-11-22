#include <map>
#include <string>

class ScoreBoard {
public:
    void add(const std::string& player, int points) {
        // TODO: 这段实现有 bug：同一个玩家多次加分时结果不对。
        scores_[player] = points;
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
            // TODO: 这段实现有 bug：并列时的选择不符合约定。
            if (entry.second >= best) {
                best = entry.second;
                out = entry.first;
            }
        }
        return true;
    }

private:
    std::map<std::string, int> scores_;
};
