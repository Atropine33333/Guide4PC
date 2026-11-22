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
    std::vector<unsigned char> data;
    for (const auto& record : records) {
        data.push_back(record.first);
        data.push_back(static_cast<unsigned char>(record.second.size()));
        for (char byte : record.second) {
            data.push_back(static_cast<unsigned char>(byte));
        }
    }
    return data;
}

DecodeResult decode_records(const std::vector<unsigned char>& data) {
    DecodeResult result;
    result.ok = true;
    result.bad_offset = 0;

    std::size_t offset = 0;
    while (offset < data.size()) {
        const std::size_t start = offset;
        if (data.size() - offset < 2) {
            result.ok = false;
            result.bad_offset = start;
            return result;
        }
        const unsigned char tag = data[offset];
        const std::size_t length = data[offset + 1];
        offset += 2;
        if (data.size() - offset < length) {
            result.ok = false;
            result.bad_offset = start;
            return result;
        }
        result.records.emplace_back(
            tag, std::string(reinterpret_cast<const char*>(data.data() + offset),
                             length));
        offset += length;
    }
    return result;
}
