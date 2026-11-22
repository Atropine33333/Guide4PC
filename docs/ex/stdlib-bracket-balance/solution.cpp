// 参考实现：stdlib-bracket-balance
#include <string>
#include <vector>

bool balanced(const std::string& text) {
    std::vector<char> stack;
    for (char c : text) {
        if (c == '(' || c == '[' || c == '{') {
            stack.push_back(c);
        } else {
            if (stack.empty()) return false;
            const char open = stack.back();
            stack.pop_back();
            if (c == ')' && open != '(') return false;
            if (c == ']' && open != '[') return false;
            if (c == '}' && open != '{') return false;
        }
    }
    return stack.empty();
}
