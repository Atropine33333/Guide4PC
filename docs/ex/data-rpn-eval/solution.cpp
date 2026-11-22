// 参考实现：data-rpn-eval
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<long long> stack;
    std::string token;

    while (std::cin >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            const long long right = stack.back();
            stack.pop_back();
            const long long left = stack.back();
            stack.pop_back();

            long long result = 0;
            if (token == "+") {
                result = left + right;
            } else if (token == "-") {
                result = left - right;
            } else if (token == "*") {
                result = left * right;
            } else {
                result = left / right;
            }
            stack.push_back(result);
        } else {
            stack.push_back(std::stoll(token));
        }
    }

    std::cout << stack.back() << '\n';
    return 0;
}
