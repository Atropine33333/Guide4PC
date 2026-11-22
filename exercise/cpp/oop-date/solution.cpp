// 参考实现：oop-date
class Date {
public:
    Date(int year, int month, int day);

    bool valid() const;
    void next_day();

    int year() const;
    int month() const;
    int day() const;

    static bool is_leap(int year);
    static int days_in_month(int year, int month);

    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;

private:
    int year_;
    int month_;
    int day_;
};

Date::Date(int year, int month, int day)
    : year_(year), month_(month), day_(day) {}

bool Date::is_leap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

int Date::days_in_month(int year, int month) {
    if (month < 1 || month > 12) return 0;
    if (month == 2) return is_leap(year) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

bool Date::valid() const {
    if (year_ < 1 || year_ > 9999) return false;
    if (month_ < 1 || month_ > 12) return false;
    return day_ >= 1 && day_ <= days_in_month(year_, month_);
}

void Date::next_day() {
    if (!valid()) return;
    if (year_ == 9999 && month_ == 12 && day_ == 31) return;

    ++day_;
    if (day_ > days_in_month(year_, month_)) {
        day_ = 1;
        ++month_;
        if (month_ > 12) {
            month_ = 1;
            ++year_;
        }
    }
}

int Date::year() const {
    return year_;
}

int Date::month() const {
    return month_;
}

int Date::day() const {
    return day_;
}

bool Date::operator<(const Date& other) const {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
}

bool Date::operator==(const Date& other) const {
    return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}
