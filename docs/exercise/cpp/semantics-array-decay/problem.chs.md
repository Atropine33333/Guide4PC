《数组观测站入站公告》

本站不设检票口，也不收取任何费用。入站者须先读壁上守则，读毕再动手。守则全部为真，长期有效；它们读起来彼此不睦，只因常识在此地并不适用，守则本身没有写错。

**人话**：补全下面几个函数：报告整个数组与指针各自的字节数、在两个同名重载间分辨实参、对六个元素求和、对左闭右开区间求和。差别只落在形参写法上，先把签名抄对，再照守则实现。

```cpp
#include <cstddef>
#include <string>

std::size_t array_bytes(const int (&values)[6]);
std::size_t pointer_bytes(const int* values);
std::string which_overload(int (&values)[6]);
std::string which_overload(const int* values);
int sum_array(const int (&values)[6]);
int sum_range(const int* first, const int* last);
```

### 守则

一、具名数组被送进函数时，若形参写作指针，它当场退化为指向首元素的指针，长度随之消失；此时 `sizeof(values)` 量到的是指针自己的宽度。`pointer_bytes` 照此办理。

二、若形参写作 `const int (&)[6]`，数组保持原形进入函数，长度与元素一同抵达；此时 `sizeof(values)` 量到的是整个数组的字节数。`array_bytes` 照此办理。

三、两个 `which_overload` 构成重载。实参为数组时选数组引用一版，返回 `"array"`；实参为指针时选指针一版，返回 `"pointer"`。指针可以是 `values + 0`，也可以是 `&values[0]`；这条从无例外。

四、`sum_array` 返回六个元素之和。

五、`sum_range` 只认左闭右开区间 `[first, last)`；两端相等时交回 0，不作任何解释。

六、不得写 `main`，不得改动函数签名。
