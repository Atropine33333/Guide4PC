// Reference solution: memory-cow-buffer
#include <cstddef>
#include <memory>
#include <string>

class CowBuffer {
public:
    CowBuffer() : data_(make_block("")) {}

    explicit CowBuffer(const std::string& data) : data_(make_block(data)) {}

    CowBuffer(const CowBuffer& other) = default;
    CowBuffer& operator=(const CowBuffer& other) = default;
    ~CowBuffer() = default;

    const std::string& str() const { return *data_; }

    void append(const std::string& suffix) {
        if (data_.use_count() > 1) {
            data_ = make_block(*data_);
        }
        data_->append(suffix);
    }

    std::size_t shares() const {
        return static_cast<std::size_t>(data_.use_count());
    }

    static int live_blocks() { return block_count(); }

private:
    static int& block_count() {
        static int count = 0;
        return count;
    }

    static std::shared_ptr<std::string> make_block(const std::string& data) {
        std::shared_ptr<std::string> block(
            new std::string(data), [](std::string* value) {
                delete value;
                --block_count();
            });
        ++block_count();
        return block;
    }

    std::shared_ptr<std::string> data_;
};
