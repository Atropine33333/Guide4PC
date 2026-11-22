// 参考实现：semantics-byte-classify
#include <string>

enum class ByteKind { Control, Whitespace, Digit, Printable, High };

ByteKind classify_byte(unsigned char byte) {
    const unsigned value = byte;
    if (value == 9 || value == 10 || value == 11 || value == 12 || value == 13 ||
        value == 32) {
        return ByteKind::Whitespace;
    }
    if (value < 32 || value == 127) return ByteKind::Control;
    if (value >= '0' && value <= '9') return ByteKind::Digit;
    if (value >= 128) return ByteKind::High;
    return ByteKind::Printable;
}

int to_lower(int ch) {
    if (ch >= 'A' && ch <= 'Z') return ch + ('a' - 'A');
    return ch;
}

std::string hex_byte(unsigned char byte) {
    static const char digits[] = "0123456789ABCDEF";
    const unsigned value = byte;
    std::string out;
    out.push_back(digits[(value >> 4) & 0x0F]);
    out.push_back(digits[value & 0x0F]);
    return out;
}
