#include <ostream>
#include <string>

std::string format_row(const std::string& name, long long amount) {
    // TODO: 没有补齐宽度，只是用空格拼起来。
    return name + " | " + std::to_string(amount);
}

void print_row(std::ostream& out, const std::string& name, long long amount) {
    // TODO: 没有处理换行，也没有复用 format_row。
    out << name << ' ' << amount;
}
