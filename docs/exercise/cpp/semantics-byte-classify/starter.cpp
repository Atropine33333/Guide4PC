#include <string>

enum class ByteKind { Control, Whitespace, Digit, Printable, High };

ByteKind classify_byte(unsigned char byte) {
    (void)byte;
    // TODO: 按区间分类，注意 DEL(127) 属于 Control，128 起属于 High。
    return ByteKind::Printable;
}

int to_lower(int ch) {
    // TODO: 只把大写字母转成小写，其余原样返回。
    return ch;
}

std::string hex_byte(unsigned char byte) {
    (void)byte;
    // TODO: 返回两位大写十六进制，高位在前。
    return "00";
}
