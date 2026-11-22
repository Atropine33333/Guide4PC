《共享数书》

耶稣在海边坐下，众人聚集到他跟前，他就用比喻说：

“天国好像一个主人，把一份产业交在众人手里。众人共用一处库房，库里只有一个数，一本账。”

门徒问：“主啊，这账如何记？”

耶稣说：“每一人领受这产业，账上便加一。谁要把产业让与别人，先还清自己那一份，再记新账；自让自受的，账上分文不动。凡借着这库房改数的，众人一同看见新数，因为库房只有一处。”

“时候到了，各人要交账。账上减至零，库房便拆了，尘土归尘土。”

**人话**：实现 `SharedInt`：每个对象只存一个指向控制块的指针，控制块里有一个 `int` 值和一个引用计数。拷贝构造与拷贝赋值让计数加一，并共享值；赋值先释放左操作数原来的引用，再共享右操作数的数据，自赋值必须安全；析构时计数减一，减到 0 才释放控制块；通过任意副本调用 `set_value`，其它副本都能看到新值。不要写 `main`。

```cpp
class SharedInt {
public:
    explicit SharedInt(int value = 0);
    SharedInt(const SharedInt& other);
    SharedInt& operator=(const SharedInt& other);
    ~SharedInt();

    int value() const;
    void set_value(int value);
    int use_count() const;  // 当前共享同一块数据的对象数
};
```

经上又记着说：控制块可以写作私有的嵌套 `struct Control { int value; int count; };`，对象里只存 `Control*`。
