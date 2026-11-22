// Guide4PC harness：拼在学生代码之后，同一个翻译单元。
#include <iostream>

namespace {

int g_total = 0;
int g_failed = 0;

void report(const char *id, bool ok) {
    ++g_total;
    if (!ok) ++g_failed;
    std::cout << "@@G4PC {\"t\":\"check\",\"id\":\"" << id
              << "\",\"ok\":" << (ok ? "true" : "false") << "}\n";
}

}  // namespace

int main() {
    report("classify-digits",
           classify_byte('0') == ByteKind::Digit &&
               classify_byte('5') == ByteKind::Digit &&
               classify_byte('9') == ByteKind::Digit &&
               classify_byte('/') == ByteKind::Printable &&
               classify_byte(':') == ByteKind::Printable);

    report("classify-space",
           classify_byte(' ') == ByteKind::Whitespace &&
               classify_byte('\t') == ByteKind::Whitespace &&
               classify_byte('\n') == ByteKind::Whitespace &&
               classify_byte('\v') == ByteKind::Whitespace &&
               classify_byte('\f') == ByteKind::Whitespace &&
               classify_byte('\r') == ByteKind::Whitespace);

    report("classify-control",
           classify_byte(0) == ByteKind::Control &&
               classify_byte(8) == ByteKind::Control &&
               classify_byte(14) == ByteKind::Control &&
               classify_byte(31) == ByteKind::Control &&
               classify_byte(127) == ByteKind::Control);

    report("classify-printable",
           classify_byte('!') == ByteKind::Printable &&
               classify_byte('A') == ByteKind::Printable &&
               classify_byte('z') == ByteKind::Printable &&
               classify_byte('~') == ByteKind::Printable &&
               classify_byte('@') == ByteKind::Printable &&
               classify_byte('`') == ByteKind::Printable &&
               classify_byte('{') == ByteKind::Printable);

    report("classify-high",
           classify_byte(128) == ByteKind::High &&
               classify_byte(200) == ByteKind::High &&
               classify_byte(255) == ByteKind::High &&
               classify_byte(0x80) == ByteKind::High);

    report("lower-basic", to_lower('A') == 'a' && to_lower('Z') == 'z' &&
                              to_lower('M') == 'm' && to_lower('a') == 'a' &&
                              to_lower('0') == '0' && to_lower('[') == '[' &&
                              to_lower(-1) == -1);

    report("hex-basic", hex_byte(0) == "00" && hex_byte(15) == "0F" &&
                            hex_byte(16) == "10" && hex_byte(128) == "80" &&
                            hex_byte(255) == "FF" && hex_byte(171) == "AB");

    {
        bool ok = true;
        for (int value = 0; value <= 255; ++value) {
            const unsigned char byte = static_cast<unsigned char>(value);
            const ByteKind kind = classify_byte(byte);
            const bool high = value >= 128;
            const bool digit = value >= '0' && value <= '9';
            const bool space = value == 9 || value == 10 || value == 11 ||
                               value == 12 || value == 13 || value == 32;
            const bool control = value < 32 || value == 127;
            bool expected = false;
            if (space) expected = kind == ByteKind::Whitespace;
            else if (control) expected = kind == ByteKind::Control;
            else if (digit) expected = kind == ByteKind::Digit;
            else if (high) expected = kind == ByteKind::High;
            else expected = kind == ByteKind::Printable;
            if (!expected) ok = false;
            const std::string text = hex_byte(byte);
            if (text.size() != 2) ok = false;
            for (char c : text) {
                if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'))) ok = false;
            }
        }
        report("all-bytes", ok);
    }

    std::cout << "@@G4PC {\"t\":\"done\",\"passed\":" << (g_total - g_failed)
              << ",\"total\":" << g_total << "}\n";
    return g_failed == 0 ? 0 : 1;
}
