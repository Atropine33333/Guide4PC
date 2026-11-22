// 参考实现：templates-deduce-guide
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector>

template <typename T>
class Seq {
public:
    Seq() = default;
    Seq(std::initializer_list<T> items) : items_(items) {}

    template <typename It, typename = std::enable_if_t<!std::is_integral_v<It>>>
    Seq(It first, It last) : items_(first, last) {}

    template <typename U, std::size_t N>
    Seq(const U (&items)[N]) : items_(items, items + N) {}

    std::size_t size() const { return items_.size(); }

    T sum() const {
        T total{};
        for (const T& item : items_) total += item;
        return total;
    }

    bool contains(const T& value) const {
        for (const T& item : items_) {
            if (item == value) return true;
        }
        return false;
    }

    const T& at(std::size_t index) const { return items_[index]; }

private:
    std::vector<T> items_;
};

template <typename It>
Seq(It, It) -> Seq<typename std::iterator_traits<It>::value_type>;

template <typename T, std::size_t N>
Seq(const T (&)[N]) -> Seq<T>;
