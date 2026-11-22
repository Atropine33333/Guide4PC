// Reference solution: io-date-parse
#include <iomanip>
#include <sstream>
#include <string>

struct Date {
    int year;
    int month;
    int day;
};

namespace {

bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

int two_digits(const std::string& text, std::size_t pos) {
    return (text[pos] - '0') * 10 + (text[pos + 1] - '0');
}

bool is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int days_in_month(int year, int month) {
    static const int kDays[] = {0,  31, 28, 31, 30, 31, 30,
                                31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap(year)) return 29;
    return kDays[month];
}

}  // namespace

bool parse_iso_date(const std::string& text, Date& out) {
    if (text.size() != 10) return false;
    if (text[4] != '-' || text[7] != '-') return false;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (i == 4 || i == 7) continue;
        if (!is_digit(text[i])) return false;
    }

    const int year = (text[0] - '0') * 1000 + (text[1] - '0') * 100 +
                     (text[2] - '0') * 10 + (text[3] - '0');
    const int month = two_digits(text, 5);
    const int day = two_digits(text, 8);

    if (year < 1 || year > 9999) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > days_in_month(year, month)) return false;

    out.year = year;
    out.month = month;
    out.day = day;
    return true;
}

std::string format_short_date(const Date& date) {
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << date.day << '/' << std::setw(2)
        << date.month << '/' << std::setw(4) << date.year;
    return out.str();
}
