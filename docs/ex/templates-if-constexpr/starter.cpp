#include <string>
#include <type_traits>

template <typename T>
std::string classify(const T& value) {
    // TODO: 现在所有类型都返回 "integral"，也没有使用 if constexpr。
    (void)value;
    return "integral";
}

template <typename T>
std::string render(const T& value) {
    // TODO: 还没有按类型渲染，只返回占位符。
    (void)value;
    return "?";
}
