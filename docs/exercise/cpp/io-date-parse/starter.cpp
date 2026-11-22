#include <string>

struct Date {
    int year;
    int month;
    int day;
};

bool parse_iso_date(const std::string& text, Date& out) {
    (void)text;
    (void)out;
    // TODO: validate the shape and the calendar values.
    return false;
}

std::string format_short_date(const Date& date) {
    (void)date;
    // TODO: produce DD/MM/YYYY.
    return "";
}
