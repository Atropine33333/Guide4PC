MEMORY ESTABLISHMENT OFFICE

Document No. 19 of 2026

NOTICE ON IMPROVING THE REGISTRATION OF LIVE OBJECTS

To all scopes and to all temporary objects:

In order to establish an accurate count of registered objects and to eliminate missing, duplicated, and inflated records, this Office has decided to place the class `Tracked` under live-object registration. The requirements are as follows.

1. Scope of registration. Every `Tracked` object is registered upon construction and struck from the register upon destruction. A copy construction opens a new account and shall not be merged with the original.

2. The approved interface follows. Members shall not be added or removed.

- `Tracked()`: constructs with value `0`.
- `explicit Tracked(int value)`: constructs with value `value`.
- `Tracked(const Tracked& other)`: copy construction copies the value; this is a **new object**.
- `Tracked& operator=(const Tracked& other)`: assignment copies the value; the object count does not change, and self-assignment is safe.
- `~Tracked()`: destructor.
- `int value() const` and `void setValue(int value)`: read and write the value.
- `static int alive()`: return the number of live objects.

3. Bookkeeping discipline. The count starts at `0`. Construction and copy construction each add one; destruction subtracts one; assignment adds nothing and subtracts nothing. Self-assignment must leave the books untouched.

4. Audit. Objects are created in nested scopes, copied, and stored in arrays; when each scope ends the count must return to zero.

5. Do not write `main`. This Notice takes effect on the date of issue.

Copies: Stack Management Committee, Heap Management Committee, Destruction Settlement Group.

**In plain words**: implement `Tracked`. Construction and copy construction increment `alive()`; destruction decrements it; assignment leaves it alone and self-assignment is safe. Read and write the value through `value()` and `setValue()`.
