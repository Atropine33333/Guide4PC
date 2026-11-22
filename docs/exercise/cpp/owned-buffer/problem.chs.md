实现一个自己拥有动态数组的整数缓冲区。

实现 `IntBuffer`：

- `explicit IntBuffer(int size)`：分配 `size` 个整数，初始值都是 `0`；`size` 可以是 `0`。
- `IntBuffer(const IntBuffer& other)`：深拷贝，两个缓冲区之后互不影响。
- `IntBuffer& operator=(const IntBuffer& other)`：深拷贝赋值，支持自赋值且不泄漏旧内存。
- `~IntBuffer()`：释放自己拥有的内存。
- `int size() const`：返回长度。
- `bool set(int index, int value)`：下标合法时写入并返回 `true`，越界返回 `false`。
- `bool get(int index, int& out) const`：下标合法时读出并返回 `true`，越界返回 `false`。
- `void fill(int value)`：把所有元素设为 `value`。

不要使用 `std::vector` 作为内部存储；这一题要自己管理 `new[]` / `delete[]`。反复构造、拷贝和赋值时，务必保证没有双重释放或悬空指针。
