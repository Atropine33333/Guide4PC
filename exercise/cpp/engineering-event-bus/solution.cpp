// 参考实现：engineering-event-bus
using EventHandler = void (*)(void* context, int payload);

class EventBus {
public:
    EventBus() = default;

    int subscribe(int event, EventHandler handler, void* context) {
        if (handler == nullptr || total_ >= 8) return -1;
        entries_[total_] = Entry{next_id_, event, handler, context};
        ++total_;
        return next_id_++;
    }

    bool unsubscribe(int id) {
        for (int i = 0; i < total_; ++i) {
            if (entries_[i].id != id) continue;
            for (int j = i; j + 1 < total_; ++j) {
                entries_[j] = entries_[j + 1];
            }
            --total_;
            return true;
        }
        return false;
    }

    void publish(int event, int payload) {
        for (int i = 0; i < total_; ++i) {
            if (entries_[i].event == event) {
                entries_[i].handler(entries_[i].context, payload);
            }
        }
    }

    int count(int event) const {
        int result = 0;
        for (int i = 0; i < total_; ++i) {
            if (entries_[i].event == event) ++result;
        }
        return result;
    }

    int total() const {
        return total_;
    }

    void clear() {
        total_ = 0;
        next_id_ = 1;
    }

private:
    struct Entry {
        int id = 0;
        int event = 0;
        EventHandler handler = nullptr;
        void* context = nullptr;
    };

    Entry entries_[8]{};
    int total_ = 0;
    int next_id_ = 1;
};
