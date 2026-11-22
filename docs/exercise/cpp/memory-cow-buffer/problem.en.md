(A drawing room. A fireplace. Two gentlemen, each with a glass.)

"Oh, my dear fellow, look at this manuscript — the two of us are sharing one and the same sheet."

"Good heavens, how economical. Only, when you put pen to it, do keep the ink off my page."

"Hence the name: copy on write. Copy construction and copy assignment merely enter the sheet in the register; nobody duplicates it, and `live_blocks()` does not stir."

"And `shares()`?"

"Count the gentlemen who share this sheet, yourself included. One at the very least."

"But I wish to add a sentence. The moment `append` arrives, it must first ask whether the sheet is mine alone. If `shares() > 1`, copy it out for myself, then write. The other fellow's manuscript loses not a single character."

"And if I assign the sheet to myself?"

"My dear, then do nothing at all. As for the sheet that gets replaced, when no one uses it any longer, burn it on the spot."

"And an empty manuscript?"

"An empty manuscript must still own a sheet. `live_blocks()` records it like any other."

**In plain words**: implement `CowBuffer`. Copy construction and copy assignment share the underlying storage: they copy no string and do not increase `live_blocks()`. `shares()` returns how many `CowBuffer` objects share that storage, itself included, at least 1. `append` first ensures unique storage: when `shares() > 1`, clone the storage before appending, and the other sharing object's string must not change. `live_blocks()` returns the number of distinct storage blocks alive right now. Copy assignment handles self-assignment, and a replaced block with no users is released promptly. An empty buffer must still own a valid block. Do not write `main`, and do not change the public interface.

```cpp
#include <cstddef>
#include <string>

class CowBuffer {
public:
    CowBuffer();
    explicit CowBuffer(const std::string& data);
    CowBuffer(const CowBuffer& other);
    CowBuffer& operator=(const CowBuffer& other);
    ~CowBuffer();

    const std::string& str() const;
    void append(const std::string& suffix);
    std::size_t shares() const;

    static int live_blocks();
};
```
