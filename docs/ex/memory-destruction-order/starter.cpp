#include <vector>

class ScopeTracer {
public:
    explicit ScopeTracer(int id) : id_(id) {}

    ~ScopeTracer() {
        // TODO: 把 id_ 追加到静态日志。
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
