实现一个单向整数链表。

实现 `IntList`，提供以下成员：

- `IntList()`：构造空链表。
- `~IntList()`：释放链表占用的所有结点，不能泄漏。
- `void pushFront(int value)`：在头部插入一个元素。
- `bool popFront()`：删除头结点并返回 `true`；链表为空时返回 `false`，不做任何事。
- `void reverse()`：就地反转整条链表，不分配新结点。
- `bool get(int index, int& out) const`：`index` 从 0 开始；合法时把值写入 `out` 并返回 `true`，越界返回 `false`。
- `int size() const`：返回元素个数。

不要使用 `std::list` 或 `std::vector` 作为内部存储；这一题要自己管理结点指针，确保析构函数正确释放内存。
