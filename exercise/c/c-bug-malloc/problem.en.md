USER MANUAL: THE SQUARE-BRICK DEPOT

Model SQ-4096. Purpose: cut a batch of square bricks on demand, and give the plot back when the work is done.

**Installation**: implement the three functions below and wire them into the main program:

```c
#define SQ_MAX 4096

int *make_squares(int n);
void release_squares(int *p);
long live_blocks(void);
```

**Operating rules**

1. `make_squares(n)`: reserve a plot that holds `n` `int`s, fill it with `0, 1, 4, ..., (n-1)*(n-1)`, and return the address of its first slot. `n` must lie between `1` and `SQ_MAX`; when it is out of range, or the system has no plot to give, return `NULL` and leave the ledger untouched.
2. `release_squares(p)`: give back a plot obtained from `make_squares` and drop the ledger by one. When `p` is `NULL`, do nothing.
3. `live_blocks()`: report how many plots are still on the ledger.

**Observed fault**: on this unit the issue desk still hands plots out, but the return desk sits empty, so the ledger only climbs; and the desk never checks whether the system actually granted a plot before writing into it.

**In plain words**: repair `make_squares` and `release_squares`: check the grant before writing, refuse out-of-range requests, really give a plot back and settle the ledger, and leave the ledger alone for a null pointer.

**Interface**:

```c
#define SQ_MAX 4096

int *make_squares(int n);
void release_squares(int *p);
long live_blocks(void);
```

### Example

```c
int *p = make_squares(3);   /* p[0]=0, p[1]=1, p[2]=4; one plot on the ledger */
release_squares(p);         /* given back; the ledger reads 0 */
release_squares(NULL);      /* does nothing; the ledger still reads 0 */
```

### Contract

- Do not change `SQ_MAX`, and do not write `main`.
- Give each plot back exactly once; `release_squares` only accepts pointers returned by `make_squares`.
- Keep the ledger exact: after every plot has been given back, `live_blocks()` must read `0`.
- A failed request returns `NULL` and must not add anything to the ledger.
