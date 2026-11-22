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
    (void)year;
    // TODO: 4 年一闰，100 年不闰，400 年再闰。
    return false;
}

int Date::days_in_month(int year, int month) {
    (void)year;
    (void)month;
    // TODO: 非法月份返回 0；二月考虑闰年。
    return 0;
}

bool Date::valid() const {
    // TODO: 检查年、月、日的范围。
    return false;
}

void Date::next_day() {
    // TODO: 推进一天；非法日期和 9999-12-31 不变。
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
    (void)other;
    // TODO: 按年、月、日依次比较。
    return false;
}

bool Date::operator==(const Date& other) const {
    (void)other;
    // TODO: 三个字段都相等。
    return false;
}
