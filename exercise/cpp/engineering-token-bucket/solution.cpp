// 参考实现：engineering-token-bucket
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
    if (elapsed <= 0 || refill_ == 0) return;
    tokens_ += 1LL * refill_ * elapsed;
    if (tokens_ > capacity_) tokens_ = capacity_;
}

bool TokenBucket::take(int elapsed) {
    advance(elapsed);
    if (tokens_ <= 0) return false;
    --tokens_;
    return true;
}

int TokenBucket::tokens() const {
    return static_cast<int>(tokens_);
}

int TokenBucket::capacity() const {
    return capacity_;
}
