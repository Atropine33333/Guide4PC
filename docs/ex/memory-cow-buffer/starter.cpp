#include <cstddef>
#include <memory>
#include <string>

class CowBuffer {
public:
    CowBuffer() : data_(std::make_shared<std::string>()) {}
    explicit CowBuffer(const std::string& data)
        : data_(std::make_shared<std::string>(data)) {}

    CowBuffer(const CowBuffer& other) : data_(other.data_) {}
    CowBuffer& operator=(const CowBuffer& other) {
        if (this != &other) data_ = other.data_;
        return *this;
    }

    const std::string& str() const { return *data_; }

    void append(const std::string& suffix) {
        // TODO: detach when the storage is shared.
        data_->append(suffix);
    }

    std::size_t shares() const {
        return static_cast<std::size_t>(data_.use_count());
    }

    static int live_blocks() {
        // TODO: count distinct blocks, not constructed objects.
        return 1;
    }

private:
    std::shared_ptr<std::string> data_;
};
