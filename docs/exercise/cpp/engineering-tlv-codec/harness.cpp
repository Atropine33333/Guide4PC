// Guide4PC harness appended to the student's code in the same translation unit.
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
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

using Records = std::vector<std::pair<unsigned char, std::string>>;

bool same_records(const Records& a, const Records& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i].first != b[i].first || a[i].second != b[i].second) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main() {
    {
        const Records records = {{1, "A"}, {2, "BC"}};
        const std::vector<unsigned char> encoded = encode_records(records);
        const std::vector<unsigned char> expected = {1, 1, 'A', 2, 2, 'B', 'C'};
        report("encode-basic", encoded == expected);
    }

    {
        const Records records = {{9, "hello"}, {0, ""}, {255, "z"}};
        const DecodeResult decoded = decode_records(encode_records(records));
        report("round-trip",
               decoded.ok && decoded.bad_offset == 0 &&
                   same_records(decoded.records, records));
    }

    {
        const DecodeResult empty = decode_records({});
        const DecodeResult decoded = decode_records(encode_records({}));
        report("empty-input", empty.ok && empty.records.empty() &&
                                  empty.bad_offset == 0 && decoded.ok &&
                                  decoded.records.empty());
    }

    {
        const Records records = {{7, ""}};
        const std::vector<unsigned char> encoded = encode_records(records);
        const DecodeResult decoded = decode_records(encoded);
        report("empty-payload",
               encoded == std::vector<unsigned char>{7, 0} && decoded.ok &&
                   same_records(decoded.records, records));
    }

    {
        const std::string payload(255, 'z');
        const Records records = {{42, payload}};
        const std::vector<unsigned char> encoded = encode_records(records);
        const DecodeResult decoded = decode_records(encoded);
        report("length-255",
               encoded.size() == 257 && decoded.ok &&
                   same_records(decoded.records, records));
    }

    {
        const DecodeResult no_tag = decode_records({5});
        const DecodeResult no_payload = decode_records({3, 4, 'a', 'b'});
        report("truncated-first", !no_tag.ok && no_tag.records.empty() &&
                                      no_tag.bad_offset == 0 &&
                                      !no_payload.ok && no_payload.records.empty() &&
                                      no_payload.bad_offset == 0);
    }

    {
        const std::vector<unsigned char> data = {1, 1, 'x', 2, 5, 'y'};
        const DecodeResult decoded = decode_records(data);
        report("bad-offset", !decoded.ok && decoded.bad_offset == 3 &&
                                 same_records(decoded.records, {{1, "x"}}));
    }

    {
        std::vector<unsigned char> data = encode_records({{8, "ok"}});
        const std::size_t extra = data.size();
        data.push_back(9);
        const DecodeResult decoded = decode_records(data);
        report("trailing-garbage", !decoded.ok && decoded.bad_offset == extra &&
                                       same_records(decoded.records, {{8, "ok"}}));
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
