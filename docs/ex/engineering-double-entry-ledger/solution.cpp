// Reference solution: engineering-double-entry-ledger
#include <cstddef>
#include <map>
#include <optional>

class Ledger {
public:
    Ledger() = default;

    bool open(int account) {
        return balances_.emplace(account, 0).second;
    }

    bool deposit(int account, long long amount) {
        if (amount <= 0) return false;
        const auto it = balances_.find(account);
        if (it == balances_.end()) return false;
        it->second += amount;
        return true;
    }

    bool withdraw(int account, long long amount) {
        if (amount <= 0) return false;
        const auto it = balances_.find(account);
        if (it == balances_.end() || it->second < amount) return false;
        it->second -= amount;
        return true;
    }

    bool transfer(int from, int to, long long amount) {
        if (from == to || amount <= 0) return false;
        const auto source = balances_.find(from);
        const auto destination = balances_.find(to);
        if (source == balances_.end() || destination == balances_.end()) {
            return false;
        }
        if (source->second < amount) return false;
        source->second -= amount;
        destination->second += amount;
        return true;
    }

    std::optional<long long> balance(int account) const {
        const auto it = balances_.find(account);
        if (it == balances_.end()) return std::nullopt;
        return it->second;
    }

    std::size_t accounts() const { return balances_.size(); }

    long long total() const {
        long long sum = 0;
        for (const auto& entry : balances_) sum += entry.second;
        return sum;
    }

    bool audit() const {
        long long sum = 0;
        for (const auto& entry : balances_) {
            if (entry.second < 0) return false;
            sum += entry.second;
        }
        return sum == total();
    }

private:
    std::map<int, long long> balances_;
};
