这一题不读 stdin，也不写 stdout。完成一个 singly linked list（单链表）类，测试在隐藏的 harness 里。

```cpp
struct Node {
    int value;
    Node* next;
};

class IntList {
public:
    IntList();
    ~IntList();
    IntList(const IntList&) = delete;
    IntList& operator=(const IntList&) = delete;

    void push_front(int value);
    void reverse();
    int size() const;
    const Node* head() const;

private:
    Node* head_;
    int size_;
};
```

规则：

- 新链表为空：`head()` 返回 `nullptr`，`size()` 返回 0。
- `push_front` 在头部插入一个值，O(1)。
- `reverse` 原地反转链表，不能新建第二个链表来装元素，也不能只交换值。
- 析构函数释放所有结点，空链表也要安全。
- 拷贝构造和拷贝赋值已声明为 `= delete`，不要实现它们。
- `head()` 返回从表头开始的结点链，最后一个是 `next == nullptr`。
- 不要写 `main`，也不要修改上面的公有接口。

### 提示

反转时维护三个指针：前一个、当前、后一个。`size_` 不随反转改变，但插入时要更新。
