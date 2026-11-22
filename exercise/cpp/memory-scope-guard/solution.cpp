// 参考实现：memory-scope-guard
class ScopeGuard {
public:
    explicit ScopeGuard(void (*action)(void*), void* context)
        : action_(action), context_(context), active_(true) {}

    ~ScopeGuard() {
        if (active_ && action_ != nullptr) action_(context_);
    }

    void dismiss() { active_ = false; }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    void (*action_)(void*) = nullptr;
    void* context_ = nullptr;
    bool active_ = false;
};
