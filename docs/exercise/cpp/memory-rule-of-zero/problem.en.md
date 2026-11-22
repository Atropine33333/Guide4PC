Hui Shi said to Zhuangzi: "I have a thing called `Notebook`. Inside it sits one `std::vector<std::string>`, holding a number of lines. I intend to write it a destructor, a copy, an assignment, a move — all five, and only then dare to use it."

Zhuangzi said: "Writing those five yourself is like taking the knife from the butcher. The compiler's claws are sharp; let it carve."

Hui Shi said: "If I write nothing, the copy is shallow; if I write nothing, memory leaks. I am afraid."

Zhuangzi said: "What you fear is a naked pointer. What you hold is a `std::vector`: it deep-copies itself, moves its contents, and releases the old ones. Keep your hands off and both deep and shallow are right. Stretch out one hand to write one of the five, and the others fall away with it."

Hui Shi said: "Then what of `at` out of range?"

Zhuangzi said: "Return `"?"`. And `join` connects the lines with `separator`; with no lines it returns an empty string. `add` appends one line; `size` reports the count. None of this deserves a sigh."

Hui Shi said: "One thing remains: may I write `main`?"

Zhuangzi said: "Heaven will punish it. How could it be escaped!"

**In plain words**: implement `Notebook` holding exactly one `std::vector<std::string>`. Declare none of the destructor, copy constructor, copy assignment, move constructor, or move assignment; let the compiler generate them. `add` appends a line; `at` returns line `index`, or `"?"` when out of range; `join` connects all lines with `separator`, returning an empty string when there are none. Do not write `main`. A copy must be independent, and a moved-from notebook must leave the target with the original content.

```cpp
#include <string>
#include <vector>

class Notebook {
public:
    void add(const std::string& line);
    std::size_t size() const;
    std::string at(std::size_t index) const;
    std::string join(const std::string& separator) const;
};
```
