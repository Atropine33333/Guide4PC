(Eight cards are laid out on the table.)

The reader shuffles, face down, and pushes them toward you one at a time.

"The table has eight seats, numbered `0` to `7`. Each seat carries a number called a generation, and it starts at `1`."

"When you draw a card, you write a value into a free seat, and I record that seat's current generation on the card as `{index, generation}`. The card is yours. When all eight seats are full, the next card I deal reads `{kInvalidIndex, 0}`."

"How long can a card be trusted?"

"The seat must exist, the generation must agree, and the seat must still be occupied. `valid` nods only at such a card. `get` reports nothing for a stale card, and points at the seat's value for a valid one."

"And discarding?"

"`destroy` takes the card from your hand and bumps that seat's generation by one. The old card is void from then on; even if someone sits in the same seat next round, your old card no longer counts. Bring me a stale card to discard, and I merely shake my head — not one card on the table moves."

"Those are all the numbers: `size()` reports how many seats are taken, `capacity()` reports `kCapacity`. Do not deal your own table, and do not alter the cards. And remember: no `main`."

**In plain words**: implement `HandleStore` with eight seats, each generation starting at `1`. `create` stores a value in a free seat and returns a handle with that seat's current generation, or `{kInvalidIndex, 0}` when the store is full. `destroy` frees the seat, bumps its generation, and returns `true` for a valid handle; for an invalid or stale handle it returns `false` and changes nothing. `valid` requires the index in range, the generation matching, and the seat occupied. `get` returns `nullptr` for an invalid handle and a pointer to the seat's value for a valid one. An old handle must go stale once its seat is reused, even at the same index. `size()` reports occupied seats and `capacity()` reports `kCapacity`. Do not change the public interface.

```cpp
#include <cstddef>
#include <cstdint>

struct Handle {
    std::uint32_t index;
    std::uint32_t generation;
};

class HandleStore {
public:
    static constexpr std::uint32_t kCapacity = 8;
    static constexpr std::uint32_t kInvalidIndex = 0xffffffffu;

    HandleStore();

    Handle create(int value);
    bool valid(Handle handle) const;
    int* get(Handle handle);
    const int* get(Handle handle) const;
    bool destroy(Handle handle);
    std::size_t size() const;
    std::size_t capacity() const;
};
```
