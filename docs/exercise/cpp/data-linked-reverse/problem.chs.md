话说这是一条单链表，结点一个扣着一个，`value` 是货，`next` 是绳。列位看官，链子初来时是空的，一颗结点也无；`push_front` 只管往头上一挂，一挂便是 O(1)。

且说这一日，台上要演一出“原地倒转”。管事的早有明令：不许另起一条链子来装货，也不许只把货两两对调——绳子的方向，得一节一节真翻过来。

只见那位把式手里捏着三根指头：一根指着前头，一根指着当下，一根指着后头。他先把后头的绳头记在掌心，再把当下的绳回头系到前头，然后三根指头一齐往前挪一步。挪到绳尾，前头那根指着的，便是新的链头。

列位看官，这一路过去，一条链翻了个身，`size_` 一个数没多、一个数没少。散场时析构函数还得把每一颗结点都收了，空链子也得收得干净，才算功德圆满。

**人话**：完成单链表类 `IntList`，不读 stdin、不写 stdout，测试在隐藏的 harness 里。`push_front` 在头部插入一个值，O(1)；`reverse` 原地反转，不能新建第二条链表，也不能只交换值，维护前一个、当前、后一个三个指针即可；`head()` 返回从表头到 `next == nullptr` 的结点链，空链表返回 `nullptr`；`size()` 返回结点数，反转不改变它、插入时要更新；析构函数释放全部结点，空链表也安全。拷贝构造与拷贝赋值已声明为 `= delete`，不要实现。不要写 `main`，不要修改公有接口。

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
