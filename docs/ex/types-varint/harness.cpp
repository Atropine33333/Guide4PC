// Guide4PC harness: compiled after the student's code in the same translation unit.
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char* id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

using Bytes = std::vector<std::uint8_t>;

bool encoded(std::uint32_t value, const Bytes& expected) {
    return encode_varint(value) == expected;
}

}  // namespace

int main() {
    report("encode-small", encoded(0, {0x00}) && encoded(1, {0x01}) &&
                               encoded(127, {0x7f}));

    report("encode-boundaries",
           encoded(128, {0x80, 0x01}) && encoded(255, {0xff, 0x01}) &&
               encoded(300, {0xac, 0x02}) && encoded(16383, {0xff, 0x7f}) &&
               encoded(16384, {0x80, 0x80, 0x01}) &&
               encoded(2097151, {0xff, 0xff, 0x7f}) &&
               encoded(2097152, {0x80, 0x80, 0x80, 0x01}) &&
               encoded(4294967295u, {0xff, 0xff, 0xff, 0xff, 0x0f}));

    {
        bool ok = true;
        std::uint32_t state = 0x12345678u;
        for (int i = 0; i < 200 && ok; ++i) {
            state = state * 1664525u + 1013904223u;
            const Bytes bytes = encode_varint(state);
            std::uint32_t value = 0;
            std::size_t consumed = 0;
            if (!decode_varint(bytes, value, consumed)) {
                ok = false;
            } else if (value != state || consumed != bytes.size()) {
                ok = false;
            } else if (encode_varint(value) != bytes) {
                ok = false;
            }
        }
        report("roundtrip", ok);
    }

    {
        std::uint32_t value = 777;
        std::size_t consumed = 88;
        const bool empty = !decode_varint({}, value, consumed);
        const bool one = !decode_varint({0x80}, value, consumed);
        const bool two = !decode_varint({0x80, 0x80}, value, consumed);
        const bool four = !decode_varint({0xff, 0xff, 0xff, 0xff}, value, consumed);
        const bool untouched = value == 777 && consumed == 88;
        report("decode-truncated", empty && one && two && four && untouched);
    }

    {
        std::uint32_t value = 1234;
        std::size_t consumed = 55;
        const bool noncanonical_zero = !decode_varint({0x80, 0x00}, value, consumed);
        const bool noncanonical_one = !decode_varint({0x81, 0x00}, value, consumed);
        const bool noncanonical_long = !decode_varint({0x80, 0x80, 0x00}, value, consumed);
        const bool overflow = !decode_varint({0xff, 0xff, 0xff, 0xff, 0x10}, value, consumed);
        const bool too_long = !decode_varint({0x80, 0x80, 0x80, 0x80, 0x80, 0x01}, value, consumed);
        const bool untouched = value == 1234 && consumed == 55;
        report("decode-canonical",
               noncanonical_zero && noncanonical_one && noncanonical_long &&
                   overflow && too_long && untouched);
    }

    {
        std::uint32_t value = 0;
        std::size_t consumed = 0;
        const bool first = decode_varint({0xac, 0x02, 0x7f}, value, consumed) &&
                           value == 300 && consumed == 2;
        const bool second = decode_varint({0x0f, 0xaa}, value, consumed) &&
                            value == 15 && consumed == 1;
        std::uint32_t max_value = 0;
        std::size_t max_consumed = 0;
        const bool third = decode_varint({0xff, 0xff, 0xff, 0xff, 0x0f}, max_value,
                                         max_consumed) &&
                           max_value == 4294967295u && max_consumed == 5;
        report("decode-consumed", first && second && third);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
