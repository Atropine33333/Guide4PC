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
        // TODO: check the balance before subtracting.
        if (amount <= 0) return false;
        const auto it = balances_.find(account);
        if (it == balances_.end()) return false;
        it->second -= amount;
        return true;
    }

    bool transfer(int from, int to, long long amount) {
        // TODO: make the two updates atomic and reject from == to.
        if (!withdraw(from, amount)) return false;
        return deposit(to, amount);
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
        // TODO: verify balances and the total invariant.
        return true;
    }

private:
    std::map<int, long long> balances_;
};
