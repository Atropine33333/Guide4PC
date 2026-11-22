#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<long long> values(static_cast<std::size_t>(n));
    for (long long& value : values) std::cin >> value;
    std::sort(values.begin(), values.end());

    long long best_value = values[0];
    int best_count = 0;
    int run = 1;

    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i] == values[i - 1]) {
            ++run;
        } else {
            if (run >= best_count) {
                best_count = run;
                best_value = values[i - 1];
            }
            run = 1;
        }
    }
    if (run >= best_count) {
        best_count = run;
        best_value = values.back();
    }

    std::cout << best_value << '\n';
    return 0;
}
