using EventHandler = void (*)(void* context, int payload);

class EventBus {
public:
    EventBus() = default;

    int subscribe(int event, EventHandler handler, void* context) {
        if (handler == nullptr || total_ >= 8) return -1;
        entries_[total_] = Entry{next_id_++, event, handler, context};
        ++total_;
        return entries_[total_ - 1].id;
    }

    bool unsubscribe(int id) {
        // TODO: 按编号删除，并保持其余订阅的顺序。
        (void)id;
        return false;
    }

    void publish(int event, int payload) {
        for (int i = 0; i < total_; ++i) {
            // TODO: 只调用订阅了 event 的 handler。
            (void)event;
            entries_[i].handler(entries_[i].context, payload);
        }
    }

    int count(int event) const {
        // TODO: 只统计该事件的订阅。
        (void)event;
        return total_;
    }

    int total() const {
        return total_;
    }

    void clear() {
        total_ = 0;
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
