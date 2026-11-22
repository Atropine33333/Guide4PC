class TokenBucket {
public:
    TokenBucket(int capacity, int refill);
    bool take(int elapsed);
    void advance(int elapsed);
    int tokens() const;
    int capacity() const;

private:
    long long tokens_;
    int capacity_;
    int refill_;
};

TokenBucket::TokenBucket(int capacity, int refill)
    : tokens_(capacity), capacity_(capacity), refill_(refill) {}

void TokenBucket::advance(int elapsed) {
    (void)elapsed;
    (void)refill_;
    // TODO: 补充 refill_ * elapsed，封顶到 capacity_。
}

bool TokenBucket::take(int elapsed) {
    (void)elapsed;
    // TODO: 先 advance，再尝试消耗一个令牌。
    return false;
}

int TokenBucket::tokens() const {
    return static_cast<int>(tokens_);
}

int TokenBucket::capacity() const {
    return capacity_;
}
