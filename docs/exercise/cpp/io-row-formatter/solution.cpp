// 参考实现：io-row-formatter
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

std::string format_row(const std::string& name, long long amount) {
    std::ostringstream out;
    out << std::setfill(' ') << std::left << std::setw(12) << name << '|'
        << std::right << std::setw(10) << amount;
    return out.str();
}

void print_row(std::ostream& out, const std::string& name, long long amount) {
    out << format_row(name, amount) << '\n';
}
