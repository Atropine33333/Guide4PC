#include <string>
#include <vector>

class Notebook {
public:
    void add(const std::string& line) { lines_.push_back(line); }

    std::size_t size() const { return lines_.size(); }

    std::string at(std::size_t index) const {
        return index < lines_.size() ? lines_[index] : std::string("?");
    }

    std::string join(const std::string& separator) const {
        std::string result;
        for (std::size_t i = 0; i < lines_.size(); ++i) {
            if (i != 0) {
                result += separator;
            }
            result += lines_[i];
        }
        return result;
    }

private:
    std::vector<std::string> lines_;
};
