// 参考实现：memory-destruction-order
#include <vector>

class ScopeTracer {
public:
    explicit ScopeTracer(int id) : id_(id) {}

    ~ScopeTracer() {
        log_.push_back(id_);
    }

    ScopeTracer(const ScopeTracer&) = delete;
    ScopeTracer& operator=(const ScopeTracer&) = delete;

    int id() const {
        return id_;
    }

    static void clear_log() {
        log_.clear();
    }

    static std::vector<int> destroyed_log() {
        return log_;
    }

private:
    int id_ = 0;
    static std::vector<int> log_;
};

std::vector<int> ScopeTracer::log_;
