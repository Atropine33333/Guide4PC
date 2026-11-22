class ScopeGuard {
public:
    explicit ScopeGuard(void (*action)(void*), void* context) : action_(action), context_(context) {
        // TODO: 还没有记录是否需要执行。
    }

    ~ScopeGuard() {
        // TODO: 析构时应该执行 action_(context_)。
    }

    void dismiss() {
        // TODO: 取消执行。
    }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

private:
    void (*action_)(void*) = nullptr;
    void* context_ = nullptr;
};
