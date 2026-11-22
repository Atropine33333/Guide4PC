// 参考实现：ticket-machine
#include <cstddef>

class TicketMachine {
public:
    int next() { return ++issued_; }

    int issued() const { return issued_; }

    void reset() { issued_ = 0; }

private:
    int issued_ = 0;
};
