#include <cstddef>

class TicketMachine {
public:
    int next() {
        // 这份计数目前放在函数里，所有机器会共用它。
        static int counter = 0;
        return ++counter;
    }

    int issued() const {
        // TODO: 返回这台机器已经发出的号码数。
        return 0;
    }

    void reset() {
        // TODO: 清零这台机器的计数。
    }

private:
    // 想一想：计数应当作为哪个类的成员？
};
