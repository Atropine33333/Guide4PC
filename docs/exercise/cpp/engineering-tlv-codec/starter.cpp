#include <cstddef>
#include <string>
#include <utility>
#include <vector>

struct DecodeResult {
    bool ok;
    std::vector<std::pair<unsigned char, std::string>> records;
    std::size_t bad_offset;
};

std::vector<unsigned char> encode_records(
    const std::vector<std::pair<unsigned char, std::string>>& records) {
    // TODO: 依次写入 tag、长度、payload。
    (void)records;
    return {};
}

DecodeResult decode_records(const std::vector<unsigned char>& data) {
    // TODO: 解析记录，处理截断并记录 bad_offset。
    (void)data;
    return DecodeResult{false, {}, 0};
}
